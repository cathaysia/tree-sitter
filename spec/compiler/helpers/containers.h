#ifndef HELPERS_CONTAINERS_H_
#define HELPERS_CONTAINERS_H_

#include <map>
#include <vector>
#include <string>
#include <initializer_list>
#include "tree_sitter/compiler.h"
#include "compiler/rules/rule.h"

using std::map;
using std::vector;
using std::string;
using std::initializer_list;
using std::pair;
using tree_sitter::rules::rule_ptr;

template<typename K>
class rule_map : public map<K, rule_ptr> {
public:
    bool operator==(const map<K, rule_ptr> &other) const {
        if (this->size() != other.size()) return false;
        for (const auto &pair : *this) {
            auto other_pair = other.find(pair.first);
            if (other_pair == other.end()) return false;
            if (!pair.second->operator==(*other_pair->second)) return false;
        }
        return true;
    }

    rule_map(const initializer_list<pair<const K, rule_ptr>> &list) : map<K, rule_ptr>(list) {}
};

class rule_list : public vector<pair<string, rule_ptr>> {
public:
    bool operator==(const vector<pair<string, rule_ptr>> &other) const {
        if (this->size() != other.size()) return false;
        for (size_t i = 0; i < this->size(); i++) {
            auto pair = this->operator[](i);
            auto other_pair = other[i];
            if (!pair.second->operator==(*other_pair.second))
                return false;
        }
        return true;
    }

    rule_list(const initializer_list<pair<string, rule_ptr>> &list) : 
        vector<pair<string, rule_ptr>>(list) {}
};


#endif  // HELPERS_CONTAINERS_H_
