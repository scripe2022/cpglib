#ifndef DEBUG_PRINT_H
#define DEBUG_PRINT_H

#include <iostream>
#include <tuple>
#include <vector>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <queue>
#include <stack>
#include <bitset>
#include <ext/pb_ds/assoc_container.hpp>
#include <boost/pfr.hpp>

#define INT32 0
#define INT64 1
#define UINT32 2
#define UINT64 3
#define FLOAT 4
#define DOUBLE 5
#define LONGDOUBLE 6
#define CHAR 7
#define CSTR 8
#define STRING 9
#define BOOL 10
#define PAIR 11
#define TUPLE 12
#define VECTOR 13
#define SET 14
#define UNORDEREDSET 15
#define MAP 16
#define UNORDEREDMAP 17
#define QUEUE 18
#define STACK 19
#define PRIORITYQUEUE 20
#define PBDS 21
#define MULTISET 22
#define BITSET 23
#define STRUCTURE 24

std::string _split_name(std::string &args_name);
std::string _combine(const int32_t var_type, const std::string &var_value);

template<typename T, typename V> std::string _print(const std::pair<T, V> &var);
template<typename... Args> std::string _print(const std::tuple<Args...> &var);
template<typename T> std::string _print(const std::vector<T> &var);
template<typename T, class C> std::string _print(const std::set<T, C> &var);
template<typename T, class C> std::string _print(const std::unordered_set<T, C> &var);
template<typename T, typename V, class C> std::string _print(const std::map<T, V, C> &var);
template<typename T, typename V, class C> std::string _print(const std::unordered_map<T, V, C> &var);
template<typename T> std::string _print(const std::queue<T> &var);
template<typename T> std::string _print(const std::stack<T> &var);
template<typename T, typename C, typename comp> std::string _print(const std::priority_queue<T, C, comp> &var);
template<typename T, class C> std::string _print(const __gnu_pbds::tree<T, __gnu_pbds::null_type, C, __gnu_pbds::rb_tree_tag, __gnu_pbds::tree_order_statistics_node_update> &var);
template<typename T, class C> std::string _print(const std::multiset<T, C> &var);
template<size_t T> std::string _print(const std::bitset<T> &var);
template<typename T> std::string _print(const T &var);

std::string _print(const int32_t &var);
std::string _print(const long long &var);
std::string _print(const unsigned &var);
std::string _print(const unsigned long &var);
std::string _print(const unsigned long long &var);
std::string _print(const float &var);
std::string _print(const double &var);
std::string _print(const long double &var);
std::string _print(const char &var);
std::string _print(const char *var);
std::string _print(const std::string &var);
std::string _print(const bool &var);
std::string _print(const std::_Bit_reference &var);

// pair
template<typename T, typename V>
std::string _print(const std::pair<T, V> &var) {
    std::string first = _print(var.first);
    std::string second = _print(var.second);
    return _combine(PAIR, "[" + first + "," + second + "]");
}

// tuple
template<typename... Args>
std::string _print(const std::tuple<Args...> &var) {
    std::string s = "[ ";
    std::apply([&s](const auto &&... args) { ((s += _print(args) + ","), ...); }, var);
    s.pop_back();
    s += "]";
    return _combine(TUPLE, s);
}

// vector
template<typename T>
std::string _print(const std::vector<T> &var) {
    std::string s = "[ ";
    for (const auto &i: var) s += _print(i) + ",";
    s.pop_back();
    s += "]";
    return _combine(VECTOR, s);
}

// set
template<typename T, class C>
std::string _print(const std::set<T, C> &var) {
    std::string s = "[ ";
    for (const auto &i: var) s += _print(i) + ",";
    s.pop_back();
    s += "]";
    return _combine(SET, s);
}

// unordered set
template<typename T, class C>
std::string _print(const std::unordered_set<T, C> &var) {
    std::string s = "[ ";
    for (const auto &i: var) s += _print(i) + ",";
    s.pop_back();
    s += "]";
    return _combine(UNORDEREDSET, s);
}

// map
template<typename T, typename V, class C>
std::string _print(const std::map<T, V, C> &var) {
    std::string s = "[ ";
    for (const auto &[i, j]: var) {
        std::string key = _print(i);
        std::string value = _print(j);
        s += "[" + key + "," + value + "],";
    }
    s.pop_back();
    s += "]";
    return _combine(MAP, s);
}

// unordered map
template<typename T, typename V, class C>
std::string _print(const std::unordered_map<T, V, C> &var) {
    std::string s = "[ ";
    for (const auto &[i, j]: var) {
        std::string key = _print(i);
        std::string value = _print(j);
        s += "[" + key + "," + value + "],";
    }
    s.pop_back();
    s += "]";
    return _combine(UNORDEREDMAP, s);
}

// queue
template<typename T>
std::string _print(const std::queue<T> &var) {
    std::queue<T> tmp = var;
    std::string s = "[ ";
    while (!tmp.empty()) {
        s += _print(tmp.front()) + ",";
        tmp.pop();
    }
    s.pop_back();
    s += "]";
    return _combine(QUEUE, s);
}

// stack
template<typename T>
std::string _print(const std::stack<T> &var) {
    std::stack<T> tmp = var;
    std::string s = "[ ";
    while (!tmp.empty()) {
        s += _print(tmp.top()) + ",";
        tmp.pop();
    }
    s.pop_back();
    s += "]";
    return _combine(QUEUE, s);
}

// priority queue
template<typename T, typename C, typename comp>
std::string _print(const std::priority_queue<T, C, comp> &var) {
    std::priority_queue<T, C, comp> tmp = var;
    std::string s = "[ ";
    while (!tmp.empty()) {
        s += _print(tmp.top()) + ",";
        tmp.pop();
    }
    s.pop_back();
    s += "]";
    return _combine(QUEUE, s);
}

// pbds
template <typename T, class C>
std::string _print(const __gnu_pbds::tree<T, __gnu_pbds::null_type, C, __gnu_pbds::rb_tree_tag, __gnu_pbds::tree_order_statistics_node_update> &var) {
    std::string s = "[ ";
    for (const auto &i: var) s += _print(i) + ",";
    s.pop_back();
    s += "]";
    return _combine(PBDS, s);
}

// multiset
template<typename T, class C>
std::string _print(const std::multiset<T, C> &var) {
    std::string s = "[ ";
    for (const auto &i: var) s += _print(i) + ",";
    s.pop_back();
    s += "]";
    return _combine(MULTISET, s);
}

// bitset
template <size_t T>
std::string _print(const std::bitset<T> &var) {
    std::string s = var.to_string();
    std::string h;
    char ch[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    for (int i = 0; i < (int)T; i += 4) {
        int n = 0;
        for (int j = std::min(i+4, (int)T) - 1; j >= i; --j) n = (n << 1) + var[j];
        h = ch[n] + h;
    }
    s = "\"" + s + " 0x" + h + "\"";
    return _combine(BITSET, s);
}

// struct
template<typename T>
std::string _print(const T &var) {
    std::string s = "[ ";
    boost::pfr::for_each_field(var, [&](const auto &&t) {
        s += _print(t) + ",";
    });
    s.pop_back();
    s += "]";
    return _combine(STRUCTURE, s);
}

// mode
// 0: inline
// 1: block
// 2: json
void _preprocess(const int32_t mode, std::vector<std::pair<std::string, std::string>> &args_list, std::string args_name);
template<typename T, typename... Args>
void _preprocess(const int32_t mode, std::vector<std::pair<std::string, std::string>> &args_list, std::string args_name, const T var, const Args... args) {
    args_list.emplace_back(make_pair(_split_name(args_name), _print(var)));
    _preprocess(mode, args_list, args_name, args...);
}

template<typename... Args>
void _debug_print(const int32_t mode, const std::string args_name, const Args... args) {
    std::vector<std::pair<std::string, std::string>> args_list;
    _preprocess(mode, args_list, args_name, args...);
}

#endif

