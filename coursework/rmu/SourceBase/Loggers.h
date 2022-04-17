#ifndef __LOGGERS_H__
#define __LOGGERS_H__

#include "Log.h"
//#include <fstream>
#include <cstdio>

class FileLogger : public ILogger {
    //std::fstream _stream;
    std::FILE* _stream;

public:
    FileLogger(const TCHAR* fileName);
    virtual void LogEvent(const TCHAR* msg);
    virtual ~FileLogger();
};

#endif