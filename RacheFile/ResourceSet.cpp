//
// Created by Rache on 2021/10/24.
//

#include "ResourceSet.h"

#include "RByteSet/RByteSet.h"
#include "rapidjson/writer.h"
#include "rapidjson/document.h"

typedef rapidjson::Document JsonDocument;
typedef rapidjson::Value::Array JsonArray;
typedef rapidjson::Value::Object JsonObject;
typedef rapidjson::Value::Object::MemberIterator JsonObjectIterator;
typedef rapidjson::StringBuffer JsonStringBuffer;
typedef rapidjson::Writer<JsonStringBuffer> JsonStringWriter;

ResItem ResourceSet::add(const std::string& name, char *bytes, int size, ResItem::ResType type) {
    auto *r = new RByteSetRef;
    r->loaded = true;
    r->bytes.write(bytes, size);
    ResItem i;
    i.name = name;
    i.byteRef = r;
    i.type = type;
    res.insert(std::pair<std::string, ResItem>(name, i));
    return i;
}

ResItem ResourceSet::get(const std::string& name) {
    auto iter = res.find(name);
    if (iter == res.end()) return ResItem();
    return iter->second;
}

bool ResourceSet::remove(const std::string& name) {
    auto iter = res.find(name);
    if (iter == res.end()) return false;
    res.erase(iter);
    delete iter->second.byteRef;
    return true;
}

ResourceSet::ResourceSet() = default;

ResItem ResourceSet::get(const string &name, bool &hasError) {
    auto iter = res.find(name);
    if (iter == res.end()) {
        hasError = true;
        return ResItem();
    } else {
        hasError = false;
        return iter->second;
    }
}

ResItem ResourceSet::add(const string &name, RLib::RBytes r, ResItem::ResType type) {
    ResItem i;
    i.byteRef = new RByteSetRef;
    i.name = name;
    i.byteRef->bytes = std::move(r);
    i.byteRef->loaded = true;
    i.type = type;
    res.insert(std::pair<std::string, ResItem>(name, i));
    return i;
}

ResItem ResourceSet::add(const string &name, RByteSetRef *ref, ResItem::ResType type) {
    ResItem resItem;
    resItem.byteRef = ref;
    resItem.type = type;
    resItem.name = name;
    res.insert(std::pair<std::string, ResItem>(name, resItem));
    return resItem;
}
