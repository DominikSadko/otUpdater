#ifndef __UPDATER_H__
#define __UPDATER_H__

#include <windows.h>

#include "unzip/unzip.h"
#include "unzip/zip.h"
#include <vector>
#include <stdio.h>

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

typedef std::vector<std::string> StringVec;

class Uptader;
extern Uptader g_updater;

struct Uptader
{
    Uptader() {};

    void message(std::string text)
    {
        MessageBox(0, text.c_str(), m_title.c_str(), 0);
    }

    static bool unzip(const char* file, const char* runFile)
    {
        // Unpack Data
        HZIP zip = OpenZip(file, 0);
        ZIPENTRY ze; GetZipItem(zip, -1, &ze);
        int numitems = ze.index;

        for(int zi = 0; zi < numitems; zi++)
        {
            std::ofstream progress("data/progress");
            progress << (zi * 100 / numitems / 2) + 50;
            progress.close();

            GetZipItem(zip, zi, &ze);
            if(strcmp(ze.name, runFile))
                UnzipItem(zip, zi, ze.name);
        }

        std::ofstream progress("data/progress");
        progress << 100;
        progress.close();
        // Close Client

        DWORD processId;
        HANDLE hProcess;
        HWND client;
        while((client = FindWindow(g_updater.getTitle().c_str(), NULL)))
        {
            GetWindowThreadProcessId(client, &processId);
            hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, processId);
            SendMessage(client, WM_CLOSE, 0, 0);
            CloseHandle(hProcess);
        }

        // Unpack exe
        Sleep(100);

        int32_t index = 0;
        FindZipItem(zip, runFile, false, &index, &ze);
        UnzipItem(zip, index, ze.name);

        CloseZipU(zip);

        Sleep(100);
        if(remove(file))
            return false;

        return true;
    }

    static HINSTANCE run(const char* file, const char* params = "")
    {
        return ShellExecute(NULL, "open", file, params, NULL, SW_SHOWNORMAL);
    }

    static std::string tostring(const char* text)
    {
        std::stringstream ss;
        std::string newText;
        ss << text;
        ss >> newText;

        return newText;
    };

    static StringVec explodeString(const std::string& string, const std::string& separator)
    {
        StringVec returnVector;
        size_t start = 0, end = 0;
        while((end = string.find(separator, start)) != std::string::npos)
        {
            returnVector.push_back(string.substr(start, end - start));
            start = end + separator.size();
        }

        returnVector.push_back(string.substr(start));
        return returnVector;
    }

    void setTitle(std::string title) { m_title = title; }
    std::string getTitle() { return m_title; }

    private:
        std::string m_title;
};

#endif



