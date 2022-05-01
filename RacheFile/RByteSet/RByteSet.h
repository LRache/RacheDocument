//
// Created by 刘镇睿 on 2020/12/13.
//

#ifndef RACHEFILE_RBYTESET_H
#define RACHEFILE_RBYTESET_H

#include <map>
#include <string>
#include "RBytes.h"

using std::map;
using std::string;

namespace RLib{
    class RByteSetReader {
    public:
        RByteSetReader();
        RByteSetReader(const RBytes& bytes);
        RByteSetReader(char *bytes, int size);
        RByteSetReader(const string& filename);

        bool load(const RBytes& bytes);
        bool load(char *bytes, int size);
        bool load(const string& filename);

        RBytesSize get(const string& name, char *bytes);
        RBytes get(const string& name);

        int count();

    private:
        map<string, RBytes> data;
    };

    class RByteSetWriter {
    public:
        RByteSetWriter();

        int add(const string& name, char *bytes, RBytesSize size);
        int add(const string& name, RBytes bytes);

        bool dump(const string& filename);
        RBytesSize dump(char *bytes);
        RBytes dump();

    private:
        int cnt = 0;
        map<string, RBytes> data;
    };
}

#endif //RACHEFILE_RBYTESET_H
