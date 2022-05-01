//
// Created by 刘镇睿 on 2020/11/20.
//

#include "FileItemGroup.h"

#include <utility>

FileItemGroup::FileItemGroup(std::string n) {
    name = std::move(n);
}

void FileItemGroup::add_item(FileItemObject *itemObject) {
    items.push_back(itemObject);
}

std::vector<FileItemObject*> FileItemGroup::get_items() {
    return items;
}

bool FileItemGroup::remove_item(FileItemObject *item) {
    auto iter = items.begin();
    auto end = items.end();
    while (iter!=end) {
        if (*iter==item) {
            items.erase(iter);
            return true;
        }
        iter++;
    }
    return false;
}

FileItemObject *FileItemGroup::get_item(int index) const{
    return items.at(index);
}

void FileItemGroup::clear_item() {
    items.clear();
}

ItemCounter FileItemGroup::count() {
    ItemCounter counter;
    for (FileItemObject *item : items){
        counter.total++;
        switch (item->type) {

            case FileItemObject::NORMAL:
                counter.normal++;
                break;
            case FileItemObject::DOUBT:
                counter.doubt++;
                break;
            case FileItemObject::ATTENTION:
                counter.attention++;
                break;
            case FileItemObject::WARNING:
                counter.warning++;
                break;
        }
    }
    return counter;
}

FileItemGroup::~FileItemGroup() {
    for (auto i: items) {
        delete i;
    }
}

FileItemObject *FileItemGroup::get_item(ItemId i) const {
    return items[i.item];
}

int FileItemGroup::size() {
    return items.size();
}
