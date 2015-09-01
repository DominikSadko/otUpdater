#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include <string>
#include <windows.h>
#include <wininet.h>
#include <fstream>

using namespace std;

const int MAX_ERRMSG_SIZE = 255;
const int MAX_FILENAME_SIZE = 512;
const int BUF_SIZE = 10240;          // 10 KB

// Exception class for donwload errors;
class Error
{
    public:
        Error(std::string error)
        {
            message = error;
        }

    std::string message;
};

// A class for downloading files from the internet
class Download
{
    private:
        static bool getFileName(const char* url, char* fname);
        static unsigned long openFile(const char* url, ofstream& fout, char* fname);

    public:
        static void doUpdateProgress(int32_t progress);
        static void download(const char* url, std::string& file);
};

#endif

