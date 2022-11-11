#pragma 
#include "windows.h"
#include <cstdio>
#include <string>
#include <fstream>
using namespace std;

#define FILE_NAME "File.txt"
#define FILE_LOG_NAME "log.txt"

class WriteInFile
{
public:
    static const char* getTime();
    static void writeErrorInLog(const char* message, int codeOfError);
    static void clearFile();
    void writeNumberInFile(char* number);
    static void writeInLog(const char* process, char* number);
};