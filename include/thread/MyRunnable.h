#ifndef MYCPPIMPLEMENT_MYRUNNABLE_H
#define MYCPPIMPLEMENT_MYRUNNABLE_H

class Runnable {
 public:
  virtual ~Runnable() = 0;

  virtual void run() = 0;
};

Runnable::~Runnable() = default;

#endif  // MYCPPIMPLEMENT_MYRUNNABLE_H
