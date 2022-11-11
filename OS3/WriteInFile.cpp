#include "windows.h"
#include <cstdio>
#include <string>
#include <iostream>
#include "WriteInFile.h"
using namespace std;


void WriteInFile::clearFile()
{
    fstream fout(FILE_NAME, fstream::trunc | fstream::app);
    fout << "";
    fout.close();
    fout.open(FILE_LOG_NAME, fstream::trunc | fstream::app);
    fout << "";
    fout.close();
}
void WriteInFile::writeNumberInFile(char* number)
{
    fstream fout(FILE_NAME, ios_base::app);

    fout << number << endl;

    fout.close();

    writeInLog("FP", number);
}
void WriteInFile::writeInLog(const char* process, char* number)
{
    fstream fout(FILE_LOG_NAME, ios_base::app);

    SYSTEMTIME systemtime;
    GetLocalTime(&systemtime);
    string s_time = to_string(systemtime.wHour) + ':' + to_string(systemtime.wMinute) + ':' + to_string(systemtime.wSecond) + ':' + to_string(systemtime.wMilliseconds);
    const char* ch_time = s_time.c_str();
    fout << ch_time << "  " << process << "  " << number << endl;
    fout.close();
}
const char* WriteInFile::getTime()
{
    SYSTEMTIME systemtime;
    GetLocalTime(&systemtime);
    string s_time = to_string(systemtime.wHour) + ':' + to_string(systemtime.wMinute) + ':' + to_string(systemtime.wSecond) + ':' + to_string(systemtime.wMilliseconds);
    const char* ch_time = s_time.c_str();

    return ch_time;
}
void WriteInFile::writeErrorInLog(const char* message, int codeOfError)
{
    fstream fout(FILE_LOG_NAME, ios_base::app);

    SYSTEMTIME systemtime;
    GetLocalTime(&systemtime);
    string s_time = to_string(systemtime.wHour) + ':' + to_string(systemtime.wMinute) + ':' + to_string(systemtime.wSecond) + ':' + to_string(systemtime.wMilliseconds);
    const char* ch_time = s_time.c_str();
    fout << ch_time << "  " << message << "  " << codeOfError << endl;

    fout.close();
}
