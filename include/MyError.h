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
    MyError(const std::string& msg) : _msg("Error: " + msg) {}
    const char* what() const noexcept override { return _msg.c_str(); }
private:
    std::string _msg;
};

class UnixError : public MyError {
public:
    UnixError() : MyError(strerror(errno)) {}
};

class MyLogicalError : public std::logic_error {
public:
    MyLogicalError(const std::string &msg) : logic_error(msg) {
        _msg = "MyLogicalError : " + msg;
    }
    const char* what() const noexcept override { return _msg.c_str(); }
private:
    std::string _msg;
};



#endif //MYCPPIMPLEMENT_ERROR_H
