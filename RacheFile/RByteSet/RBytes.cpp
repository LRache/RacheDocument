//
// Created by 刘镇睿 on 2021/1/5.
//

#include "RBytes.h"
#include <iostream>
#include <fstream>

using std::fstream;

RLib::RBytes::RBytes() {

}

RLib::RBytes::RBytes(char *bytes, RBytesSize size) {
    write(bytes, size);
}


RLib::RBytes::RBytes(const string& str) {
    write(str);
}

void RLib::RBytes::write(const char bytes[], RBytesSize size) {
    if (bytes == nullptr) return;
    for (int i = 0; i < size; ++i) {
        data.emplace_back(bytes[i]);
    }
}

RLib::RBytes RLib::RBytes::readfile(const string& filename, bool *hasError) {
    RBytes bytes;
    fstream file(filename, std::ios::binary | std::ios::in);
    if (!file.is_open()) {
        if (hasError != nullptr) *hasError = true;
        return bytes;
    }
    char buffer[RBYTES_BUFFER_SIZE];
    while (!file.eof()){
        file.read(buffer, RBYTES_BUFFER_SIZE);
        bytes.write(buffer, file.gcount());
    }
    file.close();
    *hasError = false;
    return bytes;
}

RBytesSize RLib::RBytes::size() const {
    return data.size();
}

void RLib::RBytes::write(const char byte) {
    data.emplace_back(byte);
}

void RLib::RBytes::write(const std::string& str) {
    for (char c : str) {
        data.emplace_back(c);
    }
}

void RLib::RBytes::write(int number) {
    write(std::to_string(number));
}

void RLib::RBytes::operator<<(char byte) {
    data.emplace_back(byte);
}

void RLib::RBytes::operator<<(const std::string& str) {
    write(str);
}

void RLib::RBytes::operator<<(char *str) {
    write(str, strlen(str));
}

void RLib::RBytes::operator<<(int number) {
    write(std::to_string(number));
}

void RLib::RBytes::operator<<(const RLib::RBytes& bytes) {
    write(bytes);
}

char RLib::RBytes::operator[](RBytesSize index) const {
    return data[index];
}

void RLib::RBytes::set(char byte, int index) {
    data[index] = byte;
}

RLib::RBytes RLib::RBytes::cut(const RBytes& source, RBytesSize length, unsigned long long int seek) {
    RBytes bytes;
    for (int i = 0; i < length; ++i) {
        bytes.write(source[seek+i]);
    }
    return bytes;
}

RLib::RBytes RLib::RBytes::split(const RBytes& source, char c, RBytesSize &seek) {
    RBytesSize start_seek = seek;
    RBytesSize max_seek = source.size();
    while (true) {
        seek++;
        if (seek>=max_seek) return {};
        if (source[seek] == c) break;
    }
    RBytesSize length = seek - start_seek;
    RBytes bytes;
    for (RBytesSize i = 0; i < length; ++i) {
        bytes.set(source[start_seek+i], i);
    }
    seek++;
    return bytes;
}

RLib::RBytes RLib::RBytes::readline(const RBytes& bytes, RBytesSize &seek) {
    return split(bytes, '\n', seek);
}

RBytesSize RLib::RBytes::read(char *bytes, RBytesSize start, RBytesSize length) const {
    RBytesSize end = data.size();
    RBytesSize index = start;
    RBytesSize i = 0;
    for (; i < length; ++i) {
        if (index == end) {
            break;
        }
        bytes[i] = data[index];
        index++;
    }
    return i;
}

char RLib::RBytes::get(int index) const{
    return data[index];
}

RBytesSize RLib::RBytes::read(char *bytes) const{
    if (bytes == nullptr) return 0;
    RBytesSize i = 0;
    for (char c : data) {
        bytes[i] = c;
        i++;
    }
    return i;
}

void RLib::RBytes::write(const RLib::RBytes& bytes) {
    for (auto &b : bytes.data) {
        data.emplace_back(b);
    }
}

bool RLib::RBytes::writefile(const string &filename, const RBytes& bytes) {
    RBytesSize size= bytes.size();
    char buffer[size];
    bytes.read(buffer);

    fstream s(filename, std::ios::binary | std::ios::out);
    if (!s.is_open()) return false;
    s.write(buffer, size);
    s.close();

    return true;
}

string RLib::RBytes::to_string() {
    string str;
    for (char c : data) {
        str.push_back(c);
    }
    return str;
}

RBytesSize RLib::RBytes::read_string(char *s) const {
    RBytesSize t = read(s);
    s[t] = '\0';
    return t;
}

bool RLib::RBytes::empty() const {
    return data.empty();
}

void RLib::RBytes::operator<<(RBytesSize number) {
    write(std::to_string(number));
}

std::string RLib::RBytes::read_string() {
    std::string s;
    for (auto b : data) {
        s.push_back(b);
    }
    return s;
}