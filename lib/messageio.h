#ifndef MESSAGGEIO_H
#define MESSAGGEIO_H

#include "iostream"
#include "fstream"
#include "stdint.h"
#include "string.h"
#include "stddef.h"

#define BUFF_SIZE 1024
#define FILE_ERROR "Non-existent file"

using std::ifstream;
using std::ofstream;
using std::getline;
using std::cout;
using std::endl;
using std::cerr;
using std::size_t;
using std::string;

class Message {
    public:
        Message(const char * file_path);
        ~Message() = default;
        static string read_txt(const char * file_path);
        void write_txt(const char * file_path);
        string repr_string();
        void print_array();

        uint8_t * data;
        size_t data_len;

};

Message :: Message(const char * file_path){
    string buffer = read_txt(file_path);
    this->data = (uint8_t*)buffer.c_str();
    this->data_len = buffer.length();
}

void Message :: write_txt(const char * file_path){
    ofstream file_stream(file_path);
    if(file_stream.is_open()) file_stream << this->data;
    else {
      cerr << FILE_ERROR << endl;
      exit(1);
    }
    file_stream.close();
}

string Message :: read_txt(const char * file_path) {
  ifstream file_stream(file_path);
  if(file_stream.is_open()) {
    file_stream.seekg(0, file_stream.end);
    size_t actual_len = file_stream.tellg();
    file_stream.seekg(0, file_stream.beg);
    size_t aligned_len = actual_len % 8 == 0 ? actual_len : actual_len + (8 - (actual_len % 8));
    auto * buffer = new char [aligned_len];
    file_stream.read(buffer, actual_len);
    file_stream.close();
    return string(buffer);
  }
  else {
    cerr << FILE_ERROR << endl;
    exit(1);
  }
}

void Message :: print_array() {
    cout << "[ ";
    for (size_t i = 0; i < this->data_len; ++i)
        cout << this->data[i];
    cout << " ]" << endl;
}

string Message :: repr_string(){
    return string((char*)this->data);
}

#endif