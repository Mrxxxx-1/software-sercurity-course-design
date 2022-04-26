// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include"framework.h"
#include"detours.h"
#include"stdio.h"
#include"stdarg.h"
#include"windows.h"
#include<iostream>
#include<winbase.h>
#include<comdef.h>
#include <atlstr.h>
#pragma comment(lib,"detours.lib")
#pragma comment (lib, "ws2_32.lib")

struct info {
    int type, argNum;
    SYSTEMTIME st;
    char argName[10][30] = { 0 };
    char argValue[10][70] = { 0 };
};
HANDLE S;
bool FileWriteFlag;
info sendInfo;
SYSTEMTIME st;//系统时间数据结构，word型
// 定义和引入需要Hook的函数，和替换的函数
static int(WINAPI* O1dMessageBoxW)(_In_opt_ HWND hWnd, _In_opt_ LPCWSTR lpText, _In_opt_ LPCWSTR lpCaption, _In_ UINT uType) = MessageBoxW;
static int(WINAPI* O1dMessageBoxA)(_In_opt_ HWND hWnd, _In_opt_ LPCSTR lpText, _In_opt_ LPCSTR lpCaption, _In_ UINT uType) = MessageBoxA;
extern "C" __declspec(dllexport)int WINAPI NewMessageBoxA(_In_opt_ HWND hWnd, _In_opt_ LPCSTR lpText, _In_opt_ LPCSTR lpCaption, _In_ UINT uType)
{
    printf("\n\n*******************************************\n");
    printf("MessageBoxA Hooked\n");
    GetLocalTime(&st);
    printf("DLL日志输出：%d-%d-%d %02d: %02d: %02d：%03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    printf("**********************************************\n\n");
    return O1dMessageBoxA(NULL, "new MessageBoxA", "Hooked", MB_OK);
}
extern "C" __declspec(dllexport)int WINAPI NewMessageBoxW(_In_opt_ HWND hWnd, _In_opt_ LPCSTR lpText, _In_opt_ LPCSTR lpCaption, _In_ UINT uType)
{
    printf("\n\n*******************************************\n");
    printf("MessageBoxW Hooked\n");
    GetLocalTime(&st);
    printf("DLL日志输出：%d-%d-%d %02d: %02d: %02d：%03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    printf("**********************************************\n\n");
    return O1dMessageBoxA(NULL, "new MessageBoxW", "Hooked", MB_OK);
}


//堆操作 HeapCreate HeapDestroy HeapAlloc HeapFree

static HANDLE(WINAPI* OldHeapCreate)(DWORD fIOoptions, SIZE_T dwInitialSize, SIZE_T dwMaximumSize) = HeapCreate;

extern "C" __declspec(dllexport)HANDLE WINAPI NewHeapCreate(DWORD fIOoptions, SIZE_T dwInitialSize, SIZE_T dwMaximumSize)
{
    HANDLE hHeap = OldHeapCreate(fIOoptions, dwInitialSize, dwMaximumSize);
    printf("\n\n********************************************\n");
    printf("HeapCreate Hooked\n");
    GetLocalTime(&st);
    printf("DLL日志输出：%d-%d-%d %02d: %02d: %02d：%03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    std::cout << "fIOoptions : " << fIOoptions << std::endl;
    std::cout << "dwInitialSize : " << dwInitialSize << std::endl;
    std::cout << "dwMaximumSize : " << dwMaximumSize << std::endl;
    std::cout << "hHeap : " << hHeap << std::endl;
    printf("********************************************\n\n");
    return hHeap;
}

static BOOL(WINAPI* OldHeapDestroy)(HANDLE) = HeapDestroy;

extern "C" __declspec(dllexport)BOOL WINAPI NewHeapDestroy(HANDLE hHeap)
{
    printf(" \n\n********************************************\n");
    printf("HeapDestroy Hooked\n");
    GetLocalTime(&st);
    printf("DLL日志输出：%d-%d-%d %02d: %02d: %02d：%03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    std::cout << "hHeap : " << hHeap << std::endl;
    printf("*******************************************\n\n");
    return OldHeapDestroy(hHeap);

}

//文件操作

static HANDLE(WINAPI* OldCreateFile)(
    LPCTSTR lpFileName,                              //文件名
    DWORD dwDesiredAccess,                           // 访问模式
    DWORD dwShareMode,                                // 共享模式
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,     // 安全模式（也即销毁方式）
    DWORD dwCreationDisposition,                    // how to create
    DWORD dwFlagsAndAttributes,                     // 文件属性
    HANDLE hTemplateFile                            // 模板文件句柄

    ) = CreateFile;
extern "C" __declspec(dllexport)HANDLE WINAPI NewCreateFile(
    LPCTSTR lpFileName,                              //文件名
    DWORD dwDesiredAccess,                           // 访问模式
    DWORD dwShareMode,                                // 共享模式
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,     // 安全模式（也即销毁方式）
    DWORD dwCreationDisposition,                    // how to create
    DWORD dwFlagsAndAttributes,                     // 文件属性
    HANDLE hTemplateFile                            // 模板文件句柄
)

{
    printf("\n\n********************************************\n");
    printf("CreateFile Hooked\n");
    GetLocalTime(&st);
    printf("DLL日志输出：%d-%d-%d %02d: %02d: %02d：%03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    int num = WideCharToMultiByte(CP_OEMCP, NULL, lpFileName, -1, NULL, 0, NULL, FALSE);
    char* pchar = new char[num];
    WideCharToMultiByte(CP_OEMCP, NULL, lpFileName, -1, pchar, num, NULL, FALSE);
    std::cout << "lpFilename : " << pchar << std::endl;
    std::cout << "dwDesiredAccess : 0x" << std::hex << dwDesiredAccess << std::endl;
    std::cout << "dwShareMode : 0x" << std::hex << dwShareMode << std::endl;
    std::cout << "lpSecurityAttributes : 0x" << std::hex << lpSecurityAttributes << std::endl;
    std::cout << "dwCreationDisposition : Ox" << std::hex << dwCreationDisposition << std::endl;
    std::cout << "dwFlagsAndittributes : 0x" << std::hex << dwFlagsAndAttributes << std::endl;
    std::cout << "hTemplateFile : Ox" << std::hex << hTemplateFile << std::endl;
    printf("******************************************\n\n");
    return OldCreateFile(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

static BOOL(WINAPI* OldCloseFILE)(HANDLE hObject) = CloseHandle;

extern "C" _declspec(dllexport)BOOL WINAPI NewCloseFILE(HANDLE hObject) {
    printf("\n\n*******************************************\n");
    printf("CloseHandle Hooked\n");
    GetLocalTime(&st);
    printf("DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    printf("hObject :  0x%x\n", hObject);
    printf("********************************************\n\n");

    return OldCloseFILE(hObject);
}

static BOOL(WINAPI* OldWriteFile)(
    HANDLE hFile,
    LPCVOID lpBuffer,
    DWORD nNumberOfBytesToWrite,
    LPDWORD lpNumberOfBytesWritten,
    LPOVERLAPPED lpOverlapped
    ) = WriteFile;

extern "C" __declspec(dllexport)BOOL WINAPI NewWriteFile(
    HANDLE hFile,
    LPCVOID lpBuffer,
    DWORD nNumberOfBytesToWrite,
    LPDWORD lpNumberOfBytesWritten,
    LPOVERLAPPED lpOverlapped
)
{
    S = CreateSemaphore(NULL, 1, 1, NULL);//当前1个资源，最大允许1个同时访问()
    if (FileWriteFlag == true) {
        WaitForSingleObject(S, INFINITE);
        FileWriteFlag = false;
        printf("\n\n*******************************************\n");
        printf("WriteFile Hooked\n");
        GetLocalTime(&st);
        printf("DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
        printf("hFile :  0x%x\n", hFile);
        printf("lpBuffer :  %s\n", lpBuffer);
        printf("nNumberOfBytesToWrite :  0x%x  Bytes\n", nNumberOfBytesToWrite);
        printf("lpNumberOfBytesWritten :  0x%x Bytes\n", lpNumberOfBytesWritten);
        printf("lpOverlapped :  0x%x\n", lpOverlapped);

        printf("*******************************************\n\n");
        FileWriteFlag = true;
        ReleaseSemaphore(S, 1, NULL);
    }
    CloseHandle(S);
    return OldWriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
}

/* /static BOOL(WINAPI* OldReadFile)(
    HANDLE hFile, //文件的句柄。
    LPVOID lpBuffer, //用于保存读入数据的一个缓冲区。
    DWORD nNumberOfBytesToRead, //指定需要要读入的字符数。
    LPDWORD lpNumberOfBytesRead, //返回实际读取字节数的指针。
    LPOVERLAPPED lpOverlapped //如文件打开时指定了FILE_FLAG_OVERLAPPED，
// 那么必须用这个参数引用一个特殊的结构。该结构定义了一次异步读取操作。否则，应将这个参数设为NULL。
) = ReadFile;

extern "C" __declspec(dllexport)BOOL WINAPI NewReadFile(
    HANDLE hFile,
    LPVOID lpBuffer,
    DWORD nNumberOfBytesToRead,
    LPDWORD lpNumberOfBytesRead,
    LPOVERLAPPED lpOverlapped
)
{
    printf("\n\n*******************************************\n");
    printf("ReadFile Hooked\n");
    GetLocalTime(&st);
    printf("DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    printf("lpBuffer :  %s\n", lpBuffer);
    printf("nNumberOfBytesToWrite :  0x%x\n", nNumberOfBytesToRead);
    printf("lpNumberOfBytesWritten :  0x%x\n", *lpNumberOfBytesRead);
    printf("********************************************\n\n");
    return OldReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
}
*/
//注册表操作
static LONG(WINAPI* OldRegCreateKeyEx)(
    HKEY hKey,
    LPCTSTR lpSubKey,
    DWORD Reserved,
    LPTSTR lpClass,
    DWORD dwOptions,
    REGSAM samDesired,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    PHKEY phkResult,
    LPDWORD lpdwDisposition
    ) = RegCreateKeyEx;

extern "C" __declspec(dllexport)LONG WINAPI NewRegCreateKeyEx(
    HKEY hKey,
    LPCTSTR lpSubKey,
    DWORD Reserved,
    LPTSTR lpClass,
    DWORD dwOptions,
    REGSAM samDesired,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    PHKEY phkResult,
    LPDWORD lpdwDisposition
)
{
    printf("\n\n*******************************************\n");
    printf("RegCreateKey Hooked\n");
    GetLocalTime(&st);
    printf("DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    int num = WideCharToMultiByte(CP_OEMCP, NULL, lpSubKey, -1, NULL, 0, NULL, FALSE);
    char* pchar = new char[num];
    WideCharToMultiByte(CP_OEMCP, NULL, lpSubKey, -1, pchar, num, NULL, FALSE);
    //std::cout << "lpSubKey : " << pchar << std::endl;
    printf("lpSubKey : %s\n", pchar);
    if (*lpdwDisposition == 0x00000001L) {
        printf("该键是新创建的键\n");
    }
    else if (*lpdwDisposition == 0x00000002L)
    {
        printf("该键是已经存在的键\n");
    }
    else
    {
        printf("未知是否新建\n");
    }
    printf("********************************************\n\n");
    return OldRegCreateKeyEx(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired, lpSecurityAttributes, phkResult, lpdwDisposition);
}



static LONG(WINAPI* OldRegCloseKey)(HKEY hKey) = RegCloseKey;
extern "C" __declspec(dllexport)LONG WINAPI NewRegCloseKey(
    HKEY hKey
)
{
    printf("\n\n*******************************************\n");
    printf("RegCloseKey Hooked\n");
    GetLocalTime(&st);
    printf("DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    printf("hKey(HKEY) :  0x%x\n", hKey);
    printf("********************************************\n\n");
    return OldRegCloseKey(hKey);
}

static LONG(WINAPI* OldRegOpenKeyEx)(
    HKEY hKey,         // handle to open key
    LPCTSTR lpSubKey,  // subkey name
    DWORD ulOptions,   // reserved
    REGSAM samDesired, // security access mask
    PHKEY phkResult    // handle to open key
    ) = RegOpenKeyEx;

extern "C" __declspec(dllexport)LONG WINAPI NewRegOpenKeyEx(
    HKEY hKey,         // handle to open key
    LPCTSTR lpSubKey,  // subkey name
    DWORD ulOptions,   // reserved
    REGSAM samDesired, // security access mask
    PHKEY phkResult    // handle to open key

)
{
    LONG a = OldRegOpenKeyEx(hKey, lpSubKey, ulOptions, samDesired, phkResult);
    if (a == ERROR_SUCCESS)
    {
        int err;
        unsigned long numKeys;
        unsigned long longestKey;
        unsigned long numValues;
        unsigned long longestValue;
        unsigned long longestDataLength;
        WCHAR* valueName;
        unsigned long valueLength;
        unsigned long type;
        WCHAR* data;
        unsigned long dataLength;
        HKEY rootKey, hkey;
        rootKey = HKEY_CURRENT_USER;

        if ((err = RegOpenKeyEx(rootKey, lpSubKey, 0, KEY_READ, &hkey)) == ERROR_SUCCESS)
        {

            if (RegQueryInfoKey(hkey, NULL, NULL, NULL, &numKeys, &longestKey, NULL,
                &numValues, &longestValue, &longestDataLength, NULL, NULL) == ERROR_SUCCESS)
            {
                data = new WCHAR[longestDataLength + 2];
                valueName = new WCHAR[longestValue + 2];
                printf("%d\n", numValues);
                for (unsigned long i = 0; i < numValues; ++i)
                {
                    dataLength = longestDataLength + 1;
                    valueLength = longestValue + 1;

                    if ((err = RegEnumValue(hkey, i, (LPWSTR)valueName, &valueLength, NULL, 		//用来枚举指定项的值。
                        &type, (unsigned char*)data, &dataLength)) == ERROR_SUCCESS)
                    {
                        // << (unsigned char*)data << endl;
                        _bstr_t b(valueName);
                        const char* c = b;
                        printf("valueName:%s\n", c);
                        _bstr_t d(data);
                        c = d;
                        printf("data :%s\n", c);
                    }
                }
                delete[] data;
                delete[] valueName;
            }
        }
    }
    printf("\n\n*******************************************\n");
    printf("NewRegOpenKeyEx Hooked\n");
    CString str1(lpSubKey);

    CString str2(_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"));
    if (0 == str1.CompareNoCase(str2))
    {
        printf("该日志文件为开机自启项\n");
    }
    else
    {
        printf("该日志文件不是开机自启项\n");
    }
    printf("\n\n*******************************************\n");
    printf("NewRegOpenKeyEx Hooked\n");
    GetLocalTime(&st);
    printf("DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    int num = WideCharToMultiByte(CP_OEMCP, NULL, lpSubKey, -1, NULL, 0, NULL, FALSE);
    char* pchar = new char[num];
    WideCharToMultiByte(CP_OEMCP, NULL, lpSubKey, -1, pchar, num, NULL, FALSE);
    printf("lpSubKey: %s \n", pchar);
    printf("********************************************\n\n");
    return OldRegOpenKeyEx(hKey, lpSubKey, ulOptions, samDesired, phkResult);
}

static LONG(WINAPI* OldRegSetValueEx)(
    HKEY hKey,           // handle to key
    LPCTSTR lpValueName, // value name
    DWORD Reserved,      // reserved
    DWORD dwType,        // value type
    CONST BYTE* lpData,  // value data
    DWORD cbData         // size of value data
    ) = RegSetValueEx;

extern "C" __declspec(dllexport)LONG WINAPI NewRegSetValueEx(
    HKEY hKey,           // handle to key
    LPCTSTR lpValueName, // value name
    DWORD Reserved,      // reserved
    DWORD dwType,        // value type
    CONST BYTE * lpData,  // value data
    DWORD cbData         // size of value data
)
{
    LONG a = OldRegSetValueEx(hKey, lpValueName, Reserved, dwType, lpData, cbData);
    if (a == ERROR_SUCCESS) printf("注册表修改成功\n");
    printf("\n\n*******************************************\n");
    printf("NewRegSetValueEx Hooked\n");

    printf("\n\n*******************************************\n");
    printf("NewRegSetValueEx Hooked\n");
    GetLocalTime(&st);
    printf("DLL日志输出：%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    int num = WideCharToMultiByte(CP_OEMCP, NULL, lpValueName, -1, NULL, 0, NULL, FALSE);
    char* pchar = new char[num];
    WideCharToMultiByte(CP_OEMCP, NULL, lpValueName, -1, pchar, num, NULL, FALSE);
    printf("lpValueName : %s", pchar);
    printf("********************************************\n\n");
    return OldRegSetValueEx(hKey, lpValueName, Reserved, dwType, lpData, cbData);
}

//网络通信操作
static SOCKET(WINAPI* Oldsocket)(
    int af,
    int type,
    int protocol
    ) = socket;
extern "C" __declspec(dllexport)SOCKET WINAPI Newsocket(int af, int type, int protocol) {
    sendInfo.argNum = 3;
    // 参数名
    sprintf(sendInfo.argName[0], "af");
    sprintf(sendInfo.argName[1], "type");
    sprintf(sendInfo.argName[2], "protocol");
    // 参数值
    sprintf(sendInfo.argValue[0], "%08X", af);
    sprintf(sendInfo.argValue[1], "%08X", type);
    sprintf(sendInfo.argValue[2], "%08X", protocol);
    //sendInfo.type = THESOCKET;
    GetLocalTime(&(sendInfo.st));
    //memcpy(lpBase, &sendInfo, sizeof(info));
    //ReleaseSemaphore(hSemaphore, 1, NULL);
    return Oldsocket(af, type, protocol);
}

static int (WINAPI* Oldbind)(
    SOCKET         s,
    const sockaddr* name,
    int            namelen
    ) = bind;
extern "C" __declspec(dllexport)int WINAPI Newbind(SOCKET s, const sockaddr * name, int namelen) {
    //name->sa_data
    //sendInfo.argNum = 5;
    // 参数名
    sprintf(sendInfo.argName[0], "s");
    sprintf(sendInfo.argName[1], "name");
    sprintf(sendInfo.argName[2], "namelen");
    sprintf(sendInfo.argName[3], "IP");
    sprintf(sendInfo.argName[4], "port");
    // 参数值
    sprintf(sendInfo.argValue[0], "%08X", s);
    sprintf(sendInfo.argValue[1], "%08X", name);
    sprintf(sendInfo.argValue[2], "%08X", namelen);
    struct sockaddr_in* sock = (struct sockaddr_in*)name;
    int port = ntohs(sock->sin_port);
    sprintf(sendInfo.argValue[3], "%s", inet_ntoa(sock->sin_addr));
    sprintf(sendInfo.argValue[4], "%d", port);
    //sendInfo.type = BIND;
    GetLocalTime(&(sendInfo.st));
    //memcpy(lpBase, &sendInfo, sizeof(info));
    //ReleaseSemaphore(hSemaphore, 1, NULL);
    return Oldbind(s, name, namelen);
}

static int (WINAPI* Oldsend)(
    SOCKET     s,
    const char* buf,
    int        len,
    int        flags
    ) = send;
extern "C" __declspec(dllexport)int WINAPI Newsend(SOCKET s, const char* buf, int len, int flags) {
    //sendInfo.argNum = 5;
    // 参数名
    sprintf(sendInfo.argName[0], "s");
    sprintf(sendInfo.argName[1], "buf");
    sprintf(sendInfo.argName[2], "len");
    sprintf(sendInfo.argName[3], "flags");
    sprintf(sendInfo.argName[4], "data");
    // 参数值
    sprintf(sendInfo.argValue[0], "%08X", s);
    sprintf(sendInfo.argValue[1], "%08X", buf);
    sprintf(sendInfo.argValue[2], "%08X", len);
    sprintf(sendInfo.argValue[3], "%08X", flags);
    sprintf(sendInfo.argValue[4], "%s", buf);
    //sendInfo.type = SEND;
    GetLocalTime(&(sendInfo.st));
    //memcpy(lpBase, &sendInfo, sizeof(info));
    //ReleaseSemaphore(hSemaphore, 1, NULL);
    return Oldsend(s, buf, len, flags);
}

static int (WINAPI* Oldconnect)(
    SOCKET         s,
    const sockaddr* name,
    int            namelen
    ) = connect;
extern "C" __declspec(dllexport)int WINAPI Newconnect(SOCKET s, const sockaddr * name, int namelen) {
    //name->sa_data
    sendInfo.argNum = 5;
    // 参数名
    sprintf(sendInfo.argName[0], "s");
    sprintf(sendInfo.argName[1], "name");
    sprintf(sendInfo.argName[2], "namelen");
    sprintf(sendInfo.argName[3], "IP");
    sprintf(sendInfo.argName[4], "port");
    // 参数值
    sprintf(sendInfo.argValue[0], "%08X", s);
    sprintf(sendInfo.argValue[1], "%08X", name);
    sprintf(sendInfo.argValue[2], "%08X", namelen);
    struct sockaddr_in* sock = (struct sockaddr_in*)name;
    int port = ntohs(sock->sin_port);
    sprintf(sendInfo.argValue[3], "%s", inet_ntoa(sock->sin_addr));
    sprintf(sendInfo.argValue[4], "%d", port);
    //sendInfo.type = CONNECT;
    GetLocalTime(&(sendInfo.st));
    //memcpy(lpBase, &sendInfo, sizeof(info));
    //ReleaseSemaphore(hSemaphore, 1, NULL);
    return Oldconnect(s, name, namelen);
}

static void* (__cdecl* Oldmemcpy)(void* _Dst, const void* _Src, size_t _Size) = memcpy;
extern "C" __declspec(dllexport)void* __cdecl Newmemcpy(void* _Dst, const void* _Src, size_t _Size) {
    printf("memcpy hooked\n");
    return Oldmemcpy(_Dst, _Src, _Size);
}
//void* __cdecl memcpy(void* _Dst, const void* _Src, size_t _Size)

//static void (WINAPI* OldRtlCopyMemory)(
//	void* Destination,
//	const void* Source,
//	size_t      Length	
//	) = RtlCopyMemory;



static int (WINAPI* Oldrecv)(
    SOCKET     s,
    char* buf,
    int        len,
    int        flags
    ) = recv;
extern "C" __declspec(dllexport)int WINAPI Newrecv(SOCKET s, char* buf, int len, int flags) {
    int i;
    //sendInfo.argNum = 4;
    // 参数名
    sprintf(sendInfo.argName[0], "s");
    sprintf(sendInfo.argName[1], "buf");
    sprintf(sendInfo.argName[2], "len");
    sprintf(sendInfo.argName[3], "flags");
    // 参数值
    sprintf(sendInfo.argValue[0], "%08X", s);
    sprintf(sendInfo.argValue[1], "%08X", buf);
    sprintf(sendInfo.argValue[2], "%08X", len);
    sprintf(sendInfo.argValue[3], "%08X", flags);
    //sendInfo.type = RECV;
    GetLocalTime(&(sendInfo.st));
    //memcpy(lpBase, &sendInfo, sizeof(info));
    //ReleaseSemaphore(hSemaphore, 1, NULL);
    return Oldrecv(s, buf, len, flags);
}

BOOL WINAPI DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourAttach(&(PVOID&)O1dMessageBoxW, NewMessageBoxW);
        DetourAttach(&(PVOID&)O1dMessageBoxA, NewMessageBoxA);
        DetourAttach(&(PVOID&)OldCreateFile, NewCreateFile);
        DetourAttach(&(PVOID&)OldCloseFILE, NewCloseFILE);
        DetourAttach(&(PVOID&)OldWriteFile, NewWriteFile);
        //DetourAttach(&(PVOID&)OldReadFile, NewReadFile);
        DetourAttach(&(PVOID&)OldHeapCreate, NewHeapCreate);
        DetourAttach(&(PVOID&)OldHeapDestroy, NewHeapDestroy);
        DetourAttach(&(PVOID&)OldRegCreateKeyEx, NewRegCreateKeyEx);
        DetourAttach(&(PVOID&)OldRegOpenKeyEx, NewRegOpenKeyEx);
        DetourAttach(&(PVOID&)OldRegSetValueEx, NewRegSetValueEx);
        DetourAttach(&(PVOID&)OldRegCloseKey, NewRegCloseKey);
        DetourAttach(&(PVOID&)Oldsocket, Newsocket);
        DetourAttach(&(PVOID&)Oldbind, Newbind);
        DetourAttach(&(PVOID&)Oldsend, Newsend);
        DetourAttach(&(PVOID&)Oldconnect, Newconnect);
        DetourAttach(&(PVOID&)Oldrecv, Newrecv);
        DetourTransactionCommit();
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourDetach(&(PVOID&)O1dMessageBoxW, NewMessageBoxW);
        DetourDetach(&(PVOID&)O1dMessageBoxA, NewMessageBoxA);
        DetourDetach(&(PVOID&)OldCreateFile, NewCreateFile);
        DetourDetach(&(PVOID&)OldCloseFILE, NewCloseFILE);
        DetourDetach(&(PVOID&)OldWriteFile, NewWriteFile);
        //DetourDetach(&(PVOID&)OldReadFile, NewReadFile);
        DetourDetach(&(PVOID&)OldHeapCreate, NewHeapCreate);
        DetourDetach(&(PVOID&)OldHeapDestroy, NewHeapDestroy);
        DetourDetach(&(PVOID&)OldRegCreateKeyEx, NewRegCreateKeyEx);
        DetourDetach(&(PVOID&)OldRegCloseKey, NewRegCloseKey);
        DetourDetach(&(PVOID&)OldRegOpenKeyEx, NewRegOpenKeyEx);
        DetourDetach(&(PVOID&)OldRegSetValueEx, NewRegSetValueEx);
        DetourDetach(&(PVOID&)Oldsocket, Newsocket);
        DetourDetach(&(PVOID&)Oldbind, Newbind);
        DetourDetach(&(PVOID&)Oldsend, Newsend);
        DetourDetach(&(PVOID&)Oldconnect, Newconnect);
        DetourDetach(&(PVOID&)Oldrecv, Newrecv);
        DetourTransactionCommit();
        break;
    }
    return TRUE;
}



