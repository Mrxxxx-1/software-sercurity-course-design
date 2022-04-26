// TestConsole.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include<cstdio>
#include<windows.h>
#include<detours.h>
#pragma comment(lib,"detours.lib")

int main()
{

	STARTUPINFO si;//数据结构，用于指定新进程的主窗口特性
	PROCESS_INFORMATION pi;//数据结构，返回有关新进程及其主线程的信息
	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));//初始化结构体，用0填充一段内存
	si.cb = sizeof(STARTUPINFO);
	WCHAR DirPath[MAX_PATH + 1];
	//errno_t wcscpy_s( wchar_t *restrict dest, rsize_t destsz,const wchar_t* restrict src ); 
	//dest	-	指向复制目标的宽字符数组的指针;src - 指向复制来源的空终止宽字符串的指针;destsz - 要写入的最大字符数，典型地为目标缓冲区的大小
	//MAX_PATH = 260;
	wcscpy_s(DirPath, MAX_PATH, L"C:\\Users\\Mrx\\Desktop\\software-security-design\\TestInitialNullForStudent\\InjectDll\\Debug");
	char DLLPath[MAX_PATH + 1] = "C:\\Users\\Mrx\\Desktop\\software-security-design\\TestInitialNullForStudent\\InjectDll\\Debug\\InnjectDll.dll";
	WCHAR EXE[MAX_PATH + 1] = { 0 };
	//选择测试程序
	int i = 0;
	scanf("%d", &i);
	getchar();
	if (i == 1)
		wcscpy_s(EXE, MAX_PATH, L"C:\\Users\\Mrx\\Desktop\\software-security-design\\TestInitialNullForStudent\\TestFunc\\Debug\\TestFunc.exe");
	else if (i == 2)
		wcscpy_s(EXE, MAX_PATH, L"C:\\Users\\Mrx\\Desktop\\software-security-design\\TestInitialNullForStudent\\Sample\\Heap repeatedly release.exe");
	else if(i==3)
		wcscpy_s(EXE, MAX_PATH, L"C:\\Users\\Mrx\\Desktop\\software-security-design\\TestInitialNullForStudent\\Sample\\selfcopy.exe");
	else if(i==4)
		wcscpy_s(EXE, MAX_PATH, L"C:\\Users\\Mrx\\Desktop\\software-security-design\\TestInitialNullForStudent\\Sample\\executable file.exe");
	else if(i==5)
		wcscpy_s(EXE, MAX_PATH, L"C:\\Users\\Mrx\\Desktop\\software-security-design\\TestInitialNullForStudent\\Sample\\regedit.exe");
	else if(i==6)
		wcscpy_s(EXE, MAX_PATH, L"C:\\Users\\Mrx\\Desktop\\software-security-design\\TestInitialNullForStudent\\Sample\\executable(exe).exe");
	else printf("error!");
	if (DetourCreateProcessWithDllEx(EXE, NULL, NULL, NULL, TRUE, CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED, NULL, DirPath, &si, &pi, DLLPath, NULL))
	{
		ResumeThread(pi.hThread);//回复线程运行
		WaitForSingleObject(pi.hProcess, INFINITE);//你可以使用WaitForSingleObject函数来等待一个内核对象变为已通知状态：
		//DWORD WaitForSingleObject(HANDLE hObject, //指明一个内核对象的句柄;DWORD dwMilliseconds); //等待时间
	}
	else
	{
		char error[100];
		sprintf_s(error, "%d", GetLastError());//sprintf将数据格式化输出到字符串;GetLastError()返回上一个函数调用设置的线程的32位错误代码
	}
	
	/*std::cout << "Hello World!\n";
	printf("Test!!!!!!!");*/
	return 0;
}
