#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>

int main()
{
	HANDLE hOpenFile = (HANDLE)CreateFile(L"D:\\test1.dll", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, NULL, NULL);
	CloseHandle(hOpenFile);
	return 0;
}