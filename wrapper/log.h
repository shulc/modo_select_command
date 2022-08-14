#pragma once
#include <lxu_log.hpp>

#include <string>

class TLog
    : public CLxLogMessage
    , public std::stringstream
{
public:
    ~TLog() {
        Info(str());
    }

private:
    const char* GetFormat();
    const char* GetVersion();
};