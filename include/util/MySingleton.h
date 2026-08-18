//
// Created by Lee on 2022/9/3.
//

#ifndef MYCPPIMPLEMENT_MYSINGLETON_H
#define MYCPPIMPLEMENT_MYSINGLETON_H

#include <atomic>
#include <memory>
#include <mutex>
#include <utility>

// 统一约定:
// - public 内先放 deleted 拷贝成员(报错清晰), 再放唯一公开 API getInstance();
// - private 内放默认构造、默认析构、内部辅助类/函数和静态成员;
// - 静态成员统一命名为 instance_ / s_flag_ / mutex_.

// implement1: shared_ptr + std::call_once
template <typename T>
class SingleDemo {
 public:
  SingleDemo(const SingleDemo &) = delete;

  SingleDemo &operator=(const SingleDemo &) = delete;

  static std::shared_ptr<T> getInstance();

 private:
  SingleDemo() = default;

  ~SingleDemo() = default;

  static void Init() { instance_ = std::make_shared<T>(); }

  static std::shared_ptr<T> instance_;
  static std::once_flag s_flag_;
};

template <typename T>
std::shared_ptr<T> SingleDemo<T>::instance_ = nullptr;

template <typename T>
std::once_flag SingleDemo<T>::s_flag_;

template <typename T>
std::shared_ptr<T> SingleDemo<T>::getInstance() {
  std::call_once(s_flag_, Init);
  return instance_;
}

// implement2: Meyers 魔法静态 (C++11 起初始化线程安全)
template <typename T>
class Singleton : private T {
 public:
  Singleton(const Singleton &) = delete;

  Singleton &operator=(const Singleton &) = delete;

  static T &getInstance();

 private:
  Singleton() = default;

  ~Singleton() override = default;
};

template <typename T>
T &Singleton<T>::getInstance() {
  static T s_oT;
  return s_oT;
}

// implement3: std::call_once + 原始指针, 退出时由守卫对象释放
template <typename T>
class OnceSingle {
 public:
  OnceSingle(const OnceSingle &) = delete;

  OnceSingle &operator=(const OnceSingle &) = delete;

  static T *getInstance();

 private:
  OnceSingle() = default;

  ~OnceSingle() = default;

  class CGFunctionClass {
   public:
    ~CGFunctionClass() {
      if (instance_ != nullptr) {
        delete instance_;
        instance_ = nullptr;
      }
    }
  };

  static void InitPtr() {
    instance_ = new T();
    static CGFunctionClass cg;
  }

  static T *instance_;
  static std::once_flag s_flag_;
};

template <typename T>
T *OnceSingle<T>::instance_ = nullptr;

template <typename T>
std::once_flag OnceSingle<T>::s_flag_;

template <typename T>
T *OnceSingle<T>::getInstance() {
  std::call_once(s_flag_, InitPtr);
  return instance_;
}

// implement4: std::call_once + 带参构造
template <typename T>
class OnceSingleWithArgs final {
 public:
  OnceSingleWithArgs(const OnceSingleWithArgs &) = delete;

  OnceSingleWithArgs &operator=(const OnceSingleWithArgs &) = delete;

  template <typename... Args>
  static T *getInstance(Args &&...args) {
    std::call_once(s_flag_, &InitPtr<Args...>, std::forward<Args>(args)...);
    return instance_;
  }

 private:
  OnceSingleWithArgs() = default;

  ~OnceSingleWithArgs() = default;

  class CGFunctionClass {
   public:
    ~CGFunctionClass() {
      if (instance_ != nullptr) {
        delete instance_;
        instance_ = nullptr;
      }
    }
  };

  template <typename... InitArgs>
  static void InitPtr(InitArgs &&...args) {
    instance_ = new T(std::forward<InitArgs>(args)...);
    static CGFunctionClass cg;
  }

  static T *instance_;
  static std::once_flag s_flag_;
};

template <typename T>
T *OnceSingleWithArgs<T>::instance_ = nullptr;

template <typename T>
std::once_flag OnceSingleWithArgs<T>::s_flag_;

// implement5: 原子指针 + 双重检查锁 (release store / acquire load 配对)
template <typename T>
class SingletonAtom {
 public:
  SingletonAtom(const SingletonAtom &) = delete;

  SingletonAtom &operator=(const SingletonAtom &) = delete;

  static T *getInstance();

 private:
  SingletonAtom() = default;

  ~SingletonAtom() = default;

  class CGFunctionClass {
   public:
    ~CGFunctionClass() {
      if (instance_ != nullptr) {
        delete instance_;
        instance_ = nullptr;
      }
    }
  };

  static std::atomic<T *> instance_;
  static std::mutex mutex_;
};

template <typename T>
std::atomic<T *> SingletonAtom<T>::instance_(nullptr);

template <typename T>
std::mutex SingletonAtom<T>::mutex_;

template <typename T>
T *SingletonAtom<T>::getInstance() {
  if (instance_.load(std::memory_order_relaxed) == nullptr) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (instance_.load(std::memory_order_relaxed) == nullptr) {
      // release store 与读路径的 acquire load 配对, 保证其他线程能看到
      // 对象的完整构造结果 (relaxed store + acquire load 没有 happens-before).
      instance_.store(new T(), std::memory_order_release);
      static CGFunctionClass cg;  // 避免内存泄露
    }
  }
  return instance_.load(std::memory_order_acquire);
}

#endif  // MYCPPIMPLEMENT_MYSINGLETON_H
