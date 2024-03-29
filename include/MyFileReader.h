//
// Created by Lee on 2022/9/17.
//

#ifndef MYCPPIMPLEMENT_MYFILEREADER_H
#define MYCPPIMPLEMENT_MYFILEREADER_H

#include <fstream>

class FileReader {
 public:
  FileReader();

  ~FileReader();

  auto Open(const std::string &directory) -> bool;

  auto HasOpen() -> bool;

  auto HasEnd() -> bool;

  auto ReadLine() -> std::string;

  void Close();

 private:
  std::string file_directory_;
  std::ifstream fin_;
  bool has_open_;
  bool has_end_;
};

FileReader::FileReader() {
  file_directory_ = "";
  has_open_ = false;
  has_end_ = false;
}

void FileReader::Close() {
  if (has_open_) {
    fin_.close();
  }
}

FileReader::~FileReader() {
  if (has_open_) {
    fin_.close();
  }
}

auto FileReader::Open(const std::string &directory) -> bool {
  if (fin_.is_open()) {
    fin_.close();
  }

  file_directory_ = directory;
  fin_.open(directory, std::ios::in);

  has_open_ = fin_.is_open();
  return has_open_;
}

auto FileReader::HasOpen() -> bool { return has_open_; }

auto FileReader::HasEnd() -> bool { return has_end_; }

auto FileReader::ReadLine() -> std::string {
  std::string next_line;
  if (!has_open_ || has_end_) {
    return next_line;
  }

  if (fin_.eof()) {
    has_end_ = true;
    return next_line;
  }

  getline(fin_, next_line);
  return next_line;
}

#endif  // MYCPPIMPLEMENT_MYFILEREADER_H
