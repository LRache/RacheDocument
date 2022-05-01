//
// Created by 刘镇睿 on 2021/1/5.
//

#ifndef RACHEFILE_RBYTES_H
#define RACHEFILE_RBYTES_H

#include <vector>
#include <string>
#include <cstring>

#define RBYTES_BUFFER_SIZE 32768

typedef size_t RBytesSize;

using std::string;

namespace RLib{
    class RBytes {
    public:
        RBytes();
        RBytes(char *bytes, RBytesSize size);
        explicit RBytes(const string& str);

        void write(const char bytes[], RBytesSize size);
        void write(char byte);
        void write(const std::string& str);
        void write(int number);
        void write(const RBytes& bytes);

        RBytesSize read(char *bytes, RBytesSize start, RBytesSize length) const;
        RBytesSize read(char *bytes) const;
        RBytesSize read_string(char *s) const;
        std::string read_string();
        string to_string();

        void set(char byte, int index);
        char get(int index) const;

        bool empty() const;
        RBytesSize size() const;

        void operator<<(RBytesSize number);
        void operator<<(char byte);
        void operator<<(const std::string& str);
        void operator<<(char *str);
        void operator<<(int number);
        void operator<<(const RBytes& bytes);

        char operator[](RBytesSize index) const;

        static RBytes cut(const RBytes& source, RBytesSize length, unsigned long long int seek);
        static RBytes split(const RBytes& source, char c, RBytesSize &seek);
        static RBytes readline(const RBytes& bytes, RBytesSize &seek);

        static RBytes readfile(const string& filename, bool *hasError = nullptr);
        static bool writefile(const string& filename, const RBytes& bytes);

    private:
        std::vector<char> data;
    };
};


#endif //RACHEFILE_RBYTES_H
