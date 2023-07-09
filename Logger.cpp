//
// Created by Amanda Golubics on 2023-04-26.
//

#include "Logger.h"
#include <iostream>
#include <ctime>

using namespace std;

namespace GIS {

    Logger::Logger() {}

    Logger::~Logger() {
        if (logFile.is_open()) logFile.close();
    }

    void Logger::initialText(const string& scriptFile, const string& databaseFile) {
        logFile.open("../Files/log.txt", ofstream::out | ofstream::trunc);
        if (logFile.is_open()) {
            logFile << "Course Project for COMP 8042" << endl;
            logFile << "Student Name: Amanda Golubics, Student Id: A01048162" << endl;
            logFile << "Student Name: Eunhak Lee, Student Id: A01026056" << endl;
            logFile << "Begin of GIS Program log:" << endl;
            logFile << "dbFile: " << databaseFile << endl;
            logFile << "script: " << scriptFile << endl;
            logFile << "log: " << "../Files/log.txt" << endl;
            logFile << "Start Time: " << getCurrentTime() << endl;
            logFile.flush();
        } else {
            cerr << "Error: Failed to create the log file!" << endl;
        }
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

    void Logger::writeCommandCount(const string &commandName) {
        if (logFile.is_open()) {
            logFile << "Command " << commandCount << ": " << commandName << endl << endl;
            logFile.flush();
            commandCount++;
        } else {
            cerr << "Error: Log file is not open!" << endl;
        }
    }

    void Logger::quitCommand() {
        if (logFile.is_open()) {
            logFile << "Command " << commandCount << ": quit" << endl << endl;
            logFile << "Terminating execution of commands." << endl;
            logFile << "------------------------------------------------------------------------------------------" << endl;
            logFile << "End Time: " << getCurrentTime() << endl;
            logFile.flush();
            commandCount++;
        } else {
            cerr << "Error: Log file is not open!" << endl;
        }
    }

}