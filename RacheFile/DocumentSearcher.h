//
// Created by Rache on 2021/10/23.
//

#ifndef RACHEFILE_DOCUMENTSEARCHER_H
#define RACHEFILE_DOCUMENTSEARCHER_H

#include "FileDocument.h"
#include <string>
#include <vector>

enum SearchType{
    ALL,
    CONTAIN,
    EQUAL,
    START_WITH,
    END_WITH,
};

struct SearchResult {
    SubFile *subFile        = nullptr;
    FileItemGroup *group    = nullptr;
    FileItemObject *item    = nullptr;
};

class DocumentSearcher {
public:
    explicit DocumentSearcher(FileDocument *d);

    void set_subfile_search(SearchType t, std::string s);
    void set_group_search(SearchType t, std::string s);
    void set_item_search(SearchType t, std::string s);
    std::vector<SearchResult> search(double *t = nullptr);

private:
    FileDocument *document;

    SearchType subFileSearchType;
    SearchType groupSearchType;
    SearchType itemSearchType;
    std::string subFileSearchString;
    std::string groupSearchString;
    std::string itemSearchString;

    static bool equal(const std::string& s1, const std::string& s2);
    static bool contain(const string& s1, const string& s2);
    static bool all(const std::string& s1, const std::string& s2);
    static bool start_with(const std::string& s1, const std::string& s2);
    static bool end_with(const std::string& s1, const std::string& s2);

    bool (*compareFunctions[5])(const string&, const string&) = {all, contain, equal, start_with, end_with};
};


#endif //RACHEFILE_DOCUMENTSEARCHER_H
