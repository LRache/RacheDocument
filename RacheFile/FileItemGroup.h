//
// Created by 刘镇睿 on 2020/11/20.
//

#ifndef RACHEFILE_FILEITEMGROUP_H
#define RACHEFILE_FILEITEMGROUP_H

#include "FileItemObject.h"
#include <vector>

struct ItemCounter{
    int total = 0;
    int normal = 0;
    int doubt = 0;
    int attention = 0;
    int warning = 0;

    ItemCounter operator+(ItemCounter counter) const{
        ItemCounter counter1;
        counter1.total = counter.total + total;
        counter1.doubt = counter.doubt + doubt;
        counter1.warning = counter.warning + warning;
        counter1.attention = counter.attention + attention;
        counter1.normal = counter.normal + normal;
        return counter1;
    }
};


class FileItemGroup {
public:
    explicit FileItemGroup(std::string);
    ~FileItemGroup();

    std::string name;
    bool hide = false;

    FileItemObject* get_item(int index) const;
    int size();

    void add_item(FileItemObject* item);
    void clear_item();
    bool remove_item(FileItemObject* item);
    std::vector<FileItemObject*> get_items();

    ItemCounter count();
    FileItemObject *get_item(ItemId i) const;

private:
    std::vector<FileItemObject*> items;
};


#endif //RACHEFILE_FILEITEMGROUP_H
