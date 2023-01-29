//
// Created by Lee on 2022/9/3.
//

#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <vector>

#include "MyCache.h"
#include "MyError.h"
#include "MyFileReader.h"
#include "MyFileWriter.h"
#include "MyLog.h"
#include "MyProfile.h"
#include "MyProjectPath.h"
#include "MyRandom.h"
#include "MySingleton.h"
#include "MyString.h"
#include "MyThread.h"

using std::cout;
using std::endl;

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

void NormalFunction(const int &x) { std::cout << "Call Normal Function" << std::endl; };

void NormalFunction(int &&x) { std::cout << "Call RightRef Function" << std::endl; };

TEST(MyUtil, test_rightref) {
  NormalFunction(10);  // 右值优先绑定到右值引用的函数上
}
