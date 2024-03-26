#include "print.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <filesystem>

#define COLORPRINT(x, d) std::cout<<FORE_COLOR_LIST[(d)]<<(x)<<COLOR_SUFFIX
#define PADDING(x) for(int i_=0;i_<(4*(x));++i_)std::cout<<" ";

const std::string COLOR_PREFIX = "\e[41m\e[97m";
const std::string COLOR_NAME_PREFIX = "\e[45m\e[97m";
const std::string COLOR_SUFFIX = "\e[0m";
const std::string BACK_COLOR_LIST[] = {"", "\033[1;32m", "\033[1;34m", "\033[1;39m", "\033[1;37m"};
const std::string FORE_COLOR_LIST[] = {"\033[1;31m", "\033[1;32m", "\033[1;33m", "\033[1;35m", "\033[1;36m"};

std::string add_single_quotes(const std::string &s) {return "\'" + s + "\'"; }
std::string add_double_quotes(const std::string &s) {return "\"" + s + "\""; }

std::string _split_name(std::string &args_name) {
    size_t start, end;
    for (start = 0; start < args_name.size() && args_name[start] == ' '; ++start);
    size_t curly = 0, bracket = 0, parenthese = 0;
    for (end = start; end < args_name.size(); ++end) {
        if (args_name[end] == '{') ++curly;
        else if (args_name[end] == '}') --curly;
        else if (args_name[end] == '[') ++bracket;
        else if (args_name[end] == ']') --bracket;
        else if (args_name[end] == '(') ++parenthese;
        else if (args_name[end] == ')') --parenthese;
        else if (args_name[end] == ',' && curly == 0 && bracket == 0 && parenthese == 0) break;
    }
    std::string rtn = args_name.substr(start, end - start);
    if (end == args_name.size()) args_name.clear();
    else args_name = args_name.substr(end + 1);
    return rtn;
}

std::string _combine(const int32_t var_type, const std::string &var_value) {
    std::string s = "{\"type\":" + std::to_string(var_type) + ",\"value\":" + var_value + "}";
    return s;
}

std::string _print(const int32_t &var) {return _combine(INT32, std::to_string(var)); }
std::string _print(const long long &var) {return _combine(INT64, std::to_string(var)); }
std::string _print(const unsigned &var) {return _combine(UINT32, std::to_string(var)); }
std::string _print(const unsigned long &var) {return _combine(UINT32, std::to_string(var)); }
std::string _print(const unsigned long long &var) {return _combine(UINT64, std::to_string(var)); }
std::string _print(const float &var) {return _combine(FLOAT, std::to_string(var)); }
std::string _print(const double &var) {return _combine(DOUBLE, std::to_string(var)); }
std::string _print(const long double &var) {return _combine(LONGDOUBLE, std::to_string(var)); }
std::string _print(const char &var) {std::string s(1, var); return _combine(CHAR, add_double_quotes(s)); }
std::string _print(const char *var) {return _combine(CSTR, add_double_quotes(var)); }
std::string _print(const std::string &var) {return _combine(STRING, add_double_quotes(var)); }
std::string _print(const bool &var) {return _combine(BOOL, var ? "true" : "false"); }
std::string _print(const std::_Bit_reference &var) {return _print(static_cast<bool>(var)); }

void print_inline(const nlohmann::json obj) {
    int32_t var_type = obj["type"];
    nlohmann::json var = obj["value"];
    switch (var_type) {
        case INT32:
            std::cout << var.get<int32_t>();
            break;
        case INT64:
            std::cout << var.get<int64_t>();
            break;
        case UINT32:
            std::cout << var.get<uint32_t>();
            break;
        case UINT64:
            std::cout << var.get<uint64_t>();
            break;
        case FLOAT:
            std::cout << var.get<float>();
            break;
        case DOUBLE:
            std::cout << var.get<double>();
            break;
        case LONGDOUBLE:
            std::cout << var.get<long double>();
            break;
        case CHAR:
            std::cout << "\'" << var.get<std::string>() << "\'";
            break;
        case CSTR:
            std::cout << "\"" << var.get<std::string>() << "\"";
            break;
        case STRING:
            std::cout << "\"" << var.get<std::string>() << "\"";
            break;
        case BOOL:
            std::cout << (var.get<bool>() ? "true" : "false");
            break;
        case PAIR:
            std::cout << "{";
            print_inline(var[0]);
            std::cout << ", ";
            print_inline(var[1]);
            std::cout << "}";
            break;
        case TUPLE:
            std::cout << "{";
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                print_inline(var[i]);
                if (i + 1 != (int32_t)var.size()) std::cout << ", ";
            }
            std::cout << "}";
            break;
        case VECTOR:
            std::cout << "[";
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                print_inline(var[i]);
                if (i + 1 != (int32_t)var.size()) std::cout << ", ";
            }
            std::cout << "]";
            break;
        case SET:
            std::cout << "{";
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                print_inline(var[i]);
                if (i + 1 != (int32_t)var.size()) std::cout << ", ";
            }
            std::cout << "}";
            break;
        case UNORDEREDSET:
            std::cout << "{";
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                print_inline(var[i]);
                if (i + 1 != (int32_t)var.size()) std::cout << ", ";
            }
            std::cout << "}";
            break;
        case MAP:
            std::cout << "{";
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                print_inline(var[i][0]);
                std::cout << ": ";
                print_inline(var[i][1]);
                if (i != (int32_t)var.size() - 1) std::cout << ", ";
            }
            std::cout << "}";
            break;
        case UNORDEREDMAP:
            std::cout << "{";
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                print_inline(var[i][0]);
                std::cout << ": ";
                print_inline(var[i][1]);
                if (i + 1 != (int32_t)var.size()) std::cout << ", ";
            }
            std::cout << "}";
            break;
        case QUEUE:
            std::cout << "[";
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                print_inline(var[i]);
                if (i + 1 != (int32_t)var.size()) std::cout << ", ";
            }
            std::cout << "]";
            break;
        case STACK:
            std::cout << "[";
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                print_inline(var[i]);
                if (i + 1 != (int32_t)var.size()) std::cout << ", ";
            }
            std::cout << "]";
            break;
        case PRIORITYQUEUE:
            std::cout << "[";
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                print_inline(var[i]);
                if (i + 1 != (int32_t)var.size()) std::cout << ", ";
            }
            std::cout << "]";
            break;
        case PBDS:
            std::cout << "[";
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                print_inline(var[i]);
                if (i + 1 != (int32_t)var.size()) std::cout << ", ";
            }
            std::cout << "]";
            break;
        case MULTISET:
            std::cout << "{";
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                print_inline(var[i]);
                if (i + 1 != (int32_t)var.size()) std::cout << ", ";
            }
            std::cout << "}";
            break;
        case BITSET:
            std::cout << var.get<std::string>();
            break;
        case STRUCTURE:
            std::cout << "{";
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                print_inline(var[i]);
                if (i + 1 != (int32_t)var.size()) std::cout << ", ";
            }
            std::cout << "}";
            break;
        default:
            break;
    }
}

void process_inline(std::vector<std::pair<std::string, std::string>> &args_list) {
    for (int i = 0; i < (int32_t)args_list.size(); ++i) {
        std::string var_name = args_list[i].first;
        nlohmann::json obj = nlohmann::json::parse(args_list[i].second);
        std::cout << COLOR_NAME_PREFIX << " " << var_name << " " << COLOR_SUFFIX;
        std::cout << " = ";
        std::cout << COLOR_PREFIX << " ";
        print_inline(obj);
        std::cout << " " << COLOR_SUFFIX;
        std::cout << (i + 1 == (int32_t)args_list.size() ? "\n" : " ,  ");
    }
}

bool is_basic(nlohmann::json obj) {
    int32_t var_type = obj["type"];
    return var_type <= 10;
}

void print_block(const nlohmann::json obj, const int32_t depth, const bool colored) {
    int32_t var_type = obj["type"];
    nlohmann::json var = obj["value"];
    switch (var_type) {
        case INT32:
            if (colored) COLORPRINT(var.get<int32_t>(), depth);
            else std::cout << var.get<int32_t>();
            break;
        case INT64:
            if (colored) COLORPRINT(var.get<int64_t>(), depth);
            else std::cout << var.get<int64_t>();
            break;
        case UINT32:
            if (colored) COLORPRINT(var.get<uint32_t>(), depth);
            else std::cout << var.get<uint32_t>();
            break;
        case UINT64:
            if (colored) COLORPRINT(var.get<uint64_t>(), depth);
            else std::cout << var.get<uint64_t>();
            break;
        case FLOAT:
            if (colored) COLORPRINT(var.get<float>(), depth);
            else std::cout << var.get<float>();
            break;
        case DOUBLE:
            if (colored) COLORPRINT(var.get<double>(), depth);
            else std::cout << var.get<double>();
            break;
        case LONGDOUBLE:
            if (colored) COLORPRINT(var.get<long double>(), depth);
            else std::cout << var.get<long double>();
            break;
        case CHAR:
            if (colored) {
                COLORPRINT("\'", depth);
                COLORPRINT(var.get<std::string>(), depth);
                COLORPRINT("\'", depth);
            }
            else std::cout << "\'" << var.get<std::string>() << "\'";
            break;
        case CSTR:
            if (colored) {
                COLORPRINT("\"", depth);
                COLORPRINT(var.get<std::string>(), depth);
                COLORPRINT("\"", depth);
            }
            else std::cout << "\"" << var.get<std::string>() << "\"";
            break;
        case STRING:
            if (colored) {
                COLORPRINT("\"", depth);
                COLORPRINT(var.get<std::string>(), depth);
                COLORPRINT("\"", depth);
            }
            else std::cout << "\"" << var.get<std::string>() << "\"";
            break;
        case BOOL:
            if (colored) COLORPRINT(var.get<bool>() ? "true" : "false", depth);
            else std::cout << (var.get<bool>() ? "true" : "false");
            break;
        case PAIR:
            COLORPRINT("pair {\n", depth);
            PADDING(depth + 1); COLORPRINT("first : ", depth); print_block(var[0], depth + 1, false);
            if (is_basic(var[0])) std::cout << std::endl;
            PADDING(depth + 1); COLORPRINT("second: ", depth); print_block(var[1], depth + 1, false);
            if (is_basic(var[1])) std::cout << std::endl;
            PADDING(depth); COLORPRINT("}", depth);
            std::cout << std::endl;
            break;
        case TUPLE:
            COLORPRINT("tuple [\n", depth);
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                PADDING(depth + 1);
                print_block(var[i], depth + 1, false);
                if (is_basic(var[i])) std::cout << std::endl;
            }
            PADDING(depth); COLORPRINT("}", depth);
            std::cout << std::endl;
            break;
        case VECTOR:
            COLORPRINT("vector [\n", depth);
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                PADDING(depth + 1);
                COLORPRINT(i, depth); COLORPRINT(": ", depth);
                print_block(var[i], depth + 1, false);
                if (is_basic(var[i])) std::cout << std::endl;
            }
            PADDING(depth); COLORPRINT("]", depth);
            std::cout << std::endl;
            break;
        case SET:
            COLORPRINT("set {\n", depth);
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                PADDING(depth + 1);
                COLORPRINT(i, depth); COLORPRINT(": ", depth);
                print_block(var[i], depth + 1, false);
                if (is_basic(var[i])) std::cout << std::endl;
            }
            PADDING(depth); COLORPRINT("]", depth);
            std::cout << std::endl;
            break;
        case UNORDEREDSET:
            COLORPRINT("unordered set {\n", depth);
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                PADDING(depth + 1);
                COLORPRINT(i, depth); COLORPRINT(": ", depth);
                print_block(var[i], depth + 1, false);
                if (is_basic(var[i])) std::cout << std::endl;
            }
            PADDING(depth); COLORPRINT("]", depth);
            std::cout << std::endl;
            break;
        case MAP:
            COLORPRINT("map {\n", depth);
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                PADDING(depth + 1);
                print_block(var[i][0], depth, true);
                COLORPRINT(": ", depth);
                print_block(var[i][1], depth, false);
                if (is_basic(var[i][1])) std::cout << std::endl;
            }
            PADDING(depth); COLORPRINT("}", depth);
            std::cout << std::endl;
            break;
        case UNORDEREDMAP:
            COLORPRINT("unordered map {\n", depth);
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                PADDING(depth + 1);
                print_block(var[i][0], depth, true);
                COLORPRINT(": ", depth);
                print_block(var[i][1], depth, false);
                if (is_basic(var[i][1])) std::cout << std::endl;
            }
            PADDING(depth); COLORPRINT("}", depth);
            std::cout << std::endl;
            break;
        case QUEUE:
            COLORPRINT("queue [\n", depth);
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                PADDING(depth + 1);
                COLORPRINT(i, depth); COLORPRINT(": ", depth);
                print_block(var[i], depth + 1, false);
                if (is_basic(var[i])) std::cout << std::endl;
            }
            PADDING(depth); COLORPRINT("]", depth);
            std::cout << std::endl;
            break;
        case STACK:
            COLORPRINT("stack [\n", depth);
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                PADDING(depth + 1);
                COLORPRINT(i, depth); COLORPRINT(": ", depth);
                print_block(var[i], depth + 1, false);
                if (is_basic(var[i])) std::cout << std::endl;
            }
            PADDING(depth); COLORPRINT("]", depth);
            std::cout << std::endl;
            break;
        case PRIORITYQUEUE:
            COLORPRINT("priority_queue [\n", depth);
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                PADDING(depth + 1);
                COLORPRINT(i, depth); COLORPRINT(": ", depth);
                print_block(var[i], depth + 1, false);
                if (is_basic(var[i])) std::cout << std::endl;
            }
            PADDING(depth); COLORPRINT("]", depth);
            std::cout << std::endl;
            break;
        case PBDS:
            COLORPRINT("pbds [\n", depth);
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                PADDING(depth + 1);
                COLORPRINT(i, depth); COLORPRINT(": ", depth);
                print_block(var[i], depth + 1, false);
                if (is_basic(var[i])) std::cout << std::endl;
            }
            PADDING(depth); COLORPRINT("]", depth);
            std::cout << std::endl;
            break;
        case MULTISET:
            COLORPRINT("multiset {\n", depth);
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                PADDING(depth + 1);
                COLORPRINT(i, depth); COLORPRINT(": ", depth);
                print_block(var[i], depth + 1, false);
                if (is_basic(var[i])) std::cout << std::endl;
            }
            PADDING(depth); COLORPRINT("}", depth);
            std::cout << std::endl;
            break;
        case BITSET: {
            std::string s = var.get<std::string>();
            std::istringstream iss(s);
            std::string bin, hex; iss >> bin >> hex;
            if (colored) {
                COLORPRINT(s, depth);
            }
            else {
                COLORPRINT("bitset {\n", depth);
                PADDING(depth + 1); COLORPRINT("binary: ", depth); std::cout << bin << std::endl;
                PADDING(depth + 1); COLORPRINT("hex   : ", depth); std::cout << hex << std::endl;
                PADDING(depth); COLORPRINT("}", depth);
                std::cout << std::endl;
            }
            break;
        }
        case STRUCTURE:
            COLORPRINT("struct {\n", depth);
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                PADDING(depth + 1);
                print_block(var[i], depth + 1, false);
                if (is_basic(var[i])) std::cout << std::endl;
            }
            PADDING(depth); COLORPRINT("}", depth);
            std::cout << std::endl;
            break;
        default:
            break;
    }
}

void process_block(std::vector<std::pair<std::string, std::string>> &args_list) {
    for (int i = 0; i < (int32_t)args_list.size(); ++i) {
        std::string var_name = args_list[i].first;
        nlohmann::json obj = nlohmann::json::parse(args_list[i].second);

        std::cout << COLOR_NAME_PREFIX << " " << var_name << " " << COLOR_SUFFIX << " =" << std::endl;
        print_block(obj, 0, false);
        if (is_basic(obj)) std::cout << std::endl;
        if (i + 1 == (int32_t)args_list.size()) std::cout << std::endl;
    }
}

nlohmann::json print_json(const nlohmann::json &obj) {
    int32_t var_type = obj["type"];
    nlohmann::json var = obj["value"];
    nlohmann::json rtn;
    switch (var_type) {
        case INT32: return var.get<int32_t>();
        case INT64: return var.get<int64_t>();
        case UINT32: return var.get<uint32_t>();
        case UINT64: return var.get<uint64_t>();
        case FLOAT: return var.get<float>();
        case DOUBLE: return var.get<double>();
        case LONGDOUBLE: return var.get<long double>();
        case CHAR: return var.get<std::string>();
        case CSTR: return var.get<std::string>();
        case STRING: return var.get<std::string>();
        case BOOL: return var.get<bool>();
        case PAIR:
            rtn["first"] = print_json(var[0]);
            rtn["second"] = print_json(var[1]);
            return rtn;
        case TUPLE:
            for (int i = 0; i < (int32_t)var.size(); ++i) rtn.push_back(print_json(var[i]));
            return rtn;
        case VECTOR:
            for (int i = 0; i < (int32_t)var.size(); ++i) rtn.push_back(print_json(var[i]));
            return rtn;
        case SET:
            for (int i = 0; i < (int32_t)var.size(); ++i) rtn.push_back(print_json(var[i]));
            return rtn;
        case UNORDEREDSET:
            for (int i = 0; i < (int32_t)var.size(); ++i) rtn.push_back(print_json(var[i]));
            return rtn;
        case MAP:
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                std::string key = print_json(var[i][0]).dump();
                if (key[0] == '\"') key = key.substr(1, (int32_t)(key.size() - 2));
                rtn[key] = print_json(var[i][1]);
            }
            return rtn;
        case UNORDEREDMAP:
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                std::string key = print_json(var[i][0]).dump();
                if (key[0] == '\"') key = key.substr(1, (int32_t)(key.size() - 2));
                rtn[key] = print_json(var[i][1]);
            }
            return rtn;
        case QUEUE:
            for (int i = 0; i < (int32_t)var.size(); ++i) rtn.push_back(print_json(var[i]));
            return rtn;
        case STACK:
            for (int i = 0; i < (int32_t)var.size(); ++i) rtn.push_back(print_json(var[i]));
            return rtn;
        case PRIORITYQUEUE:
            for (int i = 0; i < (int32_t)var.size(); ++i) rtn.push_back(print_json(var[i]));
            return rtn;
        case PBDS:
            for (int i = 0; i < (int32_t)var.size(); ++i) rtn.push_back(print_json(var[i]));
            return rtn;
        case MULTISET:
            for (int i = 0; i < (int32_t)var.size(); ++i) rtn.push_back(print_json(var[i]));
            return rtn;
        case BITSET:
            return var.get<std::string>();
        case STRUCTURE:
            for (int i = 0; i < (int32_t)var.size(); ++i) rtn.push_back(print_json(var[i]));
            return rtn;
        default:
            return rtn;
    }
}

void process_json(std::vector<std::pair<std::string, std::string>> &args_list) {
    std::filesystem::path dir("cppjson");
    if (!std::filesystem::exists(dir)) std::filesystem::create_directories(dir);
    for (int i = 0; i < (int32_t)args_list.size(); ++i) {
        std::string var_name = args_list[i].first;
        std::ofstream f("cppjson/" + var_name + ".json");
        nlohmann::json src = nlohmann::json::parse(args_list[i].second);
        nlohmann::json obj = print_json(src);
        f << obj.dump(4);
        f.close();
    }
}

void _preprocess(const int32_t mode, std::vector<std::pair<std::string, std::string>> &args_list, std::string args_name) {
    (void)args_name;
    if (mode == 0) process_inline(args_list);
    else if (mode == 1) process_block(args_list);
    else if (mode == 2) process_json(args_list);
}

