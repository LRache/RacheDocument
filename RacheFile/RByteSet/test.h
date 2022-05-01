//
// Created by Rache on 2021/11/7.
//

#ifndef RACHEFILE_TEST_H
#define RACHEFILE_TEST_H

#include <iostream>
#include <string>
#include "RBytes.h"
#include "RByteSetFile.h"

void test_loader(const std::string &filename) {
    RByteSetFileLoader loader(filename);
    loader.load();
    std::string names[loader.count()];
    loader.get_names(names);
    for (auto name : names) {
        auto ref = loader.get_reference(name);
        std::cout << name << " " <<ref->size << std::endl;
    }
}

void test_dumper(const std::string &filename) {
    RByteSetFileDumper dumper;

    std::string prefix = "prefix";
    for (int i = 0; i < 128; ++i) {
        int l = rand() % 32768 + 1;
        char buffer[l];
        for (int j = 0; j < l; ++j) {
            buffer[j] = rand() % 128;
        }
        dumper.add(prefix + std::to_string(i), RLib::RBytes(buffer, l));
    }
    dumper.dump(filename);
}

#endif //RACHEFILE_TEST_H
