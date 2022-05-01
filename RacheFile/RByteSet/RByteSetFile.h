//
// Created by Rache on 2021/11/6.
//

#ifndef RACHEFILE_RBYTESETFILE_H
#define RACHEFILE_RBYTESETFILE_H

#include <fstream>
#include <map>
#include "RBytes.h"

class RByteSetFileLoader;

class RByteSetRef {
public:
    RByteSetFileLoader* loader = nullptr;
    bool loaded = false;

    bool writeStream(std::ofstream &ofstream);
    bool load();
    RBytesSize get_size() const;
    RLib::RBytes read();

    RBytesSize start = 0;
    RBytesSize size = 0;
    RLib::RBytes bytes;
};

typedef std::map<std::string, RByteSetRef*> RByteSetRefMap;

class RByteSetFileLoader {
public:
    RByteSetFileLoader() = default;

    void close();
    bool is_open();
    bool load(const std::string& filename);

    RByteSetRef* get_reference(const std::string& name);
    bool writeStream(RByteSetRef* ref, std::ofstream &ofstream);
    bool read(RByteSetRef* ref);
    RLib::RBytes read(const std::string& name);
    unsigned long long count();
    unsigned long long get_names(std::string names[]);

private:
    RByteSetRefMap data;
    std::ifstream fsteam;
};

class RByteSetFileDumper {
public:
    RByteSetFileDumper();

    void add(const std::string& name, RLib::RBytes bytes);
    void add(const std::string& name, RByteSetRef *ref);

    void dump(const std::string& filename);

private:
    RByteSetRefMap data;
};


#endif //RACHEFILE_RBYTESETFILE_H
