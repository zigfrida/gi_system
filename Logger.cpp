//
// Created by Amanda Golubics on 2023-04-26.
//

#include "Logger.h"
#include <iostream>
#include <ctime>

using namespace std;

namespace GIS {

    Logger::Logger() {
        logFile.open("../Files/log.txt", ofstream::out | ofstream::trunc);
        if (logFile.is_open()) {
            logFile << "Course Project for COMP 8042" << endl;
            logFile << "Student Name: Amanda Golubics, Student Id: A01048162" << endl;
            logFile << "Student Name: Eunhak Lee, Student Id: A01026056" << endl;
            logFile << "Begin of GIS Program log:" << endl;
            logFile << "dbFile: " << endl;
            logFile << "script: " << endl;
            logFile << "log: " << endl;
            logFile << "Start Time: " << getCurrentTime() << endl;
            logFile.flush();
        } else {
            cerr << "Error: Failed to create the log file!" << endl;
        }
    }

    Logger::~Logger() {
        if (logFile.is_open()) logFile.close();
    }

    Logger& Logger::getInstance() {
        static Logger instance;
        return instance;
    }

    void Logger::closeLogger() {
        getInstance().~Logger();
    }

    void Logger::writeLog(const string& message) {
        if (logFile.is_open()) {
            logFile << message << endl;
            logFile.flush();
        } else {
            cerr << "Error: Log file is not open!" << endl;
        }
    }

    string Logger::getCurrentTime() {
        time_t currentTime = time(nullptr);
        tm* localTime = ::localtime(&currentTime);

        char timeBuffer[80];
        strftime(timeBuffer, sizeof(timeBuffer),"%Y-%m-%d %H:%M:%S", localTime);
        return timeBuffer;
    }
}