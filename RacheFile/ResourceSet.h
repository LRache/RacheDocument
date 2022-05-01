//
// Created by Rache on 2021/10/24.
//

#ifndef RACHEFILE_RESOURCESET_H
#define RACHEFILE_RESOURCESET_H

#include "RByteSet/RBytes.h"
#include "RByteSet/RByteSetFile.h"

#include <map>

struct ResItem{
    enum ResType{
        EMPTY,
        IMAGE,
        TEXT,
        AUDIO,
        VIDEO,
        OTHER,
    }type;
    std::string name;
    RByteSetRef *byteRef;
};

class ResourceSet {
public:
    ResourceSet();

    ResItem add(const std::string& name, char *bytes, int size, ResItem::ResType type);
    ResItem add(const std::string& name, RLib::RBytes, ResItem::ResType type);
    ResItem add(const std::string& name, RByteSetRef *ref, ResItem::ResType type);
    bool remove(const std::string& name);
    ResItem get(const std::string& name);
    ResItem get(const std::string& name, bool &hasError);

    std::map<std::string, ResItem> res;
};


#endif //RACHEFILE_RESOURCESET_H
