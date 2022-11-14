#ifndef LOG__H__
#define LOG__H__

#include <Windows.h>
#include <iostream>
#include <sstream>
#include <string.h>

class Logger
{
public:
    template<typename... TArgs>
    static void Error(TArgs&... _mArgs);
    template<typename... TArgs>
    static void Warning(TArgs&... _mArgs);
    template<typename... TArgs>
    static void Debug(TArgs&... _mArgs);
    template<typename... TArgs>
    static void Info(TArgs&... _mArgs);


    template<typename... TArgs>
    static void LogText(const char* type, TArgs&... _mArgs);
};

template<typename... TArgs>
static void ::Logger::LogText(const char* type, TArgs&... _mArgs)
{
    std::wostringstream os_;
    os_ << "[" << type << "] => ";
    ([&]
    {
        os_ <<std::string(_mArgs).c_str();
    }(), ...);
    os_ << std::endl;
    OutputDebugStringW(os_.str().c_str());
}

template<typename... TArgs>
static void Logger::Error(TArgs&... _mArgs)
{
    LogText("ERROR", _mArgs...);
}

template<typename... TArgs>
static void Logger::Warning(TArgs&... _mArgs)
{
    LogText("WARNING", _mArgs...);
}

template<typename... TArgs>
static void Logger::Debug(TArgs&... _mArgs)
{
    LogText("DEBUG", _mArgs...);
}

template<typename... TArgs>
static void Logger::Info(TArgs&... _mArgs)
{
    LogText("INFO", _mArgs...);
}


#endif // LOG__H__
