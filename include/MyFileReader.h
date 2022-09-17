//
// Created by Lee on 2022/9/17.
//

#ifndef MYCPPIMPLEMENT_MYFILEREADER_H
#define MYCPPIMPLEMENT_MYFILEREADER_H

#include <fstream>

using std::ifstream;
using std::string;
using std::ios;

class FileReader {
  public:
    FileReader();

    ~FileReader();

    bool open(string directory);

    bool hasOpen();

    bool hasEnd();

    string readLine();

  private:
    string file_directory;
    ifstream fin;
    bool has_open;
    bool has_end;
};

FileReader::FileReader() {
    file_directory = "";
    has_open = false;
    has_end = false;
}

FileReader::~FileReader() {
    if (has_open) {
        fin.close();
    }
}

bool FileReader::open(string directory) {
    if (fin.is_open()) {
        fin.close();
    }

    file_directory = directory;
    fin.open(directory, ios::in);

    has_open = fin.is_open();
    return has_open;
}

bool FileReader::hasOpen() { return has_open; }

bool FileReader::hasEnd() { return has_end; }

string FileReader::readLine() {
    string next_line = "";
    if (!has_open || has_end) {
        return next_line;
    }

    if (fin.eof()) {
        has_end = true;
        return next_line;
    }

    getline(fin, next_line);
    return next_line;
}

#endif // MYCPPIMPLEMENT_MYFILEREADER_H
