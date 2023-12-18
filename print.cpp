#include "print.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <filesystem>

#define COLORPRINT(x, d) std::cerr<<FORE_COLOR_LIST[(d)]<<(x)<<COLOR_SUFFIX
#define PADDING(x) for(int i_=0;i_<(4*(x));++i_)std::cerr<<" ";

const std::string COLOR_PREFIX = "\e[41m";
const std::string COLOR_NAME_PREFIX = "\e[45m";
const std::string COLOR_SUFFIX = "\e[0m";
const std::string BACK_COLOR_LIST[] = {"", "\033[1;32m", "\033[1;34m", "\033[1;39m", "\033[1;37m"};
const std::string FORE_COLOR_LIST[] = {"\033[1;31m", "\033[1;32m", "\033[1;33m", "\033[1;35m", "\033[1;36m"};

std::string add_single_quotes(const std::string &s) {return "\'" + s + "\'"; }
std::string add_double_quotes(const std::string &s) {return "\"" + s + "\""; }

std::string _split_name(std::string &args_name) {
    size_t pos = args_name.find(", ");
    if (pos == std::string::npos) {
        std::string rtn = args_name;
        args_name.clear();
        return rtn;
    }
    std::string rtn = args_name.substr(0, pos);
    args_name.erase(0, pos + 2);
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

void print_inline(const nlohmann::json obj) {
    int32_t var_type = obj["type"];
    nlohmann::json var = obj["value"];
    switch (var_type) {
        case INT32:
            std::cerr << var.get<int32_t>();
            break;
        case INT64:
            std::cerr << var.get<int64_t>();
            break;
        case UINT32:
            std::cerr << var.get<uint32_t>();
            break;
        case UINT64:
            std::cerr << var.get<uint64_t>();
            break;
        case FLOAT:
            std::cerr << var.get<float>();
            break;
        case DOUBLE:
            std::cerr << var.get<double>();
            break;
        case LONGDOUBLE:
            std::cerr << var.get<long double>();
            break;
        case CHAR:
            std::cerr << "\'" << var.get<std::string>() << "\'";
            break;
        case CSTR:
            std::cerr << "\"" << var.get<std::string>() << "\"";
            break;
        case STRING:
            std::cerr << "\"" << var.get<std::string>() << "\"";
            break;
        case BOOL:
            std::cerr << (var.get<bool>() ? "true" : "false");
            break;
        case PAIR:
            std::cerr << "{";
            print_inline(var[0]);
            std::cerr << ", ";
            print_inline(var[1]);
            std::cerr << "}";
            break;
        case TUPLE:
            std::cerr << "{";
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                print_inline(var[i]);
                if (i + 1 != (int32_t)var.size()) std::cerr << ", ";
            }
            std::cerr << "}";
            break;
        case VECTOR:
            std::cerr << "[";
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                print_inline(var[i]);
                if (i + 1 != (int32_t)var.size()) std::cerr << ", ";
            }
            std::cerr << "]";
            break;
        case SET:
            std::cerr << "{";
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                print_inline(var[i]);
                if (i + 1 != (int32_t)var.size()) std::cerr << ", ";
            }
            std::cerr << "}";
            break;
        case UNORDEREDSET:
            std::cerr << "{";
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                print_inline(var[i]);
                if (i + 1 != (int32_t)var.size()) std::cerr << ", ";
            }
            std::cerr << "}";
            break;
        case MAP:
            std::cerr << "{";
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                print_inline(var[i][0]);
                std::cerr << ": ";
                print_inline(var[i][1]);
                if (i != (int32_t)var.size() - 1) std::cerr << ", ";
            }
            std::cerr << "}";
            break;
        case UNORDEREDMAP:
            std::cerr << "{";
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                print_inline(var[i][0]);
                std::cerr << ": ";
                print_inline(var[i][1]);
                if (i + 1 != (int32_t)var.size()) std::cerr << ", ";
            }
            std::cerr << "}";
            break;
        case QUEUE:
            std::cerr << "[";
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                print_inline(var[i]);
                if (i + 1 != (int32_t)var.size()) std::cerr << ", ";
            }
            std::cerr << "]";
            break;
        case STACK:
            std::cerr << "[";
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                print_inline(var[i]);
                if (i + 1 != (int32_t)var.size()) std::cerr << ", ";
            }
            std::cerr << "]";
            break;
        case PRIORITYQUEUE:
            std::cerr << "[";
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                print_inline(var[i]);
                if (i + 1 != (int32_t)var.size()) std::cerr << ", ";
            }
            std::cerr << "]";
            break;
        case PBDS:
            std::cerr << "[";
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                print_inline(var[i]);
                if (i + 1 != (int32_t)var.size()) std::cerr << ", ";
            }
            std::cerr << "]";
            break;
        case MULTISET:
            std::cerr << "{";
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                print_inline(var[i]);
                if (i + 1 != (int32_t)var.size()) std::cerr << ", ";
            }
            std::cerr << "}";
            break;
        case STRUCTURE:
            std::cerr << "{";
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                print_inline(var[i]);
                if (i + 1 != (int32_t)var.size()) std::cerr << ", ";
            }
            std::cerr << "}";
            break;
        default:
            break;
    }
}

void process_inline(std::vector<std::pair<std::string, std::string>> &args_list) {
    for (int i = 0; i < (int32_t)args_list.size(); ++i) {
        std::string var_name = args_list[i].first;
        nlohmann::json obj = nlohmann::json::parse(args_list[i].second);

        std::cerr << COLOR_NAME_PREFIX << " " << var_name << " " << COLOR_SUFFIX;
        std::cerr << " = ";
        std::cerr << COLOR_PREFIX << " ";
        print_inline(obj);
        std::cerr << " " << COLOR_SUFFIX;
        std::cerr << (i + 1 == (int32_t)args_list.size() ? "\n" : " ,  ");
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
            else std::cerr << var.get<int32_t>();
            break;
        case INT64:
            if (colored) COLORPRINT(var.get<int64_t>(), depth);
            else std::cerr << var.get<int64_t>();
            break;
        case UINT32:
            if (colored) COLORPRINT(var.get<uint32_t>(), depth);
            else std::cerr << var.get<uint32_t>();
            break;
        case UINT64:
            if (colored) COLORPRINT(var.get<uint64_t>(), depth);
            else std::cerr << var.get<uint64_t>();
            break;
        case FLOAT:
            if (colored) COLORPRINT(var.get<float>(), depth);
            else std::cerr << var.get<float>();
            break;
        case DOUBLE:
            if (colored) COLORPRINT(var.get<double>(), depth);
            else std::cerr << var.get<double>();
            break;
        case LONGDOUBLE:
            if (colored) COLORPRINT(var.get<long double>(), depth);
            else std::cerr << var.get<long double>();
            break;
        case CHAR:
            if (colored) {
                COLORPRINT("\'", depth);
                COLORPRINT(var.get<std::string>(), depth);
                COLORPRINT("\'", depth);
            }
            else std::cerr << "\'" << var.get<std::string>() << "\'";
            break;
        case CSTR:
            if (colored) {
                COLORPRINT("\"", depth);
                COLORPRINT(var.get<std::string>(), depth);
                COLORPRINT("\"", depth);
            }
            else std::cerr << "\"" << var.get<std::string>() << "\"";
            break;
        case STRING:
            if (colored) {
                COLORPRINT("\"", depth);
                COLORPRINT(var.get<std::string>(), depth);
                COLORPRINT("\"", depth);
            }
            else std::cerr << "\"" << var.get<std::string>() << "\"";
            break;
        case BOOL:
            if (colored) COLORPRINT(var.get<bool>() ? "true" : "false", depth);
            else std::cerr << (var.get<bool>() ? "true" : "false");
            break;
        case PAIR:
            COLORPRINT("pair {\n", depth);
            PADDING(depth + 1); COLORPRINT("first : ", depth); print_block(var[0], depth + 1, false);
            if (is_basic(var[0])) std::cerr << std::endl;
            std::cerr << std::endl;
            PADDING(depth + 1); COLORPRINT("second: ", depth); print_block(var[1], depth + 1, false);
            if (is_basic(var[1])) std::cerr << std::endl;
            COLORPRINT("}", depth);
            std::cerr << std::endl;
            break;
        case TUPLE:
            COLORPRINT("tuple [\n", depth);
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                PADDING(depth + 1);
                print_block(var[i], depth + 1, false);
                if (is_basic(var[i])) std::cerr << std::endl;
            }
            PADDING(depth); COLORPRINT("}", depth);
            std::cerr << std::endl;
            break;
        case VECTOR:
            COLORPRINT("vector [\n", depth);
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                PADDING(depth + 1);
                COLORPRINT(i, depth); COLORPRINT(": ", depth);
                print_block(var[i], depth + 1, false);
                if (is_basic(var[i])) std::cerr << std::endl;
            }
            PADDING(depth); COLORPRINT("]", depth);
            std::cerr << std::endl;
            break;
        case SET:
            COLORPRINT("set {\n", depth);
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                PADDING(depth + 1);
                COLORPRINT(i, depth); COLORPRINT(": ", depth);
                print_block(var[i], depth + 1, false);
                if (is_basic(var[i])) std::cerr << std::endl;
            }
            PADDING(depth); COLORPRINT("]", depth);
            std::cerr << std::endl;
            break;
        case UNORDEREDSET:
            COLORPRINT("unordered set {\n", depth);
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                PADDING(depth + 1);
                COLORPRINT(i, depth); COLORPRINT(": ", depth);
                print_block(var[i], depth + 1, false);
                if (is_basic(var[i])) std::cerr << std::endl;
            }
            PADDING(depth); COLORPRINT("]", depth);
            std::cerr << std::endl;
            break;
        case MAP:
            COLORPRINT("map {\n", depth);
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                PADDING(depth + 1);
                print_block(var[i][0], depth, true);
                COLORPRINT(": ", depth);
                print_block(var[i][1], depth, false);
                if (is_basic(var[i][1])) std::cerr << std::endl;
            }
            PADDING(depth); COLORPRINT("}", depth);
            std::cerr << std::endl;
            break;
        case UNORDEREDMAP:
            COLORPRINT("unordered map {\n", depth);
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                PADDING(depth + 1);
                print_block(var[i][0], depth, true);
                COLORPRINT(": ", depth);
                print_block(var[i][1], depth, false);
                if (is_basic(var[i][1])) std::cerr << std::endl;
            }
            PADDING(depth); COLORPRINT("}", depth);
            std::cerr << std::endl;
            break;
        case QUEUE:
            COLORPRINT("queue [\n", depth);
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                PADDING(depth + 1);
                COLORPRINT(i, depth); COLORPRINT(": ", depth);
                print_block(var[i], depth + 1, false);
                if (is_basic(var[i])) std::cerr << std::endl;
            }
            PADDING(depth); COLORPRINT("]", depth);
            std::cerr << std::endl;
            break;
        case STACK:
            COLORPRINT("stack [\n", depth);
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                PADDING(depth + 1);
                COLORPRINT(i, depth); COLORPRINT(": ", depth);
                print_block(var[i], depth + 1, false);
                if (is_basic(var[i])) std::cerr << std::endl;
            }
            PADDING(depth); COLORPRINT("]", depth);
            std::cerr << std::endl;
            break;
        case PRIORITYQUEUE:
            COLORPRINT("priority_queue [\n", depth);
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                PADDING(depth + 1);
                COLORPRINT(i, depth); COLORPRINT(": ", depth);
                print_block(var[i], depth + 1, false);
                if (is_basic(var[i])) std::cerr << std::endl;
            }
            PADDING(depth); COLORPRINT("]", depth);
            std::cerr << std::endl;
            break;
        case PBDS:
            COLORPRINT("pbds [\n", depth);
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                PADDING(depth + 1);
                COLORPRINT(i, depth); COLORPRINT(": ", depth);
                print_block(var[i], depth + 1, false);
                if (is_basic(var[i])) std::cerr << std::endl;
            }
            PADDING(depth); COLORPRINT("]", depth);
            std::cerr << std::endl;
            break;
        case MULTISET:
            COLORPRINT("multiset {\n", depth);
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                PADDING(depth + 1);
                COLORPRINT(i, depth); COLORPRINT(": ", depth);
                print_block(var[i], depth + 1, false);
                if (is_basic(var[i])) std::cerr << std::endl;
            }
            PADDING(depth); COLORPRINT("}", depth);
            std::cerr << std::endl;
            break;
        case STRUCTURE:
            COLORPRINT("struct {\n", depth);
            for (int i = 0; i < (int32_t)var.size(); ++i) {
                PADDING(depth + 1);
                print_block(var[i], depth + 1, false);
                if (is_basic(var[i])) std::cerr << std::endl;
            }
            PADDING(depth); COLORPRINT("}", depth);
            std::cerr << std::endl;
            break;
        default:
            break;
    }
}

void process_block(std::vector<std::pair<std::string, std::string>> &args_list) {
    for (int i = 0; i < (int32_t)args_list.size(); ++i) {
        std::string var_name = args_list[i].first;
        nlohmann::json obj = nlohmann::json::parse(args_list[i].second);

        std::cerr << COLOR_NAME_PREFIX << " " << var_name << " " << COLOR_SUFFIX << " =" << std::endl;
        print_block(obj, 0, false);
        if (is_basic(obj)) std::cerr << std::endl;
        if (i + 1 == (int32_t)args_list.size()) std::cerr << std::endl;
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
