#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <ctime>
#include <string>
#include <system_error>
#include "WriteInFile.h"
using namespace std;

typedef struct
{
    STARTUPINFO startupinfo;
    PROCESS_INFORMATION process_information;
} processDate;

processDate initializationProcDate()
{
    processDate pd;
    ZeroMemory(&pd.startupinfo, sizeof(pd.startupinfo));
    pd.startupinfo.cb = sizeof(pd.startupinfo);
    ZeroMemory(&pd.process_information, sizeof(pd.process_information));
    return pd;
}

void close(processDate pd)
{
    CloseHandle(pd.process_information.hProcess);
    CloseHandle(pd.process_information.hThread);
}

BOOL createProcess(TCHAR commandLine[], processDate  pd)
{
    return CreateProcess(nullptr,
        commandLine,
        nullptr,
        nullptr,
        FALSE,
        0,
        nullptr,
        nullptr,
        &pd.startupinfo,
        &pd.process_information);
}

int _tmain(int argc, TCHAR* argv[])
{
    WriteInFile writeInFile;
    writeInFile.clearFile();
    DWORD lastError;

    HANDLE mutex = CreateMutex(NULL, TRUE, TEXT("MainProcessMutex"));
    lastError = GetLastError();
    if (lastError != 0)
    {
        writeInFile.writeErrorInLog("CreateSemaphore's in main Error", lastError);
        return lastError;
    }
    HANDLE fileMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(int), TEXT("FileMapping"));;
    lastError = GetLastError();
    if (lastError != 0)
    {
        writeInFile.writeErrorInLog("CreateFileMapping's in main Error", lastError);
        CloseHandle(mutex);
        return lastError;
    }

    TCHAR consoleProcessCL[] = TEXT("ConsoleProcess");
    TCHAR fileProcessCL[] = TEXT("FileProcess");

    processDate consoleData = initializationProcDate();
    processDate fileData = initializationProcDate();

    BOOL cpSuccess = createProcess(consoleProcessCL, consoleData);
    if (!cpSuccess)
    {
        lastError = GetLastError();
        writeInFile.writeErrorInLog("ConsoleProcess's creation's Error", lastError);
        CloseHandle(mutex);
        CloseHandle(fileMapping);
        return lastError;
    }

    BOOL fpSuccess = createProcess(fileProcessCL, fileData);
    if (!fpSuccess)
    {
        lastError = GetLastError();
        writeInFile.writeErrorInLog("FileProcess's creation's Error", lastError);
        CloseHandle(mutex);
        CloseHandle(fileMapping);
        return lastError;
    }

    if (cpSuccess && fpSuccess)
    {
        Sleep(5);
        srand(time(0));
        while (true) {
            char* ch = (char*)MapViewOfFile(fileMapping, FILE_MAP_WRITE, 0, 0, sizeof(int));
            if (ch == NULL)
            {
                lastError = GetLastError();
                writeInFile.writeErrorInLog("MapViewOfFile's in main Error", lastError);
                close(consoleData);
                close(fileData);
                CloseHandle(mutex);
                CloseHandle(fileMapping);
                return lastError;
            }
            int rand_num;
            rand_num = rand() % 1000 + 1;
            string s = to_string(rand_num);
            char const* str = s.c_str();
            CopyMemory(ch, str, sizeof(rand_num));
            UnmapViewOfFile(ch);
            ReleaseMutex(mutex);
            WaitForSingleObject(mutex, INFINITE);
        }
    }
    close(consoleData);
    close(fileData);
    CloseHandle(mutex);
    CloseHandle(fileMapping);
}
