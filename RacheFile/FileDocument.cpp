//
// Created by ¡ı’ÚÓ£ on 2020/11/20.
//

#include "FileDocument.h"
#include <iostream>

#include "rapidjson/writer.h"
#include "rapidjson/document.h"

using namespace RLib;
typedef rapidjson::Document JsonDocument;
typedef rapidjson::Value::Array JsonArray;
typedef rapidjson::Value::Object JsonObject;
typedef rapidjson::Value::Object::MemberIterator JsonObjectIterator;
typedef rapidjson::StringBuffer JsonStringBuffer;
typedef rapidjson::Writer<JsonStringBuffer> JsonStringWriter;


void log(const string& s) {
    std::cout << s << std::endl;
}

FileDocument::FileDocument(std::string n) {
    name = std::move(n);
}

void FileDocument::add_subfile(SubFile *subFile) {
    subs.emplace_back(subFile);
}

bool FileDocument::save(const std::string& filepath) {
    RByteSetFileDumper dumper;

    JsonStringBuffer indexObjectStrBuf;
    JsonStringWriter indexObjectWriter(indexObjectStrBuf);
    JsonStringBuffer resourceIndexObjectStrBuf;
    JsonStringWriter resourceIndexObjectWriter(resourceIndexObjectStrBuf);

    // dump items
    int subFileNumber = 0;
    const std::string subFilePrefix = "sf";
    const std::string resourceFilePrefix = "rs";

    indexObjectWriter.StartObject();
    resourceIndexObjectWriter.StartObject();
    for (SubFile *sub: subs) { // sub file loop
        if (sub == nullptr) continue;
        rapidjson::StringBuffer subFileStrBuf;
        rapidjson::Writer<rapidjson::StringBuffer> subFileArrayWriter(subFileStrBuf);
        subFileArrayWriter.StartArray();

        for (FileItemGroup *group : sub->get_groups()) {
            subFileArrayWriter.StartArray();
            subFileArrayWriter.String(group->name.c_str());
            subFileArrayWriter.Bool(group->hide);
            subFileArrayWriter.StartArray();

            for (FileItemObject *item : group->get_items()) {
                // normal
                subFileArrayWriter.StartArray();
                subFileArrayWriter.String(item->name.c_str());
                subFileArrayWriter.String(item->text.c_str());
                subFileArrayWriter.Int(item->type);
                subFileArrayWriter.String(item->source.c_str());
                subFileArrayWriter.Int(item->year);
                subFileArrayWriter.Int(item->month);
                subFileArrayWriter.Int(item->day);
                if (!item->resName.empty()) subFileArrayWriter.String(item->resName.c_str());

                subFileArrayWriter.EndArray();
            }
            subFileArrayWriter.EndArray();
            subFileArrayWriter.EndArray();
        }
        subFileArrayWriter.EndArray();

        // dump subfile
        std::string numberString = std::to_string(subFileNumber++);

        RBytes binary(subFileStrBuf.GetString());
        std::string sfn = subFilePrefix + numberString;
        indexObjectWriter.Key(sub->name.c_str());
        indexObjectWriter.String(sfn.c_str());
        dumper.add(sfn, binary);

        if (!sub->resourceSet.res.empty()) {
            int rfnumber = 0;
            std::string rfnp = resourceFilePrefix + numberString;
            rfnp += "_";
            resourceIndexObjectWriter.Key(sub->name.c_str());
            resourceIndexObjectWriter.StartObject();
            auto endResIter = sub->resourceSet.res.end();
            for (auto resIter = sub->resourceSet.res.begin(); resIter != endResIter; resIter++) {
                std::string rfn = rfnp + std::to_string(rfnumber++);
                resourceIndexObjectWriter.Key(resIter->first.c_str());
                resourceIndexObjectWriter.StartArray();
                resourceIndexObjectWriter.String(rfn.c_str());
                resourceIndexObjectWriter.Int(resIter->second.type);
                resourceIndexObjectWriter.EndArray();
                dumper.add(rfn, resIter->second.byteRef);
            }
            resourceIndexObjectWriter.EndObject();
        }
    }
    resourceIndexObjectWriter.EndObject();
    indexObjectWriter.EndObject();

    JsonStringBuffer infoJsonArrayStrBuf;
    JsonStringWriter infoJsonArrayWriter(infoJsonArrayStrBuf);
    infoJsonArrayWriter.StartArray();
    infoJsonArrayWriter.String(name.c_str());
    infoJsonArrayWriter.EndArray();

    dumper.add("info", RBytes(infoJsonArrayStrBuf.GetString()));
    dumper.add("index", RBytes(indexObjectStrBuf.GetString()));
    dumper.add("resourceIndex", RBytes(resourceIndexObjectStrBuf.GetString()));

    dumper.dump(filepath);

    return true;
}

bool FileDocument::open(const std::string& filepath) {
    if (!byteSetFileLoader.load(filepath)) {
        log("Reader cannot load file.");
        return false;
    }
    itemCount = 0;

    RBytes indexData = byteSetFileLoader.read("index");
    RBytes infoData = byteSetFileLoader.read("info");
    RBytes resourceInfoData = byteSetFileLoader.read("resourceIndex");
    char indexBytes[indexData.size() + 1];
    char infoBytes[infoData.size() + 1];
    indexData.read_string(indexBytes);
    infoData.read_string(infoBytes);

    JsonDocument infoJsonArrayDocument, indexJsonArrayDocument;
    if(indexJsonArrayDocument.Parse(indexBytes).HasParseError()) { log("Parse index bytes error:" + indexData.to_string()); return false;}
    if(infoJsonArrayDocument.Parse(infoBytes).HasParseError()) {log("Parse info bytes error:" + infoData.to_string()); return false;}
    JsonObject indexJsonObject = indexJsonArrayDocument.GetObject();
    JsonArray infoJsonArray = infoJsonArrayDocument.GetArray();

    // load info
    name = infoJsonArray[0].GetString();

    // load sub file
    std::map<std::string, SubFile*> subfileMap;
    subs.clear();
    JsonObjectIterator end = indexJsonObject.MemberEnd();
    for (JsonObjectIterator it = indexJsonObject.MemberBegin(); it!=end; it++)
    {
        std::string subFileName = it->value.GetString();

        RBytes subFileData = byteSetFileLoader.read(subFileName);
        char subFileBytes[subFileData.size()+1];
        subFileData.read_string(subFileBytes);

        JsonDocument subFileJsonDocument;
        if (subFileJsonDocument.Parse(subFileBytes).HasParseError()) {log("Parse subfile bytes error");
            log(subFileBytes); return false;}
        JsonArray subFileJsonArray = subFileJsonDocument.GetArray();

        std::string sn = it->name.GetString();
        auto *subFile = new SubFile(sn);
        subfileMap.insert(std::pair<std::string, SubFile*>(sn, subFile));
        subs.emplace_back(subFile);
        unsigned int groupCount = subFileJsonArray.Size();
        for (int i = 0; i < groupCount; ++i) {
            JsonArray groupArray = subFileJsonArray[i].GetArray();
            auto *group = new FileItemGroup(groupArray[0].GetString());
            subFile->add_group(group);
            group->hide = groupArray[1].GetBool();
            JsonArray groupItemArray = groupArray[2].GetArray();

            unsigned int ic = groupItemArray.Size();
            for (int j = 0; j<ic; ++j) {
                JsonArray itemArray = groupItemArray[j].GetArray();
                auto *item = new FileItemObject(itemArray[0].GetString(),
                                                itemArray[1].GetString(),
                                                itemArray[3].GetString());
                item->year = itemArray[4].GetInt();
                item->month = itemArray[5].GetInt();
                item->day = itemArray[6].GetInt();
                item->type = (FileItemObject::TYPE) itemArray[2].GetInt();
                if (itemArray.Size() > 7) {
                    item->resName = itemArray[7].GetString();
                }
                group->add_item(item);
                itemCount++;
            }
        }
    }

    char resourceInfoString[resourceInfoData.size() + 1];
    resourceInfoData.read_string(resourceInfoString);
    JsonDocument resourceInfoJsonDocument;
    if (resourceInfoJsonDocument.Parse(resourceInfoString).HasParseError()) {
        log("Parse resource json string error.");
        return false;
    }

    JsonObject resourceJsonObject = resourceInfoJsonDocument.GetObject();
    end = resourceJsonObject.MemberEnd();
    for (JsonObjectIterator sfIterator = resourceJsonObject.MemberBegin(); sfIterator != end ; ++sfIterator) {
        std::string sn = sfIterator->name.GetString();
        SubFile *sf = subfileMap[sn];
        JsonObject subFileResourceJsonObject = sfIterator->value.GetObject();
        JsonObjectIterator resIterEnd = subFileResourceJsonObject.MemberEnd();
        for (JsonObjectIterator resIterator = subFileResourceJsonObject.MemberBegin(); resIterator != resIterEnd; ++resIterator) {
            std::string rn = resIterator->name.GetString();
            JsonArray rfinfo = resIterator->value.GetArray();
            sf->resourceSet.add(rn, byteSetFileLoader.get_reference(rfinfo[0].GetString()), (ResItem::ResType)rfinfo[1].GetInt());
        }
    }

    return true;
}

SubFile *FileDocument::get_subfile(int index) const {
    return subs[index];
}

int FileDocument::count_sub_file() const {
    return subs.size();
}

bool FileDocument::contain_subfile(SubFile *subFile) const {
    for (const auto &item : subs)
        if (item == subFile) {
            return true;
        }
    return false;
}

SubFileCounter FileDocument::count() {
    SubFileCounter counter;
    for (SubFile *subFile : subs) {
        GroupCounter groupCounter = subFile->count();
        counter.itemCounter = groupCounter.itemCounter + counter.itemCounter;
        counter.group = counter.group + groupCounter.group;
        counter.subFile++;

        auto end = subFile->resourceSet.res.end();
        for (auto iter = subFile->resourceSet.res.begin(); iter != end; ++iter) {
            counter.resource++;
            counter.resourceSize += iter->second.byteRef->get_size();
        }
    }
    return counter;
}

int FileDocument::index_of(SubFile* subFile) const {
    int index = 0;
    for (const auto& item : subs) {
        if (item == subFile) {
            return index;
        }
    }
    return -1;
}

bool FileDocument::remove_subfile(SubFile *subFile) {
    auto iter = subs.begin();
    auto end = subs.end();
    while (iter != end) {
        if (*iter==subFile) {
            subs.erase(iter);
            return true;
        }
        iter++;
    }
    return false;
}

RLib::RByteSetWriter FileDocument::dump() {
    RByteSetWriter writer;

    JsonStringBuffer indexObjectStrBuf;
    JsonStringWriter indexObjectWriter(indexObjectStrBuf);
    JsonStringBuffer resourceIndexObjectStrBuf;
    JsonStringWriter resourceIndexObjectWriter(resourceIndexObjectStrBuf);

    // dump items
    int subFileNumber = 0;
    const std::string subFilePrefix = "sf";
    const std::string resourceFilePrefix = "rs";

    indexObjectWriter.StartObject();
    resourceIndexObjectWriter.StartObject();
    for (SubFile *sub: subs) { // sub file loop
        if (sub == nullptr) continue;
        rapidjson::StringBuffer subFileStrBuf;
        rapidjson::Writer<rapidjson::StringBuffer> subFileArrayWriter(subFileStrBuf);
        subFileArrayWriter.StartArray();

        for (FileItemGroup *group : sub->get_groups()) {
            subFileArrayWriter.StartArray();
            subFileArrayWriter.String(group->name.c_str());
            subFileArrayWriter.Bool(group->hide);
            subFileArrayWriter.StartArray();

            for (FileItemObject *item : group->get_items()) {
                // normal
                subFileArrayWriter.StartArray();
                subFileArrayWriter.String(item->name.c_str());
                subFileArrayWriter.String(item->text.c_str());
                subFileArrayWriter.Int(item->type);
                subFileArrayWriter.String(item->source.c_str());
                subFileArrayWriter.Int(item->year);
                subFileArrayWriter.Int(item->month);
                subFileArrayWriter.Int(item->day);
                if (!item->resName.empty()) subFileArrayWriter.String(item->resName.c_str());

                subFileArrayWriter.EndArray();
            }
            subFileArrayWriter.EndArray();
            subFileArrayWriter.EndArray();
        }
        subFileArrayWriter.EndArray();

        // dump subfile
        std::string numberString = std::to_string(subFileNumber++);

        RBytes binary(subFileStrBuf.GetString());
        std::string sfn = subFilePrefix + numberString;
        indexObjectWriter.Key(sub->name.c_str());
        indexObjectWriter.String(sfn.c_str());
        writer.add(sfn, binary);

        if (!sub->resourceSet.res.empty()) {
            int rfnumber = 0;
            std::string rfnp = resourceFilePrefix + numberString;
            rfnp += "_";
            resourceIndexObjectWriter.Key(sub->name.c_str());
            resourceIndexObjectWriter.StartObject();
            auto endResIter = sub->resourceSet.res.end();
            for (auto resIter = sub->resourceSet.res.begin(); resIter != endResIter; resIter++) {
                std::string rfn = rfnp + std::to_string(rfnumber++);
                resourceIndexObjectWriter.Key(resIter->first.c_str());
                resourceIndexObjectWriter.String(rfn.c_str());
            }
            resourceIndexObjectWriter.EndObject();
        }
    }
    resourceIndexObjectWriter.EndObject();
    indexObjectWriter.EndObject();

    JsonStringBuffer infoJsonArrayStrBuf;
    JsonStringWriter infoJsonArrayWriter(infoJsonArrayStrBuf);
    infoJsonArrayWriter.StartArray();
    infoJsonArrayWriter.String(name.c_str());
    infoJsonArrayWriter.EndArray();

    writer.add("info", RBytes(infoJsonArrayStrBuf.GetString()));
    writer.add("index", RBytes(indexObjectStrBuf.GetString()));
    writer.add("resourceIndex", RBytes(resourceIndexObjectStrBuf.GetString()));

    return writer;
}

bool FileDocument::load(RByteSetReader reader) {
    RBytes indexData = reader.get("index");
    RBytes infoData = reader.get("info");
    RBytes resourceInfoData = reader.get("resourceIndex");
    char indexBytes[indexData.size() + 1];
    char infoBytes[infoData.size() + 1];
    indexData.read_string(indexBytes);
    infoData.read_string(infoBytes);

    JsonDocument infoJsonArrayDocument, indexJsonArrayDocument;
    if(indexJsonArrayDocument.Parse(indexBytes).HasParseError()) { log("Parse index bytes error:" + indexData.to_string()); return false;}
    if(infoJsonArrayDocument.Parse(infoBytes).HasParseError()) {log("Parse info bytes error:" + infoData.to_string()); return false;}
    JsonObject indexJsonObject = indexJsonArrayDocument.GetObject();
    JsonArray infoJsonArray = infoJsonArrayDocument.GetArray();

    // load info
    name = infoJsonArray[0].GetString();

    // load sub file
    std::map<std::string, SubFile*> subfileMap;
    subs.clear();
    JsonObjectIterator end = indexJsonObject.MemberEnd();
    for (JsonObjectIterator it = indexJsonObject.MemberBegin(); it!=end; it++)
    {
        std::string subFileName = it->value.GetString();

        RBytes subFileData = reader.get(subFileName);
        char subFileBytes[subFileData.size()+1];
        subFileData.read_string(subFileBytes);

        JsonDocument subFileJsonDocument;
        if (subFileJsonDocument.Parse(subFileBytes).HasParseError()) {log("Parse subfile bytes error");
            log(subFileBytes); return false;}
        JsonArray subFileJsonArray = subFileJsonDocument.GetArray();

        std::string sn = it->name.GetString();
        auto *subFile = new SubFile(sn);
        subfileMap.insert(std::pair<std::string, SubFile*>(sn, subFile));
        subs.emplace_back(subFile);
        unsigned int groupCount = subFileJsonArray.Size();
        for (int i = 0; i < groupCount; ++i) {
            JsonArray groupArray = subFileJsonArray[i].GetArray();
            auto *group = new FileItemGroup(groupArray[0].GetString());
            subFile->add_group(group);
            group->hide = groupArray[1].GetBool();
            JsonArray groupItemArray = groupArray[2].GetArray();

            unsigned int itemCount = groupItemArray.Size();
            for (int j = 0; j<itemCount; ++j) {
                JsonArray itemArray = groupItemArray[j].GetArray();
                auto *item = new FileItemObject(itemArray[0].GetString(),
                                                itemArray[1].GetString(),
                                                itemArray[3].GetString());
                item->year = itemArray[4].GetInt();
                item->month = itemArray[5].GetInt();
                item->day = itemArray[6].GetInt();
                item->type = (FileItemObject::TYPE) itemArray[2].GetInt();
                if (itemArray.Size() > 7) {
                    item->resName = itemArray[7].GetString();
                }
                group->add_item(item);
            }
        }
    }

    if (!resourceInfoData.empty()) {
        char resourceInfoBytes[resourceInfoData.size() + 1];
        resourceInfoData.read_string(resourceInfoBytes);
        JsonDocument resourceInfoJsonDocument;

        if (resourceInfoJsonDocument.Parse(resourceInfoBytes).HasParseError()) {
            log("Parse res info error");
            return false;
        }

        JsonObject resInfoJsonObject = resourceInfoJsonDocument.GetObject();
        JsonObjectIterator end1 = resInfoJsonObject.MemberEnd();
        for (JsonObjectIterator iter = resInfoJsonObject.begin(); iter != end1; iter++) {
            std::string sn = iter->name.GetString();
            std::string fn = iter->value.GetString();
            RLib::RBytes rsb = reader.get(fn);
        }
    }

    return true;
}

std::vector<SubFile *> FileDocument::get_subs() {
    return subs;
}

SubFile *FileDocument::get_subfile(ItemId id) const{
    return subs[id.subfile];
}

FileItemObject *FileDocument::at(ItemId id) const{
    return subs[id.subfile]->get_group(id.group)->get_item(id.item);
}

FileDocument::~FileDocument() {
    byteSetFileLoader.close();
}

FileDocument::FileDocument() = default;
