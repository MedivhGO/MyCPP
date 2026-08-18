//
// Created by Lee on 2022/9/3.
//

#include <gtest/gtest.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>
#include <type_traits>
#include <vector>

#include "MyProjectPath.h"
#include "container/MyCache.h"
#include "container/MyString.h"
#include "thread/MyThread.h"
#include "util/MyError.h"
#include "util/MyFileReader.h"
#include "util/MyFileWriter.h"
#include "util/MyJsonParser.h"
#include "util/MyLog.h"
#include "util/MyProfile.h"
#include "util/MyRandom.h"
#include "util/MySingleton.h"

using std::cout;
using std::endl;

// 单例测试辅助类型: 记录构造次数, 支持默认构造(带虚析构以便用于 Singleton)
struct SingletonCounted {
  static int constructions;
  int value;

  explicit SingletonCounted(int v = 0) : value(v) { ++constructions; }

  virtual ~SingletonCounted() = default;
};
int SingletonCounted::constructions = 0;

// 无默认构造的类型, 用于验证 OnceSingleWithArgs 的参数转发
struct SingletonPoint {
  int x;
  int y;
  std::string name;

  SingletonPoint(int x_, int y_, const std::string &name_) : x(x_), y(y_), name(name_) {}
};

// 退出清理验证类型: 析构时向 stderr 打印标记 (死亡测试子进程的 stderr 可被捕获)
struct ExitCleanupType {
  static int destroyed;

  ~ExitCleanupType() {
    ++destroyed;
    std::cerr << "SINGLETON_CLEANED\n";
  }
};
int ExitCleanupType::destroyed = 0;

struct ExitCleanupArg {
  static int destroyed;
  int value;

  explicit ExitCleanupArg(int v) : value(v) {}

  ~ExitCleanupArg() {
    ++destroyed;
    std::cerr << "SINGLETON_CLEANED\n";
  }
};
int ExitCleanupArg::destroyed = 0;

TEST(MyUtil, test_random) {
  RandomNumberGenerator rg(10, 2000);
  EXPECT_GT(rg.GetRandomNumberByMt19937(), 10);
  EXPECT_LT(rg.GetRandomNumberByDefault(), 2000);
}

TEST(MyUtil, DISABLED_test_random_profile) {
  RandomNumberGenerator rg(-1000, 1000);
  auto default_generate = [&]() {
    for (int i = 0; i < 100000; ++i) {
      rg.GetRandomNumberByDefault();
    }
  };

  auto mt19937_generate = [&]() {
    for (int i = 0; i < 100000; ++i) {
      rg.GetRandomNumberByMt19937();
    }
  };

  EXPECT_LT(measure(mt19937_generate).count(), measure(default_generate).count());
}

TEST(MyUtil, test_singleton) {
  MyString &a = Singleton<MyString>::getInstance();
  MyString &b = Singleton<MyString>::getInstance();
  EXPECT_EQ(&a, &b);

  std::shared_ptr<MyString> p = SingleDemo<MyString>::getInstance();
  std::shared_ptr<MyString> pp = SingleDemo<MyString>::getInstance();
  EXPECT_EQ(p, pp);

  int *raw_ptr = OnceSingle<int>::getInstance();
  int *same_raw_ptr = OnceSingle<int>::getInstance();
  EXPECT_EQ(raw_ptr, same_raw_ptr);
  raw_ptr = nullptr;
  same_raw_ptr = nullptr;

  std::string *para_ptr = OnceSingleWithArgs<std::string>::getInstance("123");
  std::string *same_para_ptr = OnceSingleWithArgs<std::string>::getInstance("456");
  EXPECT_EQ(para_ptr, same_para_ptr);
  EXPECT_EQ(*para_ptr, *same_para_ptr);
  EXPECT_EQ(*para_ptr, "123");
  EXPECT_NE(*same_para_ptr, "456");
  para_ptr = nullptr;
  same_para_ptr = nullptr;

  std::string *para_ptr_atomic = SingletonAtom<std::string>::getInstance();
  std::string *same_para_ptr_atomic = SingletonAtom<std::string>::getInstance();
  EXPECT_EQ(para_ptr_atomic, same_para_ptr_atomic);
}

TEST(MyUtil, test_singleton_copy_disabled) {
  // 编译期验证: 五个单例均不可拷贝、不可从外部默认构造
  static_assert(!std::is_copy_constructible_v<SingleDemo<int>>);
  static_assert(!std::is_copy_assignable_v<SingleDemo<int>>);
  static_assert(!std::is_default_constructible_v<SingleDemo<int>>);

  static_assert(!std::is_copy_constructible_v<Singleton<MyString>>);
  static_assert(!std::is_copy_assignable_v<Singleton<MyString>>);
  static_assert(!std::is_default_constructible_v<Singleton<MyString>>);

  static_assert(!std::is_copy_constructible_v<Singleton<int>>);
  static_assert(!std::is_copy_assignable_v<Singleton<int>>);
  static_assert(!std::is_default_constructible_v<Singleton<int>>);

  static_assert(!std::is_copy_constructible_v<OnceSingle<int>>);
  static_assert(!std::is_copy_assignable_v<OnceSingle<int>>);
  static_assert(!std::is_default_constructible_v<OnceSingle<int>>);

  static_assert(!std::is_copy_constructible_v<OnceSingleWithArgs<int>>);
  static_assert(!std::is_copy_assignable_v<OnceSingleWithArgs<int>>);
  static_assert(!std::is_default_constructible_v<OnceSingleWithArgs<int>>);

  static_assert(!std::is_copy_constructible_v<SingletonAtom<int>>);
  static_assert(!std::is_copy_assignable_v<SingletonAtom<int>>);
  static_assert(!std::is_default_constructible_v<SingletonAtom<int>>);
}

TEST(MyUtil, test_singleton_initialized_once) {
  // 无论此前是否已构造, 多次调用只新增一次构造
  const int before = SingletonCounted::constructions;
  SingletonCounted *first = OnceSingle<SingletonCounted>::getInstance();
  EXPECT_EQ(SingletonCounted::constructions, before + 1);
  EXPECT_EQ(first->value, 0);

  SingletonCounted *second = OnceSingle<SingletonCounted>::getInstance();
  EXPECT_EQ(second, first);
  EXPECT_EQ(SingletonCounted::constructions, before + 1);
}

TEST(MyUtil, test_singleton_with_args) {
  // 多参数 + 不同类型参数转发 (修复前 getInstance(3, 4, "origin") 无法编译)
  SingletonPoint *p = OnceSingleWithArgs<SingletonPoint>::getInstance(3, 4, "origin");
  EXPECT_EQ(p->x, 3);
  EXPECT_EQ(p->y, 4);
  EXPECT_EQ(p->name, "origin");

  SingletonPoint *q = OnceSingleWithArgs<SingletonPoint>::getInstance(9, 9, "other");
  EXPECT_EQ(q, p);
  EXPECT_EQ(q->x, 3);  // 首次调用参数生效
  EXPECT_EQ(q->name, "origin");
}

TEST(MyUtil, test_singleton_state_persists) {
  // 同一实例共享状态: 修改后再次获取能看到修改
  SingleDemo<SingletonCounted>::getInstance()->value = 11;
  EXPECT_EQ(SingleDemo<SingletonCounted>::getInstance()->value, 11);

  Singleton<SingletonCounted>::getInstance().value = 22;
  EXPECT_EQ(Singleton<SingletonCounted>::getInstance().value, 22);

  OnceSingle<SingletonCounted>::getInstance()->value = 33;
  EXPECT_EQ(OnceSingle<SingletonCounted>::getInstance()->value, 33);

  SingletonAtom<SingletonCounted>::getInstance()->value = 44;
  EXPECT_EQ(SingletonAtom<SingletonCounted>::getInstance()->value, 44);
}

TEST(MyUtil, test_singleton_cleanup_at_exit) {
  // 死亡测试: 子进程创建三个裸指针单例后正常退出, 验证 CGFunctionClass 守卫
  // 在退出时确实 delete 了实例 (析构打印标记) 且清理过程不崩溃。
  ExitCleanupType::destroyed = 0;
  ExitCleanupArg::destroyed = 0;
  EXPECT_EXIT(
      {
        OnceSingle<ExitCleanupType>::getInstance();
        OnceSingleWithArgs<ExitCleanupArg>::getInstance(42);
        SingletonAtom<ExitCleanupType>::getInstance();
        std::exit(0);
      },
      ::testing::ExitedWithCode(0), "SINGLETON_CLEANED");
}

TEST(MyUtil, test_singleton_value) {
  // implement2: 移除私有继承后, 简单类型也可用
  EXPECT_EQ(Singleton<int>::getInstance(), 0);
  Singleton<int>::getInstance() = 123;
  EXPECT_EQ(Singleton<int>::getInstance(), 123);

  // implement1: 默认构造的值初始化
  EXPECT_EQ(*SingleDemo<int>::getInstance(), 0);

  // implement3: 默认构造的值初始化
  EXPECT_EQ(*OnceSingle<int>::getInstance(), 0);

  // implement5: 默认构造的值初始化
  EXPECT_EQ(*SingletonAtom<int>::getInstance(), 0);

  // implement4: 首次调用传入的参数生效，之后调用不再改变实例
  // （使用 vector<int> 避免与 test_singleton 中已初始化的 string 实例互相干扰）
  std::vector<int> *para_ptr = OnceSingleWithArgs<std::vector<int>>::getInstance(3, 7);
  EXPECT_EQ(para_ptr->size(), 3u);
  EXPECT_EQ((*para_ptr)[0], 7);
  std::vector<int> *same_para_ptr = OnceSingleWithArgs<std::vector<int>>::getInstance(2, 9);
  EXPECT_EQ(para_ptr, same_para_ptr);
  EXPECT_EQ(same_para_ptr->size(), 3u);
  EXPECT_EQ((*same_para_ptr)[0], 7);
}

TEST(MyUtil, test_singleton_thread_safety) {
  constexpr int kThreads = 16;
  constexpr int kIterations = 100;

  // implement1: SingleDemo（shared_ptr + 双重检查锁）
  {
    std::vector<std::shared_ptr<MyString>> results(kThreads);
    std::vector<std::thread> threads;
    for (int i = 0; i < kThreads; ++i) {
      threads.emplace_back([&results, i]() {
        for (int j = 0; j < kIterations; ++j) {
          results[i] = SingleDemo<MyString>::getInstance();
        }
      });
    }
    for (auto &t : threads) {
      t.join();
    }
    for (int i = 1; i < kThreads; ++i) {
      EXPECT_EQ(results[0], results[i]);
    }
  }

  // implement2: Singleton（Meyers 魔法静态）
  {
    std::vector<MyString *> results(kThreads);
    std::vector<std::thread> threads;
    for (int i = 0; i < kThreads; ++i) {
      threads.emplace_back([&results, i]() {
        for (int j = 0; j < kIterations; ++j) {
          results[i] = &Singleton<MyString>::getInstance();
        }
      });
    }
    for (auto &t : threads) {
      t.join();
    }
    for (int i = 1; i < kThreads; ++i) {
      EXPECT_EQ(results[0], results[i]);
    }
  }

  // implement3: OnceSingle（call_once + 原始指针）
  {
    std::vector<std::string *> results(kThreads);
    std::vector<std::thread> threads;
    for (int i = 0; i < kThreads; ++i) {
      threads.emplace_back([&results, i]() {
        for (int j = 0; j < kIterations; ++j) {
          results[i] = OnceSingle<std::string>::getInstance();
        }
      });
    }
    for (auto &t : threads) {
      t.join();
    }
    for (int i = 1; i < kThreads; ++i) {
      EXPECT_EQ(results[0], results[i]);
    }
  }

  // implement4: OnceSingleWithArgs（call_once + 带参构造）
  {
    std::vector<std::string *> results(kThreads);
    std::vector<std::thread> threads;
    for (int i = 0; i < kThreads; ++i) {
      threads.emplace_back([&results, i]() {
        for (int j = 0; j < kIterations; ++j) {
          results[i] = OnceSingleWithArgs<std::string>::getInstance("shared");
        }
      });
    }
    for (auto &t : threads) {
      t.join();
    }
    for (int i = 1; i < kThreads; ++i) {
      EXPECT_EQ(results[0], results[i]);
    }
  }

  // implement5: SingletonAtom（原子指针 + 双重检查锁）
  {
    std::vector<std::string *> results(kThreads);
    std::vector<std::thread> threads;
    for (int i = 0; i < kThreads; ++i) {
      threads.emplace_back([&results, i]() {
        for (int j = 0; j < kIterations; ++j) {
          results[i] = SingletonAtom<std::string>::getInstance();
        }
      });
    }
    for (auto &t : threads) {
      t.join();
    }
    for (int i = 1; i < kThreads; ++i) {
      EXPECT_EQ(results[0], results[i]);
    }
  }
}

TEST(MyUtil, DISABLED_test_thread) {
  std::shared_ptr<MyString> str_p = SingleDemo<MyString>::getInstance();
  auto test_call = [](const std::shared_ptr<MyString> &str_p, uint64_t thread_itr = 0) {
    std::shared_ptr<MyString> t = SingleDemo<MyString>::getInstance();
    EXPECT_EQ(str_p, t);
  };
  LaunchParallelTest(100, test_call, str_p);
}

TEST(MyUtil, DISABLED_test_mutex) {
  MutexLock m;
  const int atomic_str_length = 100;
  const int test_cycle = 100;
  int failure_time_without_lock = 0;
  int failure_time_with_lock = 0;
  for (int tc = 0; tc < test_cycle; ++tc) {
    std::string expect_str;
    for (int i = 0; i < atomic_str_length; ++i) {
      expect_str += "0123456789";
    }

    std::string res_str_without_lock;
    auto multi_add_without_lock = [&](uint64_t thread_itr = 0) {
      for (int i = 0; i < 10; ++i) {
        res_str_without_lock += std::to_string(i);
      }
    };

    std::string res_str_with_lock;
    auto multi_add_with_lock = [&](uint64_t thread_itr = 0) {
      m.Lock();
      for (int i = 0; i < 10; ++i) {
        res_str_with_lock += std::to_string(i);
      }
      m.Unlock();
    };

    LaunchParallelTest(atomic_str_length, multi_add_without_lock);
    LaunchParallelTest(atomic_str_length, multi_add_with_lock);

    if (expect_str != res_str_without_lock) {
      failure_time_without_lock++;
    }

    if (expect_str != res_str_with_lock) {
      failure_time_with_lock++;
    }
  }

  EXPECT_GT(failure_time_without_lock, 0);
  EXPECT_EQ(failure_time_with_lock, 0);
}

TEST(MyUtil, test_error) {
  try {
    throw UnixError();
  } catch (const UnixError &e) {
    EXPECT_STRNE(e.what(), "");
  }

  try {
    throw MyLogicalError("len must > 0");
  } catch (const MyLogicalError &e) {
    EXPECT_STREQ("MyLogicalError : len must > 0", e.what());
  }

  EXPECT_THROW(throw UnixError(), std::exception);
}

TEST(MyUtil, DISABLED_test_log) {
  EnableLogging();
  LOG_DEBUG("DEBUG MESSAGE");
  LOG_ERROR("ERROR MESSAGE");
  LOG_INFO("INFO MESSAGE");
  LOG_WARN("WARN MESSAGE");
  DisableLogging();
}

TEST(MyUtil, test_cache) {
  Cache<int> int_cache;
  auto fill_data = [&](uint64_t thread_itr = 0) {
    std::shared_ptr<const int> id_ptr = int_cache.FastLoadT(std::this_thread::get_id());
    LOG_DEBUG(std::to_string(*id_ptr).c_str());
  };
  LaunchParallelTest(20, fill_data);
  EXPECT_EQ(int_cache.GetCacheSize(), 20);
}

TEST(MyUtil, test_file_wandr) {
  FileWriter fw;
  FileReader fr;

  std::string project_root = PROJECT_PATH;
  std::vector<std::string> input_vec{"line1", "line1", "line1"};
  EXPECT_TRUE(fw.Open(project_root + "/resource/filetestcase"));
  EXPECT_TRUE(fw.WriteVector(input_vec, "\n"));
  fw.Flush();
  EXPECT_TRUE(fw.WriteVector(input_vec));
  fw.Flush();

  EXPECT_TRUE(fr.Open(project_root + "/resource/filetestcase"));
  std::string read_one_line;
  std::vector<std::string> content_vec;
  while (read_one_line = fr.ReadLine(), !read_one_line.empty()) {
    EXPECT_EQ(read_one_line, "line1");
    content_vec.push_back(read_one_line);
  }
  EXPECT_EQ(content_vec.size(), 6);
  fw.Close();
  fr.Close();

  EXPECT_TRUE(fw.Open(project_root + "/resource/filetestcase"));
  EXPECT_TRUE(fr.Open(project_root + "/resource/filetestcase"));
  fw.SetAppend(false);
  fw.WriteLine("line");
  fw.Flush();
  content_vec.clear();

  while (read_one_line = fr.ReadLine(), !read_one_line.empty()) {
    EXPECT_EQ(read_one_line, "line");
    content_vec.push_back(read_one_line);
  }

  EXPECT_EQ(content_vec.size(), 1);
  fw.Close();
  fr.Close();
}

void NormalFunction(const int &x) { std::cout << "Call Normal Function" << '\n'; };

void NormalFunction(int &&x) { std::cout << "Call RightRef Function" << '\n'; };

TEST(MyUtil, test_rightref) {
  NormalFunction(10);  // 右值优先绑定到右值引用的函数上
}

TEST(MyUtil, TotalMem) {
  auto p = new uint64_t[1024 * 1024];
  size_t mem_num = MemProfiler::memory();
  ASSERT_NE(0, mem_num);
  std::cout << mem_num << " KB";
  delete[] p;
}

TEST(MyUtil, JsonParser) {
  std::string_view json_str = "{\"key\": 11}";
  auto [obj, eaten] = parse(json_str);
  JSONDict meta = obj.get<JSONDict>();
  EXPECT_EQ(meta["key"]->get<int>(), 11);
}
