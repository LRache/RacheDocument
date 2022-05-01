//
// Created by 刘镇睿 on 2020/11/20.
//

#ifndef RACHEFILE_FILEITEMOBJECT_H
#define RACHEFILE_FILEITEMOBJECT_H

#include <string>
#include "RByteSet/RBytes.h"
#include "ResourceSet.h"

struct ItemId {
    int subfile = -1;
    int group = -1;
    int item = -1;
};

class FileItemObject {
public:
    FileItemObject(const std::string &name, const std::string &text, const std::string &source);

    enum TYPE{
        NORMAL,
        DOUBT,
        ATTENTION,
        WARNING,
    };

    std::string name;
    std::string text;
    std::string source;
    std::string resName;
    int year = 0;
    int month = 0;
    int day = 0;
    TYPE type = NORMAL;
};


#endif //RACHEFILE_FILEITEMOBJECT_H
