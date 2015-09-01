#include "download.h"
#include "update.h"


void Download::doUpdateProgress(int progress)
{
    HWND client = FindWindow(g_updater.getTitle().c_str(), NULL);
    if(!client)
        throw std::string("");

    std::ofstream file("data/progress");
    file << progress;
    file.close();
}

void Download::download(const char* url, std::string& file)
{
    ofstream fout;                          // output stream
    unsigned char buf[BUF_SIZE];            // input buffer
    unsigned long numrcved;                 // number of bytes read
    unsigned long filelen;                  // length of the file on disk
    HINTERNET hIurl = NULL, hInet = NULL;   // internet handles
    unsigned long contentlen;               // length of content
    unsigned long len;                      // length of contentlen
    unsigned long total = 0;                // running total of bytes received
    char header[80];                        // holds Range header
    int m_percent;

    char fname[MAX_FILENAME_SIZE];
    if(!Download::getFileName(url, fname))
        throw std::string("File name error");

    try
    {
        filelen = Download::openFile(url, fout, fname);

        // See if internet connection is available
        if(InternetAttemptConnect(0) != ERROR_SUCCESS)
            throw std::string("Can not connect.");

        // Open internet connection
        hInet = InternetOpen("downloader", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
        if(hInet == NULL)
            throw std::string("Can not open connection.");

        // Open the URL and request range
        //hIurl = InternetOpenUrl(hInet, url, header, -1, INTERNET_FLAG_NO_CACHE_WRITE, 0);
        hIurl = InternetOpenUrl(hInet, url, header, strlen(header), INTERNET_FLAG_NO_CACHE_WRITE, 0);
        if(hIurl == NULL)
            throw std::string("Can not open url.");

        // Get content length
        len = sizeof contentlen;
        if(!HttpQueryInfo(hIurl, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, &contentlen, &len, NULL))
            throw std::string("File or content length not found.");

        // If existing file (if any) is not complete, then finish downloading
        if(filelen != contentlen && contentlen)
        {
            do
            {
                // Read a buffer of info
                if(!InternetReadFile(hIurl, &buf, BUF_SIZE, &numrcved))
                    throw std::string("Error occurred during download.");

                // Write buffer to disk
                fout.write((const char *) buf, numrcved);
                if(!fout.good())
                    throw std::string("Error while writing file.");

                // update running total
                total += numrcved;

                // Call update function, if specified
                if(numrcved > 0)
                {
                    int percent = (int) ((double) (total + filelen) / (contentlen + filelen) * 100) / 2;
                    if(percent != m_percent)
                    {
                        m_percent = percent;
                        doUpdateProgress(m_percent);
                    }
                }
            } while (numrcved > 0);
        }
    }
    catch (std::string error)
    {
        fout.close();
        InternetCloseHandle(hIurl);
        InternetCloseHandle(hInet);

        // rethrow the exception for use by the caller
        throw;
    }

    fout.close();
    InternetCloseHandle(hIurl);
    InternetCloseHandle(hInet);

    doUpdateProgress(100 / 2);
    std::stringstream ss;
    ss << fname;
    ss >> file;
}

bool Download::getFileName(const char* url, char* fname)
{
    char* p = strrchr(url, '/');
    if(p && (strlen(p) < MAX_FILENAME_SIZE))
    {
        p++;
        strcpy(fname, p);
        return true;
    }

    return false;
}

unsigned long Download::openFile(const char *url, ofstream &fout, char* fname)
{
    std::string dir = "data/";
    dir += fname;
    fout.open(dir.c_str(), ios::binary | ios::out | ios::app | ios::ate);

    if(!fout)
        throw Error("Can't open output file");

    // get current file length
    return fout.tellp();
}

