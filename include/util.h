#pragma once

#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <map>
#ifndef __DREDIKI_UTIL__
#define __DREDIKI_UTIL__

#define Convert(t,base) std::shared_ptr<t> to##t (){ \
    return std::dynamic_pointer_cast<t>(shared_from_this()); \
}

#define DConvert2(t) std::shared_ptr<t> to##t ();

#define Convert2(t,base) std::shared_ptr<t> base::to##t (){ \
    return std::dynamic_pointer_cast<t>(shared_from_this()); \
}

#define message(format, args...) {\
	printf(format,##args);\
	printf("\033[0m\n");}

#define error(format, args...) { printf("\033[31m"); \
	printf(format,##args); \
	printf("\033[0m\n");} 

#define info(format, args...) { printf("\033[32m"); \
	printf(format,##args); \
	printf("\033[0m\n"); }

#define warning(format, args...) {\
	printf("\033[33m"); \
	printf(format,##args); \
	printf("\033[0m\n"); }

#ifdef DEBUG
#define debug(format, args...) {\
	printf("\033[34;47m"); \
	printf(format,##args); \
	printf("\033[0m\n"); }
#else
#define debug(format, args...) 
#endif

// http://blog.csdn.net/yozidream/article/details/22789147

typedef std::uint64_t hash_t;  
   
constexpr hash_t prime = 0x100000001B3ull;  
constexpr hash_t basis = 0xCBF29CE484222325ull;  
  
static hash_t _hash(char const* str)  
{  
    hash_t ret{basis};  
   
    while(*str){  
        ret ^= *str;  
        ret *= prime;  
        str++;  
    }  
   
    return ret;  
}  

static constexpr hash_t hash_compile_time(char const* str, hash_t last_value = basis)  
{  
    return *str ? hash_compile_time(str+1, (*str ^ last_value) * prime) : last_value;  
}

static constexpr unsigned long long operator "" _hash(char const* p, size_t)  
{  
    return hash_compile_time(p);  
}

// https://stackoverflow.com/questions/236129/the-most-elegant-way-to-iterate-the-words-of-a-string

static std::vector<std::string> split(const std::string &text, char sep) {
    std::vector<std::string> tokens;
    std::size_t start = 0, end = 0;
    while ((end = text.find(sep, start)) != std::string::npos) {
        if (end != start) {
          tokens.push_back(text.substr(start, end - start));
        }
        start = end + 1;
    }
    if (end != start) {
       tokens.push_back(text.substr(start));
    }
    return tokens;
}

template<typename Out>
static void split(const std::string &s, char delim, Out result) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
		if (item.empty())continue;
        *(result++) = item;
    }
}
/**
static std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}**/

#endif