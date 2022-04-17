#include "Loggers.h"
#include "stdafx.h"

FileLogger::FileLogger(const TCHAR* fileName) {
    //_stream.open(fileName, std::ios_base::out||std::ios_base::app );
    _stream = std::fopen(fileName, "a");
}

void FileLogger::LogEvent(const TCHAR* msg) {
    //_stream << msg << std::endl;
    std::fprintf(_stream, "%s\n", msg);
}

FileLogger::~FileLogger() {
    //_stream.close();
    std::fclose(_stream);
}
