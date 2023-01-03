#ifndef INCLUDE_STB_LOG_H
#include "log.h"
#endif
#ifdef STB_LOG_IMPLEMENTATION
#include <sys/stat.h>
#include <cstdarg>
#include <ctime>
#include <string>
#include <algorithm>
#include <cassert>

#define ASSERT_ALIGNMENT(ptr, align) assert((ptr) && ((uintptr_t(ptr) % (align)) == 0))

#ifdef USE_NAMESPACE
namespace STB_LOG_NAMESPACE {
#endif

// get global logger info
inline LogContext* get_log_context() {
	static LogContext s_logger_context;
	return &s_logger_context;
}

// get global logger
CLogger* get_logger()
{
	return get_log_context()->logger;
}

inline LogContext* add_log_handler(CLogHandler *handler)
{
	LogContext *lc = get_log_context();
	if (!lc->logger)
		lc->logger = new CLogger(LOG_QUEUE_SIZE);
	lc->logger->add_handler(handler);
	return lc;
}

void start_handler_thread(CLogHandler *handler, millisecond_t sleep_time) {
	LogContext *lc = add_log_handler(handler);
	std::chrono::milliseconds msec(sleep_time);
	lc->thread_pool.emplace_back(std::make_unique<std::thread>([handler, msec] {
		while (!handler->is_closed()) {
			handler->process();
			std::this_thread::sleep_for(msec);
		}
	}));
}

void close_logger() {
	LogContext *lc = get_log_context();
	if (!lc->logger)
		return;
	lc->logger->close();
	for (auto &th : lc->thread_pool) {
		th->join();
	}
	lc->thread_pool.clear();
	lc->logger->release_handlers();
	delete lc->logger;
	lc->logger = nullptr;
}

void start_logger(bool async, millisecond_t sleep_time) {
	CLogStdout *handler = new CLogStdout();
	handler->set_time_formatter(std::make_unique<CDateTimeFormatter>());
	if(async)
		start_handler_thread(handler, sleep_time);
	else
		add_log_handler(handler);
}

void start_file_logger(const char *log_file_path, bool append_mode, int max_rotation,
					   size_t rotate_size, bool async, millisecond_t sleep_time) {
	CLogFile *handler = new CLogFile(log_file_path, append_mode, max_rotation, rotate_size);
	handler->set_time_formatter(std::make_unique<CDateTimeFormatter>());
	if(async)
		start_handler_thread(handler, sleep_time);
	else
		add_log_handler(handler);
}
	
void start_string_logger(size_t buffer_size, bool async, millisecond_t sleep_time)
{
	CLogString *handler = new CLogString(buffer_size);
	handler->set_time_formatter(std::make_unique<CDateTimeFormatter>());
	if(async)
		start_handler_thread(handler, sleep_time);
	else
		add_log_handler(handler);
}

static void *stblog_aligned_alloc(size_t alignment, size_t size) {
	// [Memory returned][ptr to start of memory][aligned memory][extra memory]
	size_t request_size = size + alignment;
	void *raw = malloc(request_size + sizeof(void *));
	if (!raw)
		return nullptr;
	void *ptr = (void **) raw + 1;
	ptr = std::align(alignment, size, ptr, request_size);
	if (!ptr) {
		free(raw);
		return nullptr;
	}
	*((void **) ptr - 1) = raw;
	return ptr;
}

static void stblog_aligned_free(void *ptr) {
	void *raw = *((void **) ptr - 1);
	free(raw);
}

// The smallest integral power of two that is not smaller than x.
size_t bit_ceil(size_t x) {
	// val maybe power of 2
	--x;
	// set the bits right of MSB to 1
	x |= (x >> 1);
	x |= (x >> 2);
	x |= (x >> 4);
	x |= (x >> 8);        /* Ok, since int >= 16 bits */
#if (SIZE_MAX != 0xffff)
	x |= (x >> 16);        /* For 32 bit int systems */
#if (SIZE_MAX > 0xffffffffUL)
	x |= (x >> 32);        /* For 64 bit int systems */
#endif // SIZE_MAX != 0xffff
#endif // SIZE_MAX > 0xffffffffUL
	++x;
	assert((x & (x - 1)) == 0);
	return x;
}

// --------------------------------
// CLogger implementation
// --------------------------------
	
void* CLogger::operator new(size_t size) {
	return stblog_aligned_alloc(alignof(CLogger), size);
}

void CLogger::operator delete(void* ptr) {
	stblog_aligned_free(ptr);
}

CLogger::CLogger(size_t size) {
	assert(size > 0);
	if (size & (size - 1))
		size = bit_ceil(size);
	static_assert(sizeof(LogEvent) % CACHELINE_SIZE == 0, "LogEvent should be fit in cacheline.");
	size_t buf_size = sizeof(LogEvent) * size;
	m_event_queue = (LogEvent *) stblog_aligned_alloc(CACHELINE_SIZE, buf_size);
	ASSERT_ALIGNMENT(m_event_queue, CACHELINE_SIZE);
	m_size_mask = size - 1;
	for (size_t i = 0; i < size; ++i) {
		LogEvent *log = &m_event_queue[i];
		ASSERT_ALIGNMENT(log, CACHELINE_SIZE);
		ASSERT_ALIGNMENT(&log->publish, CACHELINE_SIZE);
		// initialize LogEvent
		new(log) LogEvent;
		log->publish.set(0);
	}
	m_seq_claim.set(0);
	m_min_seq = 0;
}

CLogger::~CLogger() {
	for (auto handler : m_handler_list) {
		handler->follow(nullptr);
	}
	for (size_t i = 0; i <= m_size_mask; ++i) {
		// clean up LogEvent
		LogEvent *log = &m_event_queue[i];
		log->~LogEvent();
	}
	stblog_aligned_free(m_event_queue);
	m_event_queue = nullptr;
}


uint64_t CLogger::claim(uint64_t count) {
	uint64_t request_seq = m_seq_claim.fetch_add(count);
	if (request_seq <= m_min_seq + m_size_mask) {
		return request_seq;
	}
	uint64_t min_seq = ULLONG_MAX, seq = 0;
	constexpr unsigned YIELD = 1;
	constexpr unsigned SLEEP = 2;
	constexpr unsigned SPIN = 10;
	unsigned spin_count = 0;
	unsigned loop_count = 0;
	// millisecond_t ms = 1;
	// TODO: need a better spin lock
	for (CLogHandler *handler : m_handler_list) {
		seq = handler->get_sequence();
		while (request_seq > seq + m_size_mask) {
			if(loop_count < YIELD) {
				for (spin_count = SPIN; spin_count > 0; --spin_count)
					_mm_pause(); // pause, about 12ns
			}
			else {
				unsigned yield_count = loop_count - YIELD;
				if(SLEEP > 0 && (yield_count % SLEEP == SLEEP - 1)) {
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
					loop_count = 0;
				}
				else {
					// if no waiting threads, about 113ns
					// else lead to thread switching
					std::this_thread::yield();
				}
			}
			++loop_count;
			seq = handler->get_sequence();
		}
		seq = handler->acquire_sequence();
		if (seq < min_seq)
			min_seq = seq;
	}
	m_min_seq = min_seq;
	return request_seq;
}

void CLogger::publish(std::shared_ptr<void>&& sptr) {
	auto seq = claim(1);
	auto *log = get_event(seq);
	log->data = std::move(sptr);
	log->publish.store(seq + 1);
}

void CLogger::add_handler(CLogHandler *handler) {
	m_handler_list.push_back(handler);
	handler->follow(this);
}

void CLogger::remove_handler(CLogHandler *handler) {
	for (auto iter = m_handler_list.begin(); iter != m_handler_list.end(); ++iter) {
		if (*iter == handler) {
			m_handler_list.erase(iter);
			handler->follow(nullptr);
			return;
		}
	}
}

void CLogger::release_handlers() {
	for (auto iter: m_handler_list) {
		iter->follow(nullptr);
		delete iter;
	}
	m_handler_list.clear();
}

void CLogger::close() {
	uint64_t seq = claim(1);
	LogEvent *log = get_event(seq);
	log->data = nullptr;
	log->publish.store(seq + 1);
}

// --------------------------------
// CLogHandler implementation
// --------------------------------

void* CLogHandler::operator new(size_t size) {
	return stblog_aligned_alloc(alignof(CLogHandler), size);
}

void CLogHandler::operator delete(void* ptr) {
	stblog_aligned_free(ptr);
}

CLogHandler::CLogHandler()
		: m_logger(nullptr), m_filter(nullptr), m_formatter(nullptr), m_closed(false)
{
	ASSERT_ALIGNMENT(this, CACHELINE_SIZE);
	ASSERT_ALIGNMENT(&m_seq, CACHELINE_SIZE);
	m_seq.set(0);
	m_batch.reserve(LOG_BATCH_SIZE);
}

CLogHandler::~CLogHandler() {
	if (m_logger) {
		m_logger->remove_handler(this);
		m_logger = nullptr;
	}
}

void CLogHandler::process() {
	assert(m_logger);
	uint64_t seq = m_seq.get(), pub;
	LogEvent *log;
	LogData *data;
	while (!m_closed) {
		log = m_logger->get_event(seq);
		pub = log->publish.load();
		if (pub <= seq)
			break;
		data = (LogData*)(log->data.get());
		if (!data) {
			m_closed = true;
		} else if (!m_filter || m_filter(data)) {
			m_batch.push_back(log->data);
		}
		m_seq.store(pub);
		seq += 1;
		assert(pub == seq);
		if (m_batch.size() >= LOG_BATCH_SIZE)
			flush();
	}
	if(!m_batch.empty())
		flush();
	if (m_closed)
		on_close();
}
	
void CLogHandler::flush()
{
	for (auto &iter: m_batch) {
		process_event((LogData*)iter.get());
	}
	m_batch.clear();
}

// --------------------------------
// CLogTimeFormatter implementation
// --------------------------------
#if defined(WIN32) || defined(WIN64)
#define LOCALTIME(datetime, timestam) localtime_s(&(datetime), &(timestamp))
#define S_IFDIR _S_IFDIR
#define S_IFREG _S_IFREG
#else
#define LOCALTIME(datetime, timestamp) localtime_r(&(timestamp), &(datetime))
#endif

const char *CTimeFormatter::format_time(LogEventTime t) {
	time_t timestamp = std::chrono::system_clock::to_time_t(t);
	tm datetime;
	LOCALTIME(datetime, timestamp);
	if (strftime(m_buf, MAX_LENGTH, "%T", &datetime) == 0) {
		m_buf[MAX_LENGTH - 1] = 0;
	}
	return m_buf;
}

const char *CMsTimeFormatter::format_time(LogEventTime t) {
	time_t timestamp = std::chrono::system_clock::to_time_t(t);
	tm datetime;
	LOCALTIME(datetime, timestamp);
	auto len = strftime(m_buf, MAX_LENGTH, "%T", &datetime);
	if (len == 0) {
		m_buf[MAX_LENGTH - 1] = 0;
		return m_buf;
	}
	auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(t.time_since_epoch());
	sprintf(m_buf + len, ".%03d", (int)(msec.count() % 1000));
	return m_buf;
}

const char *CDateTimeFormatter::format_time(LogEventTime t) {
	time_t timestamp = std::chrono::system_clock::to_time_t(t);
	tm datetime;
	LOCALTIME(datetime, timestamp);
	if (strftime(m_buf, MAX_LENGTH, "%F %T", &datetime) == 0) {
		m_buf[MAX_LENGTH - 1] = 0;
	}
	return m_buf;
}

// --------------------------------
// Standard logger implementation
// --------------------------------

void CLogStdout::process_event(const LogData *log) {
	const char* stime = nullptr;
	if (m_formatter) {
		stime = m_formatter->format_time(log->time);
	}
	if(log->source_file)
	{
		if(stime) printf("[%s] ", stime);
		if(log->channel) printf("[%s] ", log->channel);
		printf("%s(%d)\n", log->source_file, log->source_line);
	}
	if(stime) printf("[%s] ", stime);
	if(log->channel) printf("[%s] ", log->channel);
	log->writer[LOG_WRITER_STDOUT](log, nullptr);
	printf("\n");
}

// --------------------------------
// File system implementation
// --------------------------------

void CLogFileSystem::normpath(std::string &path) {
	std::replace(path.begin(), path.end(), char(reversed_seperator), char(seperator));
}

void CLogFileSystem::split(const std::string &path, std::string &dir, std::string &file_name) {
	size_t pos = path.rfind(seperator);
	if (pos != std::string::npos) {
		pos += 1;
		dir = path.substr(0, pos);
		file_name = path.substr(pos);
	} else {
		dir = "";
		file_name = path;
	}
}

void CLogFileSystem::split_ext(const std::string &file_name, std::string &base_name, std::string &ext) {
	size_t pos = file_name.rfind('.');
	if (pos != std::string::npos) {
		base_name = file_name.substr(0, pos);
		ext = file_name.substr(pos);
	} else {
		base_name = file_name;
		ext = "";
	}
}

bool CLogFileSystem::isdir(const std::string &path) {
	struct stat path_st;
	return stat(path.c_str(), &path_st) == 0 && path_st.st_mode & S_IFDIR;
}

bool CLogFileSystem::isfile(const std::string &path) {
	struct stat path_st;
	return stat(path.c_str(), &path_st) == 0 && path_st.st_mode & S_IFREG;
}

bool CLogFileSystem::makedirs(const std::string &path) {
	std::string cmd = "mkdir ";
	cmd += path;
	return std::system(cmd.c_str()) == 0;
}

// --------------------------------
// File logger implementation
// --------------------------------

FILE *CLogFile::_share_open(const char *path, const char *mode) {
#if defined(_WIN32) || defined(_WIN64)
	return _fsopen(path, mode, _SH_DENYWR);
#else
	return fopen(path, mode);
#endif
}

CLogFile::CLogFile(const char *filepath, bool append, int rotate_count, size_t rotate_size)
	: m_hfile(nullptr)
	, m_curfile(filepath)
	, m_cur_size(0)
	, m_rotate_size(rotate_size)
	, m_rotate_count(rotate_count)
{
	CLogFileSystem::normpath(m_curfile);
	CLogFileSystem::split(m_curfile, m_logpath, m_logname);
	if (!CLogFileSystem::isdir(m_logpath) && !CLogFileSystem::makedirs(m_logpath)) {
		printf("Fail to create log director [%s]\n", m_logpath.c_str());
		m_logpath = "";
	}
	if (append)
		m_hfile = _share_open(m_curfile.c_str(), "a");
	else if (CLogFileSystem::isfile(m_curfile))
		rotate();
	else
		m_hfile = _share_open(m_curfile.c_str(), "w");
}

CLogFile::~CLogFile() {
	if (m_hfile) {
		fclose(m_hfile);
		m_hfile = 0;
	}
}
	
void CLogFile::flush()
{
	CLogHandler::flush();
	fflush(m_hfile);
}

void CLogFile::process_event(const LogData *log) {
	const char* stime = nullptr;
	if (m_formatter) {
		stime = m_formatter->format_time(log->time);
	}
	if(log->source_file)
	{
		if(stime) m_cur_size += fprintf(m_hfile, "[%s] ", stime);
		if(log->channel) m_cur_size += fprintf(m_hfile, "[%s] ", log->channel);
		m_cur_size += fprintf(m_hfile, "%s(%d)\n", log->source_file, log->source_line);
	}
	if(stime) m_cur_size += fprintf(m_hfile, "[%s] ", stime);
	if(log->channel) m_cur_size += fprintf(m_hfile, "[%s] ", log->channel);
	std::pair<FILE*, long> context{m_hfile, 0};
	log->writer[LOG_WRITER_FILE](log, &context);
	m_cur_size += context.second + 1;
	fprintf(m_hfile, "\n");
	if (m_cur_size >= m_rotate_size) {
		rotate();
	}
}

void CLogFile::on_close() {
	if (m_hfile) {
		fclose(m_hfile);
		m_hfile = nullptr;
	}
}

void CLogFile::rotate() {
	if (m_rotate_count < 1)
		return;
	if (m_hfile) {
		fclose(m_hfile);
		m_hfile = nullptr;
	}
	std::string logfile, ext;
	CLogFileSystem::split_ext(m_curfile, logfile, ext);
	std::string last_file = logfile, cur_file;
	last_file += std::to_string(m_rotate_count);
	last_file += ext;
	if (CLogFileSystem::isfile(last_file)) {
		if (std::remove(last_file.c_str()) != 0)
			return;
	}
	for (int i = m_rotate_count - 1; i >= 0; --i) {
		cur_file = logfile;
		cur_file += std::to_string(i);
		cur_file += ext;
		if (CLogFileSystem::isfile(cur_file))
			std::rename(cur_file.c_str(), last_file.c_str());
		last_file = cur_file;
	}
	std::rename(m_curfile.c_str(), last_file.c_str());
	m_hfile = _share_open(m_curfile.c_str(), "w");
}

// --------------------------------
// String logger implementation
// --------------------------------

CCustomLog::CCustomLog()
	: m_str(nullptr, 0)
{
	
}
	
void CCustomLog::process_event(const LogData *log)
{
	size_t capacity;
	m_str = getstr(1);
	if(!m_str.first) {
		goto ERROR_EXIT;
	}
	capacity = m_str.second;
	log->writer[LOG_WRITER_STRING](log, &m_str);
	if(m_str.second < 0) {
		goto ERROR_EXIT;
	}
	if(m_str.second >= capacity) {
		// not enough size, try resize then write again
		m_str = getstr(m_str.second + 1);
		if(!m_str.first) {
			goto ERROR_EXIT;
		}
		capacity = m_str.second;
		log->writer[LOG_WRITER_STRING](log, &m_str);
		if(m_str.second < 0) {
			goto ERROR_EXIT;
		}
		// if still not enough, strip the string
		if(m_str.second >= capacity) {
			m_str.second = capacity - 1;
			m_str.first[m_str.second] = 0;
		}
	}
	// done with string buffer
	setstr(m_str.second + 1);
	return;
	
ERROR_EXIT:
	// encounter error
	handle_error(log);
	return;
}

void CCustomLog::handle_error(const LogData *log)
{
	printf("Fail to write log.\n");
}


CLogString::CLogString(size_t init_size)
	: CCustomLog()
	, m_buf(nullptr)
	, m_capacity(init_size)
{
	if(init_size < 1)
		init_size = 1;
	m_buf = new char[init_size];
	m_buf[0] = 0;
}

CLogString::~CLogString()
{
	if(m_buf) {
		delete [] m_buf;
		m_buf = nullptr;
	}
}

std::pair<char*, size_t> CLogString::getstr(size_t required_size)
{
	if(required_size > m_capacity) {
		size_t size = m_capacity;
		while(size < required_size) {
			size *= 2;
		}
		if(size >= LOG_STRING_SIZE_MAX)
			size = LOG_STRING_SIZE_MAX;
		if(!m_buf)
			delete [] m_buf;
		m_buf = new char[size];
		m_capacity = size;
	}
	return {m_buf, m_capacity};
}

#ifdef USE_NAMESPACE
}
#endif
#endif // STB_LOG_IMPLEMENTATION
