#pragma once
#include "Logger.h"
#define NODEBUG
#ifdef NODEBUG
ERROR CRITICAL = ERROR::CRITICAL;
ERROR INFO = ERROR::INFO;
ERROR WARNING = ERROR::WARNING;
extern Logger& Log = Logger::Instance();
#define LOG(ERROR,EXPR)\
       Log.LoggerFill(ERROR,EXPR);
#define RESET\
       Log.Reset();
#else
#define LOGGI(ERROR,EXPR)   
#define LOG(ERROR,EXPR)    
#define RESET
#endif
