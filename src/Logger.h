#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <string>
#include <time.h>
#include <iostream>
#include <sys/stat.h>
#include <iomanip>

enum class ERROR {
	CRITICAL = -3,
	INFO = -2,
	WARNING = -1
};

class FileNameGenerate {
protected:
	std::string fileName;
	char Date[50];
public:
	FileNameGenerate() {
		std::string date(getCurrentDate());
		fileName += date;
		fileName += "error_log.txt";
	}
	std::string getFileName() {
		return fileName;
	}
private:
	char* getCurrentDate() {
		time_t t = time(NULL);
		tm* cur_t = localtime(&t);
		strftime(Date, 99, "%Y.%m.%d_%Hh%Mm%Ss_", cur_t);
		return  Date;
	}
};

class Logger {
	std::ofstream fout;
	std::string path;
	std::string mistakeName;
	Logger() {
		auto fileName = FileNameGenerate();
		path = fileName.getFileName();
		fout.open(path, std::ios::app);
	}
public:
	static Logger& Instance()
	{
		static Logger theSingleInstance;
		return theSingleInstance;
	}
	void LoggerFill(const ERROR& mistake, const std::string& expr) {
		MistakeName(mistake);
		time_t t = time(NULL);
		tm* cur_t = localtime(&t);
		char TIME[50];
		strftime(TIME, 99, "%Y.%m.%d %H:%M:%S", cur_t);
		fout << TIME << " - " << mistakeName << " - " << expr << std::endl;
		if (mistake == ERROR::CRITICAL) {
			std::cerr << TIME << " - " << "CRITICAL ERROR " << expr << std::endl;
		}
	}
	void Reset() {
		fout.close();
		auto fileName = FileNameGenerate();
		path = fileName.getFileName();
		fout.open(path, std::ios::app);
	}
	std::string MistakeName(const ERROR& mistake) {
		if (mistake == ERROR::CRITICAL) { mistakeName = "CRITICAL"; }
		else if (mistake == ERROR::INFO) { mistakeName = "INFO"; }
		else if (mistake == ERROR::WARNING) { mistakeName = "WARNING"; }
		return mistakeName;
	}
	~Logger() {
		fout.close();
	}
};