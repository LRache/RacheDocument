//
// Created by ¡ı’ÚÓ£ on 2020/12/13.
//

#include "RByteSet.h"
#include <iostream>
#include <fstream>

RLib::RByteSetReader::RByteSetReader() {

}

RLib::RByteSetReader::RByteSetReader(char *bytes, int size) {
    load(bytes, size);
}

RLib::RByteSetReader::RByteSetReader(const RLib::RBytes& bytes) {
    load(bytes);
}

RLib::RByteSetReader::RByteSetReader(const string& filename) {
    load(filename);
}

RBytesSize RLib::RByteSetReader::get(const string& name, char *bytes) {
    return data[name].read(bytes);
}

RLib::RBytes RLib::RByteSetReader::get(const string& name) {
    auto iter = data.find(name);
    if (iter == data.end()) return RBytes();
    return iter->second;
}

bool RLib::RByteSetReader::load(char *bytes, int size) {
    RBytes b = RBytes(bytes, size);
    return load(b);
}

bool RLib::RByteSetReader::load(const string& filename) {
    std::fstream fstream;
    fstream.open(filename + ".txt", std::ios::binary | std::ios::out);
    std::locale::global(std::locale(""));
    if (fstream.is_open()) std::cout << "1";
    /*bool e = false;
    RBytes b = RBytes::readfile(filename, &e);
    if (e) {
        return false;
    }
    return load(b);*/
    return false;
}

bool RLib::RByteSetReader::load(const RLib::RBytes& bytes) {
    RBytesSize seek = 0;
    char c;

    RBytesSize len = 0;
    while (true) {
        c = bytes[seek];
        seek++;
        if (c == '\n') break;
        len += c - '0';
        len *= 10;
    }
    len /= 10;

    RBytesSize sizes[len];
    string names[len];
    for (RBytesSize i = 0; i < len; ++i) {
        string name;
        RBytes d;
        RBytesSize s = 0;
        while (true) {
            c = bytes[seek];
            seek++;
            if (c == '\t') break;
            name += c;
        }
        while (true) {
            c = bytes[seek];
            seek++;
            if (c == '\n') break;
            s += c - '0';
            s *= 10;
        }
        s /= 10;
        std::cout << "[RByteSetReader]load byte item: name="<<name<<" size="<<s<<std::endl;

        sizes[i] = s;
        names[i] = name;
    }

    for (RBytesSize i = 0; i < len; ++i) {
        RBytes b = RBytes::cut(bytes, sizes[i], seek);
        data[names[i]] = b;
        seek += sizes[i];
    }
    return true;
}

int RLib::RByteSetReader::count() {
    return (int)data.size();
}

RLib::RByteSetWriter::RByteSetWriter() {

}

int RLib::RByteSetWriter::add(const string& name, char *bytes, RBytesSize size) {
    data[name] = RBytes(bytes, size);
    return ++cnt;
}

RLib::RBytes RLib::RByteSetWriter::dump() {
    RBytes bytes;
    bytes << cnt;
    bytes << '\n';

    RBytes dataBytes;
    for (auto &item : data) {
        bytes << item.first;
        bytes << '\t';
        bytes << item.second.size();
        bytes << '\n';
        dataBytes << item.second;
    }
    bytes << dataBytes;
    return bytes;
}

int RLib::RByteSetWriter::add(const string& name, RLib::RBytes bytes) {
    data[name] = std::move(bytes);
    return ++cnt;
}

RBytesSize RLib::RByteSetWriter::dump(char *bytes) {
    RBytes b = dump();
    b.read(bytes);
    return b.size();
}

bool RLib::RByteSetWriter::dump(const string& filename) {
    RBytes b = dump();
    return RBytes::writefile(filename, b);
}
