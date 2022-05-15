#include "stdafx.h"
#include "Log.h"

ILogger::~ILogger() {}

Logger::Logger()
{
}

Logger::~Logger()
{
	for(std::list<ILogger*>::iterator i = _loggers.begin(); i != _loggers.end(); ++i)
		delete *i;
}

Logger * Logger::_instance = 0;

Logger * Logger::Instance()
{
	if (!_instance)
		_instance = new Logger();
	return _instance;
}

void Logger::Destroy()
{
	delete Instance();
}

void Logger::AddLog(ILogger * log)
{
	_loggers.push_back(log);
}

void Logger::LogEvent(const char * msg)
{
	for(std::list<ILogger*>::iterator i = _instance->_loggers.begin(); i != _instance->_loggers.end(); ++i)
		(*i)->LogEvent(msg);
}

void Logger::LogException(Xception * err)
{
	LogEvent(String("ОШИБКА:") + err->GetNotification());
}


