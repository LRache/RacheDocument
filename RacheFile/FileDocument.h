//
// Created by 刘镇睿 on 2020/11/20.
//

#ifndef RACHEFILE_FILEDOCUMENT_H
#define RACHEFILE_FILEDOCUMENT_H

#include "SubFile.h"
#include "RByteSet/RByteSet.h"

struct SubFileCounter {
    ItemCounter itemCounter;
    int group = 0;
    int subFile = 0;
    int resource = 0;
    RBytesSize resourceSize = 0;
};

class FileDocument {
public:
    FileDocument();
    explicit FileDocument(std::string);
    ~FileDocument();

    struct ItemIter {
        SubFile *subFile;
        FileItemGroup *group;
        FileItemObject *item;
    };

    std::string name;
    int itemCount = 0;

    SubFile *get_subfile(int index) const;
    SubFile *get_subfile(ItemId id) const;
    FileItemObject *at(ItemId id) const;

    bool save(const std::string& filepath);
    bool open(const std::string& filepath);

    RLib::RByteSetWriter dump();
    bool load(RLib::RByteSetReader reader);

    void add_subfile(SubFile* subFile);
    int count_sub_file() const;
    int index_of(SubFile*) const;
    bool remove_subfile(SubFile* subFile);
    SubFileCounter count();
    bool contain_subfile(SubFile *subFile) const;

    std::vector<SubFile*> get_subs();

private:
    std::vector<SubFile*> subs;
    RByteSetFileLoader byteSetFileLoader;
};


#endif //RACHEFILE_FILEDOCUMENT_H
