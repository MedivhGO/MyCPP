//
// Created by Lee on 2022/9/4.
//

#ifndef MYCPPIMPLEMENT_ERROR_H
#define MYCPPIMPLEMENT_ERROR_H

#include <cerrno>
#include <cstring>
#include <string>

class MyError : public std::exception {
public:
    explicit MyError(const std::string& msg) : msg_("Error: " + msg) {}
    auto what() const noexcept -> const char* override { return msg_.c_str(); }
private:
    std::string msg_;
};

class UnixError : public MyError {
public:
    UnixError() : MyError(strerror(errno)) {}
};

class MyLogicalError : public std::logic_error {
public:
    explicit MyLogicalError(const std::string &msg) : logic_error(msg) {
        msg_ = "MyLogicalError : " + msg;
    }
    auto what() const noexcept -> const char* override { return msg_.c_str(); }
private:
    std::string msg_;
};

#endif //MYCPPIMPLEMENT_ERROR_H
