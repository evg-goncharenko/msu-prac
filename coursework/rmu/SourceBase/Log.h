#ifndef __LOG_H__
#define __LOG_H__

#include <list>
#include "Lib/Xception.h"

class ILogger
{
public:
	virtual void LogEvent(const char * msg) = 0;
	virtual ~ILogger() = 0;
};

class Logger
{
	std::list<ILogger*> _loggers;
	Logger();
	~Logger();
	static Logger * _instance;
public:
	static Logger * Instance();
	static void Destroy();
	void AddLog(ILogger *);
	void LogEvent(const char * msg);
	void LogException(Xception *);
};

#endif