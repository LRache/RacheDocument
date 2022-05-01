//
// Created by 刘镇睿 on 2020/11/20.
//

#include "SubFile.h"

SubFile::SubFile(std::string n) {
    name = n;
}

void SubFile::add_group(FileItemGroup *g) {
    groups.push_back(g);
}

std::vector<FileItemGroup*> SubFile::get_groups() {
    return groups;
}

bool SubFile::remove_group(FileItemGroup *group) {
    auto iter = groups.begin();
    auto end = groups.end();
    while (iter != end) {
        if (*iter==group) {
            groups.erase(iter);
            return true;
        }
        iter++;
    }
    return false;
}

FileItemGroup *SubFile::get_group(int index) const{
    return groups[index];
}

void SubFile::clear_group() {
    groups.clear();
}

GroupCounter SubFile::count() {
    GroupCounter counter;
    int size = groups.size();
    for (int i=0; i<size; i++) {
        auto group = groups[i];
        counter.itemCounter = counter.itemCounter + group->count();
        counter.group++;
    }
    return counter;
}

FileItemGroup *SubFile::get_group(ItemId i) const{
    return groups[i.group];
}

int SubFile::size() const {
    return groups.size();
}
