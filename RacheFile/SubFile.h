//
// Created by 刘镇睿 on 2020/11/20.
//

#ifndef RACHEFILE_SUBFILE_H
#define RACHEFILE_SUBFILE_H

#include "FileItemGroup.h"
#include "ResourceSet.h"
#include <vector>

struct GroupCounter {
    ItemCounter itemCounter;
    int group = 0;
};

class SubFile {
public:
    explicit SubFile(std::string);

    void add_group(FileItemGroup*);
    bool remove_group(FileItemGroup* group);
    void clear_group();
    int size() const;

    FileItemGroup* get_group(int index) const;
    FileItemGroup* get_group(ItemId i) const;

    std::vector<FileItemGroup*> get_groups();
    GroupCounter count();

    std::string name;
    ResourceSet resourceSet;

private:
    std::vector<FileItemGroup*> groups;
};


#endif //RACHEFILE_SUBFILE_H
