//
// Created by Rache on 2021/11/6.
//

#include "RByteSetFile.h"

#include <iostream>
#include <cstdio>

RLib::RBytes RByteSetRef::read() {
    if (!loaded) {
        if (!load()) return RLib::RBytes();
    }
    return bytes;
}

bool RByteSetRef::load() {
    if (loader == nullptr) return false;
    return loader->read(this);
}

bool RByteSetRef::writeStream(std::ofstream &ofstream) {
    if (loader == nullptr) {
        char buffer[RBYTES_BUFFER_SIZE];
        RBytesSize l = bytes.size();
        RBytesSize seek = 0;
        while (l > RBYTES_BUFFER_SIZE) {
            bytes.read(buffer, seek, RBYTES_BUFFER_SIZE);
            ofstream.write(buffer, RBYTES_BUFFER_SIZE);
            l -= RBYTES_BUFFER_SIZE;
            seek += RBYTES_BUFFER_SIZE;
        }
        bytes.read(buffer, seek, l);
        ofstream.write(buffer, l);
        return true;
    } else {
        return loader->writeStream(this, ofstream);
    }
}

RBytesSize RByteSetRef::get_size() const {
    if (loaded) return bytes.size();
    return size;
}

bool RByteSetFileLoader::is_open() {
    return fsteam.is_open();
}

bool RByteSetFileLoader::load(const std::string& filename) {
    if (fsteam.is_open()) fsteam.close();
    fsteam.open(filename, std::ios::binary);
    if (!fsteam.is_open()) return false;

    data.clear();

    char lengthBytes[64];
    fsteam.getline(lengthBytes, 64);
    int length = strtol(lengthBytes, nullptr, 10);

    std::string byteNames[length];
    RBytesSize byteSizes[length];
    for (int i = 0; i < length; ++i) {
        char setInfo[128], setName[128], setLength[64];
        fsteam.getline(setInfo, 128);
        sscanf(setInfo, "%s\t%s", setName, setLength);
        byteNames[i] = setName;
        byteSizes[i] = strtoul(setLength, nullptr, 10);
    }

    RBytesSize seek = fsteam.tellg();
    for (int i = 0; i < length; ++i) {
        auto *ref = new RByteSetRef;
        ref->loader = this;
        ref->start = seek;
        ref->size = byteSizes[i];
        data.insert(std::pair<std::string, RByteSetRef*>(byteNames[i], ref));
        seek += byteSizes[i];
    }

    return true;
}

bool RByteSetFileLoader::read(RByteSetRef *ref) {
    if (ref == nullptr) return false;
    if (!fsteam.is_open()) return false;
    RBytesSize size = ref->size;
    fsteam.seekg(ref->start);

    char buffer[RBYTES_BUFFER_SIZE];
    while (size > RBYTES_BUFFER_SIZE) {
        fsteam.read(buffer, RBYTES_BUFFER_SIZE);
        ref->bytes.write(buffer, RBYTES_BUFFER_SIZE);
        size -= RBYTES_BUFFER_SIZE;
    }
    fsteam.read(buffer, size);
    ref->bytes.write(buffer, size);

    std::cout << "[RByteSetFileLoader]read(RByteSetRef*) start=" << ref->start << " size=" << ref->size << std::endl;
    ref->loaded = true;
    return true;
}

RByteSetRef *RByteSetFileLoader::get_reference(const std::string& name) {
    auto iter = data.find(name);
    if (iter == data.end()) return nullptr;
    return iter->second;
}

unsigned long long RByteSetFileLoader::count() {
    return data.size();
}

unsigned long long RByteSetFileLoader::get_names(std::string *names) {
    unsigned long long i = 0;
    auto end = data.end();
    for (auto it = data.begin(); it != end; it++) {
        names[i++] = it->first;
    }
    return i;
}

RLib::RBytes RByteSetFileLoader::read(const string &name) {
    auto ref = get_reference(name);
    if (ref == nullptr) return RLib::RBytes();
    read(ref);
    return ref->bytes;
}

bool RByteSetFileLoader::writeStream(RByteSetRef *ref, std::ofstream &ofstream) {
    RBytesSize size = ref->size;
    fsteam.clear();
    fsteam.seekg(ref->start);
    char buffer[RBYTES_BUFFER_SIZE];
    while (size > RBYTES_BUFFER_SIZE) {
        fsteam.read(buffer, RBYTES_BUFFER_SIZE);
        ofstream.write(buffer, RBYTES_BUFFER_SIZE);
        size -= RBYTES_BUFFER_SIZE;
    }
    fsteam.read(buffer, size);
    ofstream.write(buffer, size);
    std::cout << "[RByteSetFileLoader]writeStream(RByteSetRef*, std::ofstream): tellp=" << ofstream.tellp() << " ref.size=" << ref->size << std::endl;
    return true;
}

void RByteSetFileLoader::close() {
    fsteam.close();
}

RByteSetFileDumper::RByteSetFileDumper() {

}

void RByteSetFileDumper::add(const std::string& name, RLib::RBytes bytes) {
    auto *ref = new RByteSetRef;
    ref->loaded = true;
    ref->bytes = std::move(bytes);
    data.insert(std::pair<std::string, RByteSetRef*>(name, ref));
}

void RByteSetFileDumper::add(const string &name, RByteSetRef *ref) {
    data.insert(std::pair<std::string, RByteSetRef*>(name, ref));
}

void RByteSetFileDumper::dump(const string &filename) {
    std::ofstream ofstream;
    ofstream.open(filename, std::ios::binary);
    ofstream << data.size();
    ofstream << '\n';

    auto end = data.end();
    for (auto it = data.begin(); it != end; it++) {
        ofstream << it->first << '\t';
        if (it->second->size == 0) {
            ofstream << it->second->bytes.size();
        } else {
            ofstream << it->second->size;
        }
        ofstream << '\n';
    }

    for (auto it = data.begin(); it != end; it++) {
        auto ref = it->second;
        char buffer[RBYTES_BUFFER_SIZE];
        if (ref->loader == nullptr) {
            RBytesSize size = ref->bytes.size();
            RBytesSize seek = 0;
            while (size > RBYTES_BUFFER_SIZE) {
                ref->bytes.read(buffer, seek, RBYTES_BUFFER_SIZE);
                seek += RBYTES_BUFFER_SIZE;
                size -= RBYTES_BUFFER_SIZE;
                ofstream.write(buffer, RBYTES_BUFFER_SIZE);
            }
            ref->bytes.read(buffer, seek, size);
            ofstream.write(buffer, size);
        } else {
            ref->loader->writeStream(ref, ofstream);
        }
    }

    ofstream.close();
}
