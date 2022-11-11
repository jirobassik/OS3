#include <windows.h>
#include <stdio.h>
#include <locale.h>
#include <iostream>
#include "D:/LABS/OS3/OS3/WriteInFile.cpp"
using namespace std;

int main()
{
	DWORD lastError;
	WriteInFile writeInFile;
	HANDLE mutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, TEXT("MainProcessMutex"));
	lastError = GetLastError();
	if (lastError != 0)
	{
		writeInFile.writeErrorInLog("OpenMutex's in ConsoleProcess Error", lastError);
		return lastError;
	}
	HANDLE fileMapping = OpenFileMapping(FILE_MAP_READ, FALSE, TEXT("FileMapping"));
	lastError = GetLastError();
	if (lastError != 0)
	{
		writeInFile.writeErrorInLog("OpenFileMapping's in ConsoleProcess Error", lastError);
		return lastError;
	}

	char* ch = (char*)MapViewOfFile(fileMapping, FILE_MAP_READ, 0, 0, sizeof(int));
	lastError = GetLastError();
	if (lastError != 0)
	{
		writeInFile.writeErrorInLog("MapViewOfFile's in ConsoleProcess Error", lastError);
		return lastError;
	}
	while (true)
	{
		WaitForSingleObject(mutex, INFINITE);
		cout << "CP: ";
		cout << ch << endl;
		writeInFile.writeInLog("CP", ch);
		ReleaseMutex(mutex);
	}
}


