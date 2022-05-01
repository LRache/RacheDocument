//
// Created by Rache on 2021/10/23.
//

#include "DocumentSearcher.h"

#include <ctime>
#include <utility>

DocumentSearcher::DocumentSearcher(FileDocument *d) {
    document = d;
}

void DocumentSearcher::set_subfile_search(SearchType t, std::string s) {
    subFileSearchType = t;
    subFileSearchString = std::move(s);
}

void DocumentSearcher::set_group_search(SearchType t, std::string s) {
    groupSearchType = t;
    groupSearchString = std::move(s);
}

void DocumentSearcher::set_item_search(SearchType t, std::string s) {
    itemSearchType = t;
    itemSearchString = std::move(s);
}

std::vector<SearchResult> DocumentSearcher::search(double *t) {
    bool (*subFileComFun)(const string&, const string&) = compareFunctions[subFileSearchType];
    bool (*groupComFun)(const string&, const string&)   = compareFunctions[groupSearchType];
    bool (*itemComFun)(const string&, const string&)    = compareFunctions[itemSearchType];

    double start;
    if (t != nullptr) {
        start = clock();
    }
    std::vector<SearchResult> results;
    for (auto subFile : document->get_subs()) {
        if (subFile == nullptr) continue;
        if (subFileComFun(subFile->name, subFileSearchString)) {
            for (auto group : subFile->get_groups()) {
                if (group == nullptr) continue;
                if (groupComFun(group->name, groupSearchString)) {
                    for (auto item : group->get_items()) {
                        if (item == nullptr) continue;
                        if (itemComFun(item->name, itemSearchString)) {
                            SearchResult result;
                            result.subFile  = subFile;
                            result.group    = group;
                            result.item     = item;
                            results.emplace_back(result);
                        }
                    }
                }
            }
        }
    }
    if (t != nullptr) {
        double end = clock();
        *t = end - start;
    }

    return results;
}

bool DocumentSearcher::equal(const std::string& s1, const std::string& s2) {
    return s1 == s2;
}

bool DocumentSearcher::contain(const string& s1, const string& s2) {
    return s1.find(s2) != std::string::npos;
}

bool DocumentSearcher::all(const std::string& s1, const std::string& s2) {
    return true;
}

bool DocumentSearcher::start_with(const string &s1, const string &s2) {
    return s1.find(s2) == 0;
}

bool DocumentSearcher::end_with(const string &s1, const string &s2) {
    if (s1.size() < s2.size()) return false;
    return s1.compare(s1.size() - s2.size(), s2.size(), s2) == 0;
}
