#include "update.h"
#include "download.h"

using namespace std;

Uptader g_updater = Uptader();

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR args, int)
{
    std::string arguments = Uptader::tostring((char*)(args));
    StringVec vecs = Uptader::explodeString(arguments, ";");

    if(vecs.size() < 3)
    {
        g_updater.message("You are not supported to run this file.");
        return 0;
    }

    g_updater.setTitle(vecs[0]);
    std::string runFile = vecs[1];
    std::string downloadUrl = vecs[2];
    std::string zipFile = "";

    // download file
    try
    {
        Download::download(downloadUrl.c_str(), zipFile);
    }
    catch(std::string error)
    {
        if(!error.empty())
            g_updater.message(error);

        return 0;
    }

    Sleep(100);

    // update file
    zipFile = "data/"+ zipFile;
    if(!Uptader::unzip(zipFile.c_str(), runFile.c_str()))
    {
        g_updater.message("Error while unpacking file: "+ zipFile);
        return 0;
    }

    g_updater.message("Update files has been downloaded successfully.");

    if((int)Uptader::run(runFile.c_str()) <= 32)
        g_updater.message("Cannot run: "+ runFile);

    return 0;
}
