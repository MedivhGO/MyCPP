//#ifndef INCLUDE_STB_LOG_H
//#define INCLUDE_STB_LOG_H

#include <atomic>
#include <chrono>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#if defined(_WINDOWS) && defined(STB_SHARED_LIBS)
#ifdef stb_EXPORTS
#define STB_LOG_API __declspec(dllexport)
#else
#define STB_LOG_API __declspec(dllimport)
#endif
#else
#define STB_LOG_API
#endif

#ifdef USE_NAMESPACE
#ifndef STB_LOG_NAMESPACE
#define STB_LOG_NAMESPACE stb
#endif
namespace STB_LOG_NAMESPACE {
#endif

// --------------------------------
// library settings
// --------------------------------
enum StbLogLevel {
    LOG_CRITICAL = 50,
    LOG_ERROR = 40,
    LOG_WARNING = 30,
    LOG_INFO = 20,
    LOG_DEBUG = 10,
};

// log severity level
#ifndef LOG_SEVERITY_LEVEL
#define LOG_SEVERITY_LEVEL 0
#endif
// default log file rotate size (256 MB)
#define LOG_FILE_ROTATE_SIZE (256 * 1024 * 1024)
// define log file rotate count
// keep latest 8 log file
#define LOG_FILE_ROTATE_COUNT 8
// logger queue buffer size
// writer(producer) thread will block when the queue is full
// change the size according the maximum concurrency
#define LOG_QUEUE_SIZE 256
// logger worker thread sleep time when it's casual (in milliseconds)
#define LOG_WORKER_SLEEP_TIME 30
// logger worker batch size
#define LOG_BATCH_SIZE 64
// string logger default buffer size
#define LOG_STRING_SIZE 1024
#define LOG_STRING_SIZE_MAX (1024 * 1024)
// cache line size
#ifndef CACHELINE_SIZE
#define CACHELINE_SIZE 64
#endif

// stb_log namespace
#ifdef USE_NAMESPACE
#define STB_LOG_LEVEL STB_LOG_NAMESPACE::StbLogLevel
#define LOGGER() STB_LOG_NAMESPACE::get_logger()
#else
#define STB_LOG_LEVEL StbLogLevel
#define LOGGER() get_logger()
#endif

// write log
#define log_write(lvl, channel, fmt, ...) (LOGGER()->write(lvl, channel, nullptr, 0, (fmt), ##__VA_ARGS__))
#ifdef LOG_SEVERITY_LEVEL
// write critical log
#if LOG_SEVERITY_LEVEL <= 50
#define log_critical(fmt, ...)                                                                                         \
    (LOGGER()->write(STB_LOG_LEVEL::LOG_CRITICAL, "CRITICAL", __FILE__, __LINE__, (fmt), ##__VA_ARGS__))
#else
#define log_critical(fmt, ...)
#endif
// write error log
#if LOG_SEVERITY_LEVEL <= 40
#define log_error(fmt, ...)                                                                                            \
    (LOGGER()->write(STB_LOG_LEVEL::LOG_ERROR, "ERROR", __FILE__, __LINE__, (fmt), ##__VA_ARGS__))
#else
#define log_error(fmt, ...)
#endif
// write warning log
#if LOG_SEVERITY_LEVEL <= 30
#define log_warning(fmt, ...) (LOGGER()->write(STB_LOG_LEVEL::LOG_WARNING, "WARNING", nullptr, 0, (fmt), ##__VA_ARGS__))
#else
#define log_warning(fmt, ...)
#endif
// write info log
#if LOG_SEVERITY_LEVEL <= 20
#define log_info(fmt, ...) (LOGGER()->write(STB_LOG_LEVEL::LOG_INFO, "INFO", nullptr, 0, (fmt), ##__VA_ARGS__))
#else
#define log_info(fmt, ...)
#endif
// write debug log
#if LOG_SEVERITY_LEVEL <= 10
#define log_debug(fmt, ...) (LOGGER()->write(STB_LOG_LEVEL::LOG_DEBUG, "DEBUG", nullptr, 0, (fmt), ##__VA_ARGS__))
#else
#define log_debug(fmt, ...)
#endif
#else // skip log macro
#define log_critical(fmt, ...)
#define log_error(fmt, ...)
#define log_warning(fmt, ...)
#define log_info(fmt, ...)
#define log_debug(fmt, ...)
#endif // LOG_SEVERITY_LEVEL

// --------------------------------
// public user interface
// --------------------------------
class CLogger;
class CLogHandler;

struct LogContext {
    CLogger *logger = nullptr;
    std::vector<std::unique_ptr<std::thread>> thread_pool;
};

typedef std::chrono::milliseconds::rep millisecond_t;

// get global logger
STB_LOG_API CLogger *get_logger();

// close logger
STB_LOG_API void close_logger();

// start a logger thread
STB_LOG_API void start_handler_thread(CLogHandler *handler, millisecond_t sleep_time = LOG_WORKER_SLEEP_TIME);

// start logging to standard output
STB_LOG_API void start_logger(bool async = true, millisecond_t sleep_time = LOG_WORKER_SLEEP_TIME);

// start logging to file
STB_LOG_API void start_file_logger(const char *log_file_path, bool append_mode = false,
                                   int max_rotation = LOG_FILE_ROTATE_COUNT, size_t rotate_size = LOG_FILE_ROTATE_SIZE,
                                   bool async = true, millisecond_t sleep_time = LOG_WORKER_SLEEP_TIME);

// start logging to string buffer
STB_LOG_API void start_string_logger(size_t buffer_size = LOG_STRING_SIZE, bool async = true,
                                     millisecond_t sleep_time = LOG_WORKER_SLEEP_TIME);

// convert to data that can be copied between threads
// string literal has type "const char (&)[N]". It can be copied as pointer.

template <class T>
struct IsStringLiteral : std::false_type {};

template <size_t N>
struct IsStringLiteral<const char (&)[N]> : std::true_type {};

template <size_t N>
struct IsStringLiteral<const wchar_t (&)[N]> : std::true_type {};

template <class T>
struct CastStringType {
    typedef T type;
};

template <>
struct CastStringType<char *> {
    typedef std::string type;
};

template <>
struct CastStringType<const char *> {
    typedef std::string type;
};

template <>
struct CastStringType<wchar_t *> {
    typedef std::wstring type;
};

template <>
struct CastStringType<const wchar_t *> {
    typedef std::wstring type;
};

template <class T>
struct RemoveConstRef {
    typedef std::remove_const_t<std::remove_reference_t<std::decay_t<T>>> type;
};

template <class T, bool IsStringLiteral = IsStringLiteral<T>::value>
struct CopyableTypeT {
    typedef typename CastStringType<typename RemoveConstRef<T>::type>::type type;
};

template <class T>
struct CopyableTypeT<T, true> {
    typedef T type;
};

template <class T>
using CopyableType = typename CopyableTypeT<T>::type;

// convert any value to primitive types that can be recognized by printf
// add overload functions to customize type conversion
template <class T>
inline T to_printable(const T v) {
    return v;
}

inline const char *to_printable(const std::string &v) { return v.c_str(); }

inline const wchar_t *to_printable(const std::wstring &v) { return v.c_str(); }

inline uint64_t to_printable(std::thread::id tid) {
    // if-constexpr
    if constexpr (sizeof(std::thread::id) == sizeof(uint64_t))
        return *(uint64_t *)&tid;
    else {
        std::hash<std::thread::id> hasher;
        return hasher(tid);
    }
}

// --------------------------------
// END of interface declaration
// --------------------------------

struct alignas(CACHELINE_SIZE) Sequence {
    std::atomic<uint64_t> value;
    char _padding[CACHELINE_SIZE - sizeof(std::atomic<uint64_t>)];

    inline void set(uint64_t v) { value = v; }

    inline uint64_t get() const { return value; }

    inline uint64_t load() const { return value.load(std::memory_order_acquire); }

    inline void store(uint64_t v) { value.store(v, std::memory_order_release); }

    inline uint64_t fetch_add(uint64_t v) { return value.fetch_add(v, std::memory_order_relaxed); }
};

using LogEventTime = std::chrono::system_clock::time_point;

struct LogData;
typedef void (*LogWriter)(const LogData *log, void *context);

struct LogData {
    int level;
    int source_line;
    const char *source_file;
    LogEventTime time;
    const char *channel;
    const LogWriter *writer;
};

struct LogEvent {
    // 1 cacheline for shared publish flag
    Sequence publish;
    // 1 cacheline for data
    std::shared_ptr<void> data;
    char _padding[CACHELINE_SIZE - sizeof(std::shared_ptr<void>)];
};

template <class F, size_t... Is>
constexpr auto index_apply_impl(F f, std::index_sequence<Is...>) {
    return f(std::integral_constant<size_t, Is>{}...);
}

template <size_t N, class F>
constexpr auto index_apply(F f) {
    return index_apply_impl(f, std::make_index_sequence<N>{});
}

template <class Tuple, size_t... Is>
constexpr auto printf_tuple(const Tuple &t, std::index_sequence<Is...>) {
    printf(to_printable(std::get<Is>(t))...);
}

template <class Tuple, size_t... Is>
constexpr auto fprintf_tuple(std::pair<FILE *, long> *out, const Tuple &t, std::index_sequence<Is...>) {
    out->second = fprintf(out->first, to_printable(std::get<Is>(t))...);
}

template <class Tuple, size_t... Is>
constexpr auto sprintf_tuple(std::pair<char *, size_t> *out, const Tuple &t, std::index_sequence<Is...>) {
    out->second = snprintf(out->first, out->second, to_printable(std::get<Is>(t))...);
}

enum ELogWriterType {
    LOG_WRITER_STDOUT,
    LOG_WRITER_FILE,
    LOG_WRITER_STRING,

    LOG_WRITER_COUNT
};

template <class Tuple>
struct GenericLogData : LogData {
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-security"
#endif
    alignas(Tuple) char data[sizeof(Tuple)];

    ~GenericLogData() { ((Tuple *)data)->~Tuple(); }

    static void write_stdout(const LogData *entry, void *context) {
        const GenericLogData *self = (const GenericLogData *)entry;
        constexpr size_t N = std::tuple_size_v<Tuple>;
        auto t = reinterpret_cast<const Tuple *>(self->data);
        printf_tuple(*t, std::make_index_sequence<N>{});
    }

    static void write_file(const LogData *entry, void *context) {
        const GenericLogData *self = (const GenericLogData *)entry;
        constexpr size_t N = std::tuple_size_v<Tuple>;
        auto t = reinterpret_cast<const Tuple *>(self->data);
        auto c = (std::pair<FILE *, long> *)context;
        fprintf_tuple(c, *t, std::make_index_sequence<N>{});
    }

    static void write_string(const LogData *entry, void *context) {
        const GenericLogData *self = (const GenericLogData *)entry;
        constexpr size_t N = std::tuple_size_v<Tuple>;
        auto t = reinterpret_cast<const Tuple *>(self->data);
        auto c = (std::pair<char *, size_t> *)context;
        sprintf_tuple(c, *t, std::make_index_sequence<N>{});
    }

#ifdef __clang__
#pragma clang diagnostic pop
#endif

    static const LogWriter *get_writer() {
        static constexpr LogWriter s_writer_table[LOG_WRITER_COUNT] = {
            &write_stdout,
            &write_file,
            &write_string,
        };
        return s_writer_table;
    }
};

typedef bool (*LogFilter)(const LogData *);

class CLogger;

class CLogTimeFormatter {
  public:
    virtual ~CLogTimeFormatter() = default;
    virtual const char *format_time(LogEventTime t) = 0;
};

// time string format: "HH:MM:SS", 8 char
class CTimeFormatter : public CLogTimeFormatter {
  public:
    CTimeFormatter() { m_buf[0] = 0; }
    const char *format_time(LogEventTime t) override;

  private:
    static constexpr unsigned MAX_LENGTH = 9;
    char m_buf[MAX_LENGTH];
};

// time string format: "HH:MM:SS.xxx", 12 char
class CMsTimeFormatter : public CLogTimeFormatter {
  public:
    CMsTimeFormatter() { m_buf[0] = 0; }
    const char *format_time(LogEventTime t) override;

  private:
    static constexpr unsigned MAX_LENGTH = 13;
    char m_buf[MAX_LENGTH];
};

// time string format: "YYYY-MM-DD HH:MM:SS", 19 char
class CDateTimeFormatter : public CLogTimeFormatter {
  public:
    CDateTimeFormatter() { m_buf[0] = 0; }
    const char *format_time(LogEventTime t) override;

  private:
    static constexpr unsigned MAX_LENGTH = 20;
    char m_buf[MAX_LENGTH];
};

class CLogHandler {
  public:
    static void *operator new(size_t size);
    static void operator delete(void *ptr);

    CLogHandler();
    virtual ~CLogHandler();
    CLogHandler(const CLogHandler &) = delete;
    CLogHandler(CLogHandler &&) = delete;
    CLogHandler &operator=(const CLogHandler &) = delete;
    CLogHandler &operator=(CLogHandler &&) = delete;

    void process();
    virtual void flush();
    virtual void process_event(const LogData *data) {}
    virtual void on_close() {}

    inline void follow(CLogger *seq) { m_logger = seq; }

    inline uint64_t get_sequence() const { return m_seq.get(); }

    inline uint64_t acquire_sequence() const { return m_seq.load(); }

    inline void set_filter(LogFilter filter) { m_filter = filter; }

    inline bool is_closed() const { return m_closed; }

    inline void set_time_formatter(std::unique_ptr<CLogTimeFormatter> &&ptr) { m_formatter = std::move(ptr); }

  protected:
    CLogger *m_logger;
    LogFilter m_filter;
    std::unique_ptr<CLogTimeFormatter> m_formatter;
    std::vector<std::shared_ptr<void>> m_batch;
    bool m_closed;
    Sequence m_seq;
};

class CLogStdout : public CLogHandler {
  public:
    void process_event(const LogData *log) override;
};

// platform dependence filesystem api
// It should be replaced by std::filesystem (C++17) if possible
class CLogFileSystem {
  public:
#if defined(_WIN32) || defined(_WIN64)
    static constexpr char seperator = '\\';
    static constexpr char reversed_seperator = '/';
#else
    static constexpr char seperator = '/';
    static constexpr char reversed_seperator = '\\';
#endif

    static void normpath(std::string &path);

    static void split(const std::string &path, std::string &dir, std::string &file_name);

    static void split_ext(const std::string &file_name, std::string &base_name, std::string &ext);

    static bool isdir(const std::string &path);

    static bool isfile(const std::string &path);

    static bool makedirs(const std::string &path);
};

class CLogFile : public CLogHandler {
  public:
    CLogFile(const char *filepath, bool append = false, int rotate_count = LOG_FILE_ROTATE_COUNT,
             size_t rotate_size = LOG_FILE_ROTATE_SIZE);
    ~CLogFile() override;
    void flush() override;
    void process_event(const LogData *log) override;
    void on_close() override;
    void rotate();

    bool is_ready() const { return m_hfile != nullptr; }
    const std::string &get_directory() const { return m_logpath; }
    const std::string &get_base_name() const { return m_logname; }
    const std::string &get_file_path() const { return m_curfile; }

  private:
    static FILE *_share_open(const char *path, const char *mode);

    FILE *m_hfile;
    std::string m_logpath;
    std::string m_logname;
    std::string m_curfile;
    size_t m_cur_size;
    size_t m_rotate_size;
    int m_rotate_count;
};

class CCustomLog : public CLogHandler {
  public:
    CCustomLog();
    void process_event(const LogData *log) override;
    // get C string
    const char *str() const { return m_str.first; }
    // get string size
    size_t size() const { return m_str.second; }

  protected:
    virtual std::pair<char *, size_t> getstr(size_t required_size) = 0;

    virtual void setstr(size_t size) {}

    virtual void handle_error(const LogData *log);

    std::pair<char *, size_t> m_str;
};

class CLogString : public CCustomLog {
  public:
    CLogString(size_t init_size = LOG_STRING_SIZE);
    ~CLogString() override;

  protected:
    std::pair<char *, size_t> getstr(size_t required_size) override;

  private:
    char *m_buf;
    size_t m_capacity;
};

class STB_LOG_API CLogger {
  public:
    CLogger(size_t buf_size);
    ~CLogger();
    CLogger(const CLogger &) = delete;
    CLogger(CLogger &&) = delete;
    CLogger &operator=(const CLogger &) = delete;
    CLogger &operator=(CLogger &&) = delete;
    // notify all handlers to close
    void close();
    void add_handler(CLogHandler *handler);
    void remove_handler(CLogHandler *handler);
    // release all handlers
    // assume self own the handlers, and handlers are allocated by new operator
    void release_handlers();
    // send log message to handlers
    template <class... Args>
    void write(int level, const char *channel, const char *source_file, int source_line, const char *format,
               Args &&...args) {
        using tuple_t = std::tuple<const char *, CopyableType<decltype(args)>...>;
        using entry_t = GenericLogData<tuple_t>;
        auto sptr = std::make_shared<entry_t>();
        new (sptr->data) tuple_t{format, std::forward<Args>(args)...};
        sptr->level = level;
        sptr->channel = channel;
        sptr->source_file = source_file;
        sptr->source_line = source_line;
        sptr->time = std::chrono::system_clock::now();
        sptr->writer = entry_t::get_writer();
        publish(sptr);
    }

    // send any data to handlers
    template <class T>
    void write(int level, const char *channel, const T &obj) {
        struct entry_t : LogData {
            T data;
        };
        auto sptr = std::make_shared<entry_t>();
        sptr->data = obj;
        sptr->level = level;
        sptr->channel = channel;
        sptr->source_file = nullptr;
        sptr->source_line = 0;
        sptr->time = std::chrono::system_clock::now();
        sptr->writer = nullptr;
        publish(sptr);
    }

    void write(int level, const char *channel, const char *source_file, int source_line, const std::string &obj) {
        write(level, channel, source_file, source_line, "%s", obj);
    }

    void write(int level, const char *channel, const char *source_file, int source_line, const std::wstring &obj) {
        write(level, channel, source_file, source_line, "%ws", obj);
    }

    const LogEvent *get_event(uint64_t seq) const { return m_event_queue + (seq & m_size_mask); }

    LogEvent *get_event(uint64_t seq) { return m_event_queue + (seq & m_size_mask); }

    void *operator new(size_t size);
    void operator delete(void *ptr);

  private:
    uint64_t claim(uint64_t count);
    void publish(std::shared_ptr<void> &&sptr);

    LogEvent *m_event_queue;
    size_t m_size_mask;
    std::vector<CLogHandler *> m_handler_list;
    uint64_t m_min_seq;
    Sequence m_seq_claim;
};

#ifdef USE_NAMESPACE
}
#endif
//#endif // INCLUDE_STB_LOG_H
