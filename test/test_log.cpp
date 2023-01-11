#include "log.h"
#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <ctime>
#include <gtest/gtest.h>
#include <random>

#ifdef USE_NAMESPACE
using namespace STB_LOG_NAMESPACE;
#endif

class CLogWriter : public CLogHandler {
  public:
    CLogWriter(size_t max_size, std::atomic<unsigned> *counter) : m_counter(counter) { m_logs.reserve(max_size); }

    virtual void process_event(const LogData *log) override {
        unsigned index = *reinterpret_cast<const unsigned *>((const char *)log + sizeof(LogData));
        m_logs.push_back(index);
        m_counter->fetch_sub(1, std::memory_order_relaxed);
    }

    inline std::vector<unsigned> &get_logs() { return m_logs; }

  private:
    std::vector<unsigned> m_logs;
    std::atomic<unsigned> *m_counter;
};

void thread_test() {
    printf("stb_log thread test start...\n");
    printf("core: %d\n", std::thread::hardware_concurrency());

    constexpr int channel_count = 2;
    const StbLogLevel channel_level[channel_count] = {
        LOG_DEBUG,
        LOG_INFO,
    };
    const char *channel_name[channel_count] = {"DEBUG", "INFO"};

    constexpr int max_write_count = 100000;

    alignas(CACHELINE_SIZE) std::atomic<unsigned> ch1_counter(1);
    alignas(CACHELINE_SIZE) std::atomic<unsigned> ch2_counter(1);
    alignas(CACHELINE_SIZE) std::atomic<unsigned> max_read_count(max_write_count * 2);

    std::atomic<unsigned> *channel_counter[channel_count] = {&ch1_counter, &ch2_counter};

    CLogWriter *h1 = new CLogWriter(max_write_count, &max_read_count);
    h1->set_filter([](const LogData *log) -> bool { return log->level == LOG_DEBUG; });
    CLogWriter *h2 = new CLogWriter(max_write_count, &max_read_count);
    h2->set_filter([](const LogData *log) -> bool { return log->level == LOG_INFO; });
    CLogger *logger = new CLogger(256);
    logger->add_handler(h1);
    logger->add_handler(h2);

    std::thread th1([&] {
        while (max_read_count > 0) {
            h1->process();
            std::this_thread::yield();
        }
    });

    std::thread th2([&] {
        while (max_read_count > 0) {
            h2->process();
            std::this_thread::yield();
        }
    });

    std::thread th3([&] {
        std::mt19937 random((unsigned)time(nullptr));
        for (int cnt = 0; cnt < max_write_count; ++cnt) {
            auto i = random() % channel_count;
            auto c = channel_counter[i]->fetch_add(1, std::memory_order_relaxed);
            logger->write(channel_level[i], channel_name[i], c);
        }
    });

    std::thread th4([&] {
        std::mt19937 random((unsigned)time(nullptr));
        for (int cnt = 0; cnt < max_write_count; ++cnt) {
            auto i = random() % channel_count;
            auto c = channel_counter[i]->fetch_add(1, std::memory_order_relaxed);
            logger->write(channel_level[i], channel_name[i], c);
        }
    });

    printf("please wait...\n");
    th1.join();
    th2.join();
    th3.join();
    th4.join();

    // validation
    printf("validate result...\n");
    assert(max_read_count == 0);
    auto &log1 = h1->get_logs();
    auto &log2 = h2->get_logs();
    printf("channel[1] = %d\n", unsigned(log1.size()));
    printf("channel[2] = %d\n", unsigned(log2.size()));
    assert(log1.size() + log2.size() == max_write_count * 2);
    assert(log1.size() == ch1_counter - 1);
    assert(log2.size() == ch2_counter - 1);
    std::sort(log1.begin(), log1.end());
    for (unsigned i = 0; i < log1.size(); ++i) {
        assert(i + 1 == log1[i]);
    }
    std::sort(log2.begin(), log2.end());
    for (unsigned i = 0; i < log2.size(); ++i) {
        assert(i + 1 == log2[i]);
    }

    // clear up
    delete h1;
    delete h2;
    delete logger;
    printf("success\n");
}

void common_test() {
    printf("stb_log common test start...\n");
    CLogger *logger = new CLogger(256);
    CLogHandler *handlers[] = {
        new CLogStdout(),
        new CLogFile("log/test.log"),
    };
    for (auto h : handlers)
        logger->add_handler(h);

    handlers[0]->set_time_formatter(std::make_unique<CMsTimeFormatter>());
    handlers[1]->set_time_formatter(std::make_unique<CDateTimeFormatter>());

    std::thread th1([&] {
        while (!handlers[0]->is_closed()) {
            handlers[0]->process();
            std::this_thread::yield();
        }
    });

    std::thread th2([&] {
        while (!handlers[1]->is_closed()) {
            handlers[1]->process();
            std::this_thread::yield();
        }
    });

    int i = 0;
    float f = 3.1415926f;
    const char *c = "const chars";
    std::string s = "std::string";

    logger->write(LOG_DEBUG, "DEBUG", __FILE__, __LINE__, "debug info: %d, %f, '%s', '%s', '%s'", i, f, c, s,
                  "string literal");
    logger->write(LOG_INFO, "INFO", __FILE__, __LINE__, "common message");
    logger->write(LOG_WARNING, "WARNING", __FILE__, __LINE__, "it's a warning");
    logger->write(LOG_ERROR, "ERROR", __FILE__, __LINE__, "it's an error");
    logger->write(LOG_CRITICAL, "CRITICAL", __FILE__, __LINE__, "fatal error!");

    // close and exit
    logger->close();
    th1.join();
    th2.join();

    // cleanup up
    for (auto h : handlers)
        delete h;
    delete logger;
    printf("success\n");
}

void file_rotate_test() {
    printf("stb_log file rotate test start...\n");
    CLogFile *h = new CLogFile("log/exception/test.log");
    assert(h->get_base_name() == "test.log");
#if defined(_WIN32) || defined(_WIN64)
    assert(h->get_directory() == "log\\exception\\");
#else
    assert(h->get_directory() == "log/exception/");
#endif
    h->rotate();
    h->rotate();
    h->rotate();
    h->rotate();
    h->rotate();

    delete h;
    printf("success\n");
}

void usage_test() {
    printf("stb_log usage test start...\n");
    start_logger();
    start_file_logger("log/test.log");

#ifdef LOG_SEVERITY_LEVEL
    constexpr int log_severity_level = LOG_SEVERITY_LEVEL;
#else
    constexpr int log_severity_level = 99;
#endif

    int i = 255;
    float f = 3.1415926f;
    const char *c = "const chars";
    std::string s = "std::string";

    log_info("simple string literal");
    log_info(std::string("simple string"));
    log_info(std::wstring(L"simple wide string"));

    log_write(LOG_INFO, "TEST", "current log sevirity level is [%d]", log_severity_level);
    log_debug("debug message: %d, %f, '%s', '%s'", i, f, c, s);
    log_info("info message: %d, %f, '%s', '%s'", i, f, c, s);
    log_warning("warning message: %d, %f, '%s', '%s'", i, f, c, s);
    log_error("error message: %d, %f, '%s', '%s'", i, f, c, s);
    log_critical("critical message: %d, %f, '%s', '%s'", i, f, c, s);

    close_logger();
    printf("success\n");
}

template <class T>
auto Copy(T &&v) {
    constexpr bool b = IsStringLiteral<decltype(v)>::value;
    typename CopyableTypeT<T, b>::type s = v;
    return s;
}

void type_trait_test() {
    int i = 0;
    int &iref = i;
    const int &iref2 = i;
    float f = 3.1415926f;
    float &fref = f;
    const float &fref2 = f;
    const char *c = "const chars";
    std::string s = "std::string";
    std::string &sref = s;
    const std::string &sref2 = s;
    int data[] = {1, 2, 3};
    const int *pdata = data;
    const wchar_t *wc = L"const wide chars";
    std::wstring ws = L"std::wstring";
    std::wstring &wsref = ws;
    const std::wstring &wsref2 = ws;

    auto b1 = Copy("string literal");
    auto b2 = Copy(s);
    auto b3 = Copy(c);
    auto b4 = Copy(sref);
    auto b5 = Copy(sref2);
    auto b6 = Copy(i);
    auto b7 = Copy(iref);
    auto b8 = Copy(f);
    auto b9 = Copy(fref);
    auto b10 = Copy(data);
    auto b11 = Copy(pdata);
    auto b12 = Copy(1);
    auto b13 = Copy(3.14f);
    auto b14 = Copy(std::string("rvalue"));
    auto b15 = Copy(iref2);
    auto b16 = Copy(fref2);
    auto b17 = Copy(wc);
    auto b18 = Copy(ws);
    auto b19 = Copy(wsref);
    auto b20 = Copy(wsref2);
    auto b21 = Copy(L"wide string literal");

    b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21;
}

TEST(MyAsyncLog, DISABLED_test_1) {
    thread_test();
    // file_rotate_test();
    common_test();
    usage_test();
    type_trait_test();
}