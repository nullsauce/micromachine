//
// Created by fla on 17-5-21.
//

#ifndef MICROMACHINE_STRINGPRINTF_HPP
#define MICROMACHINE_STRINGPRINTF_HPP

#include <memory>
#include <iostream>
#include <string>
#include <cstdio>

template<typename ... Args>
std::string string_format( const std::string& format, Args ... args ){
    size_t size = snprintf(nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    std::unique_ptr<char[]> buf( new char[ size ] );
	std::snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}

#endif //MICROMACHINE_STRINGPRINTF_HPP
