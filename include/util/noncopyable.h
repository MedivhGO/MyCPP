//
// Created by Lee on 2018/6/29.
// Modify time 2022年9月2日01:25:35
//

#ifndef NONCOPYABLE_H
#define NONCOPYABLE_H

class noncopyable {
 protected:
  noncopyable() = default;

  ~noncopyable() = default;

 public:
  noncopyable(const noncopyable &) = delete;

  noncopyable &operator=(const noncopyable &) = delete;
};

/* C++11 写法 */

class noncopyableCXX11 {
 public:
  constexpr noncopyableCXX11() = default;

  ~noncopyableCXX11() = default;

  noncopyableCXX11(const noncopyable &) = delete;

  noncopyableCXX11 &operator=(const noncopyableCXX11 &) = delete;
};

#endif  // NONCOPYABLE_H
