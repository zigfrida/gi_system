//
// Created by Amanda Golubics on 2023-04-26.
//

#ifndef GI_SYSTEM_LOGGER_H
#define GI_SYSTEM_LOGGER_H

#include <fstream>
#include <iostream>
#include <ctime>

using namespace std;

namespace GIS {
    class Logger {
        public:
            static Logger& getInstance();
            static void closeLogger();
            void writeLog(const string& message);

        private:
            Logger();
            ~Logger();
            ofstream logFile;
            string getCurrentTime();

    };
}


#endif //GI_SYSTEM_LOGGER_H
