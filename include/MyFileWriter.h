//
// Created by Lee on 2023/1/29.
//

#ifndef MYCPPIMPLEMENT_MYFILEWRITER_H
#define MYCPPIMPLEMENT_MYFILEWRITER_H

#include <fstream>
#include <vector>

class FileWriter {
 public:
  FileWriter() {
    file_directory_ = "";
    append_ = false;
  }

  explicit FileWriter(const std::string &path) {
    this->file_directory_ = path;
    fout_.open(path);
    this->append_ = false;
  }

  FileWriter(const std::string &path, const bool &append) {
    this->file_directory_ = path;
    this->append_ = append;
    if (!append) {
      fout_.open(path);
      return;
    }
    fout_.open(path, std::ios::app);
  }

  auto Open(const std::string &path) -> bool {
    if (fout_.is_open()) {
      fout_.close();
    }

    file_directory_ = path;
    fout_.open(path, std::ios::out);

    return fout_.is_open();
  }

  void SetAppend(const bool &append) {
    this->append_ = append;
    if (append) {
      fout_.close();
      fout_.open(file_directory_, std::ios::app);
    }
  }

  auto WriteLine(const std::string &line) -> bool {
    fout_ << line;
    return true;
  }

  auto WriteVector(const std::vector<std::string> &svec) -> bool {
    for (auto &i : svec) {
      fout_ << i << std::endl;
    }
    return true;
  }

  auto WriteVector(const std::vector<std::string> &svec, const std::string &delimiter) -> bool {
    if (append_) {
      for (const auto &i : svec) {
        fout_ << i << delimiter;
      }
      return true;
    }
    SetAppend(true);
    for (const auto &i : svec) {
      fout_ << i << delimiter;
    }
    append_ = false;
    return true;
  }

  auto IsAppend() -> bool { return append_; }

  auto IsOpen() -> bool { return fout_.is_open(); }

  auto Close() -> bool {
    fout_.close();
    return true;
  }

  void Flush() { fout_.flush(); }

  ~FileWriter() { fout_.close(); }

 private:
  std::ofstream fout_;
  std::string file_directory_;
  bool append_;
};

#endif  // MYCPPIMPLEMENT_MYFILEWRITER_H
