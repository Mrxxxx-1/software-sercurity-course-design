// dllmain.cpp : ���� DLL #include "framework.h"
#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "framework.h"
#include "detours.h"
#include "stdio.h"
#include "stdarg.h"
#include "windows.h"
#include <iostream>
#include<set>
#include<mutex>
#include <atlstr.h>
#include<comdef.h>
#include <wchar.h>
#include <locale.h>

#pragma comment(lib,"detours.lib")
#define  MAX  2048
HANDLE S, HeapD, HeapAll;   //ÿ����һ���Ѿͼ�¼��HeapC�� ɾ���Ķѵ�ַ�洢��HeapD�� Ȼ����HeapC������ �����������ȷ�ͷ� ��������쳣
std::set<HANDLE> HeapC;
std::set<char*> File_name_copy;

SYSTEMTIME st;
bool FileWriteFlag = true;
bool HeapAllocFlag = true;
char Temp[256];
const char* FileCopy = "copy";
const char* EXE = "test";


#pragma data_seg ("MySeg")
char seg[1000][256] = {};//�������ݶ� //�����ڶ����ͬʱ���г�ʼ��!!!!
int count1 = 0;//�������ݶ�
volatile int HeapA1locNum = 0;//�������ݶ�
volatile int buffer[1024] = {};
#pragma data_seg ()
#pragma comment (linker,"/section:MySeg,RWS")
//[ע�⣺���ݽڵ�����is case sensitive]
//��ô������ݽ��е����ݿ���������DLL��ʵ��֮�乲�����ж���Щ���ݵĲ��������ͬһ��ʵ���ģ�
//��������ÿ�����̵ĵ�ַ�ռ��ж���һ�ݡ�
std::mutex mtx;

extern "C" __declspec(dllexport)void ChangeSeg(LPCSTR str) {
    //ʹ��extern "C" __declspec(dllexport)����������������DLL�еĺ������ֱ����뵼��������һ�¡�
    //ʹ��__declspec(dllexport)����������������DLL�еĺ��������뵼���������ֲ�һ�£��б仯
    mtx.lock();
    count1++;
    if (count1 >= 1000) {
        count1--;
    }
    else {
        strcpy_s(seg[count1], str);//����ŵ�ǰ
    }
    mtx.unlock();
}

extern "C"  __declspec(dllexport)void GetSeg(char* str, int i) {   //���������е��û�ȡ������Ϣ
    mtx.lock();
    strcpy_s(str, 256, seg[i]);
    mtx.unlock();
}

extern "C" _declspec(dllexport)void GetNum(int& num) {  //��ö��ٸ���������
    mtx.lock();
    num = count1;
    mtx.unlock();
}

extern "C" __declspec(dllexport)void GetHeapAllocNum(int& num) {
    num = HeapA1locNum;
}

void OutputDebugStringOwn(TCHAR s[])
{
}
//_T("")��һ����,��������������ĳ���֧��Unicode����
#define DP0(fmt) {TCHAR sOut[256]; _stprintf_s(sOut,_T(fmt));OutputDebugStringOwn(sOut);ChangeSeg(fmt);}
#define DP1(fmt,var) {TCHAR sOut[256]; _stprintf_s(sOut,_T(fmt),var);OutputDebugStringOwn(sOut);ChangeSeg(fmt);}
#define DP2(fmt,var1,var2) {TCHAR sOut[256]; _stprintf_s(sOut,_T(fmt),var1,var2);OutputDebugStringOwn(sOut);ChangeSeg(fmt);}
#define DP3(fmt,var1,var2,var3) {TCHAR sOut[256]; _stprintf_s(sOut,_T(fmt),var1,var2,var3);OutputDebugStringOwn(sOut);ChangeSeg(fmt);}
#define DP4(fmt,var1,var2,var3,var4) {TCHAR sOut[256]; _stprintf_s(sOut,_T(fmt),var1,var2,var3,var4);OutputDebugStringOwn(sOut);ChangeSeg(fmt);}
#define DP5(fmt,var1,var2,var3,var4,var5) {TCHAR sOut[256]; _stprintf_s(sOut,_T(fmt),var1,var2,var3,var4,var5);OutputDebugStringOwn(sOut);ChangeSeg(fmt);}
#define DP6(fmt,var1,var2,var3,var4,var5,var6) {TCHAR sOut[256]; _stprintf_s(sOut,_T(fmt),var1,var2,var3,var4,var5,var6);OutputDebugStringOwn(sOut);ChangeSeg(fmt);}
#define DP7(fmt,var1,var2,var3,var4,var5,var6,var7) {TCHAR sOut[256]; _stprintf_s(sOut,_T(fmt),var1,var2,var3,var4,var5,var6,var7);OutputDebugStringOwn(sOut);ChangeSeg(fmt);}


void Folder_Scope(char* FilePath) {   //�����ļ�����API���ļ�����·�� �жϸ��ļ����ڵ�λ���Ƿ����ļ���
    HANDLE hFile;
    char PATH[256];
    WCHAR P[256];
    int i;
    for (i = strlen(FilePath) - 1; i >= 0; i--) {
        if (FilePath[i] == 92) {
            break;
        }
    }
    for (int j = 0; j <= 255; j++)
        PATH[j] = '\0';
    int j;
    for (j = 0; j <= i; j++) {
        PATH[j] = FilePath[j];
    }
    PATH[j] = '*'; PATH[j + 1] = '.'; PATH[j + 2] = '*';
    WIN32_FIND_DATA FileData;
    int count = 0;
    MultiByteToWideChar(CP_ACP, 0, PATH, strlen(PATH) + 1, P, sizeof(P) / sizeof(P[0]));
    //hFile = FindFirstFileW(L"C:\\Users\\12432\\Desktop\\ע���\\*.*", &FileData);
    hFile = FindFirstFileW(P, &FileData);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("û���漰����ļ���\n");
        exit(1);
    }
    while (FindNextFile(hFile, &FileData))
    {
        if (FileData.cFileName[0] == '.')   //����.��..
            continue;
        setlocale(LC_ALL, "");
        printf("�ļ�����%ls \n", FileData.cFileName);
        count++;
    }
    printf("Count of File :  %d\n", count);
    printf("**********************************\n\n");
    FindClose(hFile);
}



int IsMe(char* Name) {
    char Filename[256];
    GetModuleFileName(0, (LPWSTR)Filename, 256);
    if (strstr(Filename, ".\\Inject.exe")) {
        return 1;
    }
    else {
        return 0;
    }
}
static int (WINAPI* OldMesssageBoxW)(_In_opt_ HWND hWnd, _In_opt_ LPCWSTR lpText, _In_opt_ LPCWSTR lpCaption, _In_ UINT uType) = MessageBoxW;
static int (WINAPI* OldMesssageBoxA)(_In_opt_ HWND hWnd, _In_opt_ LPCSTR lpText, _In_opt_ LPCSTR lpCaption, _In_ UINT uType) = MessageBoxA;



extern "C" __declspec(dllexport)int WINAPI NewMesssageBoxA(_In_opt_ HWND hWnd, _In_opt_ LPCSTR lpText, _In_opt_ LPCSTR lpCaption, _In_ UINT uType)
{
    printf("\n\n*******************************************\n");
    printf("MessageBoxA Hooked\n");
    GetLocalTime(&st);
    printf("DLL��־�����%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    printf("********************************************\n\n");
    return OldMesssageBoxA(NULL, "new MessageBoxA", "Hooked", MB_OK);

}

extern "C" __declspec(dllexport)int WINAPI NewMesssageBoxW(_In_opt_ HWND hWnd, _In_opt_ LPCWSTR lpText, _In_opt_ LPCWSTR lpCaption, _In_ UINT uType)
{
    printf("\n\n*******************************************\n");
    printf("MessageBoxW Hooked\n");
    GetLocalTime(&st);
    printf("DLL��־�����%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    printf("********************************************\n\n");
    return OldMesssageBoxW(NULL, L"new MessageBoxW", L"Hooked", MB_OK);
}

static BOOL(WINAPI* OldCloseFILE)(HANDLE hObject) = CloseHandle;

extern "C" _declspec(dllexport)BOOL WINAPI NewCloseFILE(HANDLE hObject) {
    printf("\n\n*******************************************\n");
    printf("CloseHandle Hooked\n");
    GetLocalTime(&st);
    printf("DLL��־�����%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    printf("hObject :  0x%x\n", hObject);
    printf("********************************************\n\n");

    return OldCloseFILE(hObject);
}

static HANDLE(WINAPI* OldCreateFile)(
    LPCTSTR lpFileName,  //ָ���ļ�����ָ��
    DWORD dwDesiredAccesss,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile
    ) = CreateFile;

extern "C" __declspec(dllexport)HANDLE WINAPI NewCreateFile(
    LPCTSTR lpFileName,
    DWORD dwDesiredAccesss,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile
)
{
    printf("\n\n*******************************************\n");
    printf("CreateFile Hooked\n");
    GetLocalTime(&st);
    printf("DLL��־�����%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    int num = WideCharToMultiByte(CP_OEMCP, NULL, lpFileName, -1, NULL, 0, NULL, FALSE);
    char* pchar = new char[num];
    WideCharToMultiByte(CP_OEMCP, NULL, lpFileName, -1, pchar, num, NULL, FALSE);
    memset(Temp, 0, sizeof(Temp));
    /*for (int i = 1; i <= num; i++)
        Temp[i] = pchar[i];*/
    printf("lpFileName: %s\n", pchar);
    printf("dwDesiredAccesss :  0x%x\n", dwDesiredAccesss);
    printf("dwShareMode :  0x%x\n", dwShareMode);
    printf("lpSecurityAttributes :  0x%x\n", lpSecurityAttributes);
    printf("dwCreationDisposition :  0x%x\n", dwCreationDisposition);
    printf("dwFlagsAndAttributes :  0x%x\n", dwFlagsAndAttributes);
    printf("hTemplateFile :  0x%x\n", hTemplateFile);
    printf("********************************************\n\n");
    ChangeSeg(pchar);
    char szDrive[256];   //������
    char szDir[256];    //·����
    char szFname[256];  //�ļ���
    char szExt[256];   //��׺��
    _splitpath_s(pchar, szDrive, szDir, szFname, szExt);
    if (!strcmp(FileCopy, szFname)) {
        printf("!!!!���ڽ������Ҹ���!!!!\n");
    }
    //printf("%s",szFname);
    //else printf("û�з������Ҹ���!\n");
    if (!strcmp(szExt, EXE)) {
        printf("�޸�����չ��Ϊexe�Ŀ�ִ���ļ�\n");
    }
    else if (!strcmp(szExt,".dll")) {
        printf("�޸�����չ��Ϊdll�Ŀ�ִ���ļ�\n");
    }
    else if (!strcmp(szExt, ".ocx")) {
        printf("�޸�����չ��Ϊocx�Ŀ�ִ���ļ�\n");
    }
    else if (!strcmp(szExt, ".bat")) {
        printf("�޸�����չ��Ϊbat�Ŀ�ִ���ļ�\n");
    }

    return OldCreateFile(lpFileName, dwDesiredAccesss, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
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
    S = CreateSemaphore(NULL, 1, 1, NULL);//��ǰ1����Դ���������1��ͬʱ����()
    if (FileWriteFlag == true) {
        WaitForSingleObject(S, INFINITE);
        FileWriteFlag = false;
        printf("\n\n*******************************************\n");
        printf("WriteFile Hooked\n");
        GetLocalTime(&st);
        printf("DLL��־�����%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
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

static BOOL(WINAPI* OldReadFile)(
    HANDLE hFile, //�ļ��ľ����
    LPVOID lpBuffer, //���ڱ���������ݵ�һ����������
    DWORD nNumberOfBytesToRead, //ָ����ҪҪ������ַ�����
    LPDWORD lpNumberOfBytesRead, //����ʵ�ʶ�ȡ�ֽ�����ָ�롣
    LPOVERLAPPED lpOverlapped //���ļ���ʱָ����FILE_FLAG_OVERLAPPED��
// ��ô�����������������һ������Ľṹ���ýṹ������һ���첽��ȡ����������Ӧ�����������ΪNULL��
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
    printf("DLL��־�����%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    printf("lpBuffer :  %s\n", lpBuffer);
    printf("nNumberOfBytesToWrite :  0x%x\n", nNumberOfBytesToRead);
    printf("lpNumberOfBytesWritten :  0x%x\n", *lpNumberOfBytesRead);
    printf("********************************************\n\n");
    return OldReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
}


static HANDLE(WINAPI* OldHeapCreate)(DWORD fIOoptions, SIZE_T dwInitialSize, SIZE_T dwMaximunSize) = HeapCreate;

extern "C" __declspec(dllexport)HANDLE WINAPI NewHeapCreate(DWORD fIOoptions, SIZE_T dwInitialSize, SIZE_T dwMaximunSize)
{
    HANDLE hHeap = OldHeapCreate(fIOoptions, dwInitialSize, dwMaximunSize);
    printf("\n\n*******************************************\n");
    printf("HeapCreate Hooked\n");
    GetLocalTime(&st);
    printf("DLL��־�����%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    printf("fIOoptions :  0x%x\n", fIOoptions);
    printf("dwInitialSize :  0x%x\n", dwInitialSize);
    printf("dwMaximunSize :  0x%x\n", dwMaximunSize);
    printf("hHeap��ʼ��ַ :  0x%x\n", hHeap);
    printf("********************************************\n\n");
    HeapC.insert(hHeap);
    return hHeap;
}

static BOOL(WINAPI* OldHeapDestroy)(HANDLE) = HeapDestroy;

extern "C" __declspec(dllexport)BOOL WINAPI NewHeapDestroy(HANDLE hHeap)
{
    printf("\n\n*******************************************\n");
    printf("HeapDestroy Hooked\n");
    GetLocalTime(&st);
    printf("DLL��־�����%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    printf("hHeap :  0x%x\n", hHeap);
    printf("********************************************\n\n");
    HeapD = hHeap;
    std::set<HANDLE>::iterator it = HeapC.find(HeapD);
    if (it == std::end(HeapC)) {
        printf("!!!!!0x%x��ַ�Ķ��ظ��ͷ�!!!!!\n", hHeap);
        return 0;
    }
    else {
        HeapC.erase(it);
        return OldHeapDestroy(hHeap);
    }
}

static BOOL(WINAPI* OldHeapFree)(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem) = HeapFree;
extern "C" __declspec(dllexport) BOOL WINAPI NewHeapFree(
    HANDLE hHeap,
    DWORD dwFlags,
    LPVOID lpMem
)
{
    printf("\n\n*******************************************\n");
    printf("HeapFree Hooked\n");
    GetLocalTime(&st);
    printf("DLL��־�����%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    printf("hHeap :  0x%x\n", hHeap);
    printf("dwFlags :  %ul\n", dwFlags);
    printf("lpMew :  0x%x\n", lpMem);
    printf("********************************************\n\n");
    return OldHeapFree(hHeap, dwFlags, lpMem);
}


static LPVOID(WINAPI* OldHeapAlloc)(HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes) = HeapAlloc;

extern "C" __declspec(dllexport) LPVOID WINAPI NewHeapAlloc(
    HANDLE hHeap,
    DWORD dwFlags,
    SIZE_T dwBytes
)
{
    HeapAll = CreateSemaphore(NULL, 1, 1, 0);
    if (HeapAllocFlag == true) {
        WaitForSingleObject(HeapAll, INFINITE);
        HeapAllocFlag = false;
        printf("\n\n*******************************************\n");
        printf("HeapAlloc Hooked\n");
        GetLocalTime(&st);
        printf("DLL��־�����%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
        printf("hHeap :  0x%x\n", hHeap);
        printf("dwFlags :  %ul\n", dwFlags);
        printf("dwBytes :  %ul Bytes\n", dwBytes);
        printf("********************************************\n\n");
        HeapAllocFlag = true;
        ReleaseSemaphore(HeapAll, 1, NULL);
    }
    CloseHandle(HeapAll);
    return OldHeapAlloc(hHeap, dwFlags, dwBytes);

}

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
    printf("DLL��־�����%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    int num = WideCharToMultiByte(CP_OEMCP, NULL, lpSubKey, -1, NULL, 0, NULL, FALSE);
    char* pchar = new char[num];
    WideCharToMultiByte(CP_OEMCP, NULL, lpSubKey, -1, pchar, num, NULL, FALSE);
    //std::cout << "lpSubKey : " << pchar << std::endl;
    printf("lpSubKey : %s\n", pchar);
    /*if (*lpdwDisposition == 0x00000001L) {
        printf("�ü����´����ļ�\n");
    }
    else if (*lpdwDisposition == 0x00000002L)
    {
        printf("�ü����Ѿ����ڵļ�\n");
    }
    else
    {
        printf("δ֪�Ƿ��½�\n");
    }*/
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
    printf("DLL��־�����%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
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
   /* if (a == ERROR_SUCCESS)
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

                    if ((err = RegEnumValue(hkey, i, (LPWSTR)valueName, &valueLength, NULL, 		//����ö��ָ�����ֵ��
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
    }*/
    printf("\n\n*******************************************\n");
    printf("NewRegOpenKeyEx Hooked\n");
    CString str1(lpSubKey);

    CString str2(_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"));
    if (0 == str1.CompareNoCase(str2))
    {
        printf("����־�ļ�Ϊ����������\n");
    }
    else
    {
        printf("����־�ļ����ǿ���������\n");
    }
    printf("\n\n*******************************************\n");
    printf("NewRegOpenKeyEx Hooked\n");
    GetLocalTime(&st);
    printf("DLL��־�����%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
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
    if (a == ERROR_SUCCESS) printf("ע����޸ĳɹ�\n");
    //printf("\n\n*******************************************\n");
    //printf("NewRegSetValueEx Hooked\n");

    printf("\n\n*******************************************\n");
    printf("NewRegSetValueEx Hooked\n");
    GetLocalTime(&st);
    printf("DLL��־�����%d-%d-%d %02d: %02d: %02d: %03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    int num = WideCharToMultiByte(CP_OEMCP, NULL, lpValueName, -1, NULL, 0, NULL, FALSE);
    char* pchar = new char[num];
    WideCharToMultiByte(CP_OEMCP, NULL, lpValueName, -1, pchar, num, NULL, FALSE);
    printf("lpValueName : %s\n", pchar);
    printf("********************************************\n\n");
    return OldRegSetValueEx(hKey, lpValueName, Reserved, dwType, lpData, cbData);
}

//����ͨ�Ų���


BOOL WINAPI DllMain(HMODULE hModule,
    DWORD ul_reason_for_call,
    LPVOID lpReserved)
{
    char Buf[256];
    GetModuleFileName(0, (LPWSTR)Buf, 256);
    if (IsMe(Buf)) {
        return TRUE;
    }
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        DisableThreadLibraryCalls(hModule);
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourAttach(&(PVOID&)OldMesssageBoxW, NewMesssageBoxW);
        DetourAttach(&(PVOID&)OldMesssageBoxA, NewMesssageBoxA);
        DetourAttach(&(PVOID&)OldCreateFile, NewCreateFile);
        DetourAttach(&(PVOID&)OldCloseFILE, NewCloseFILE);
        DetourAttach(&(PVOID&)OldWriteFile, NewWriteFile);
        //DetourAttach(&(PVOID&)OldReadFile, NewReadFile);
        DetourAttach(&(PVOID&)OldHeapCreate, NewHeapCreate);
        //DetourAttach(&(PVOID&)OldHeapAlloc, NewHeapAlloc);
        //DetourAttach(&(PVOID&)OldHeapFree, NewHeapFree);
        DetourAttach(&(PVOID&)OldHeapDestroy, NewHeapDestroy);
        DetourAttach(&(PVOID&)OldRegCreateKeyEx, NewRegCreateKeyEx);
        DetourAttach(&(PVOID&)OldRegOpenKeyEx, NewRegOpenKeyEx);
        DetourAttach(&(PVOID&)OldRegSetValueEx, NewRegSetValueEx);
        DetourAttach(&(PVOID&)OldRegCloseKey, NewRegCloseKey);
        DetourTransactionCommit();//������Ч
    }
    break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourDetach(&(PVOID&)OldMesssageBoxW, NewMesssageBoxW);
        DetourDetach(&(PVOID&)OldMesssageBoxA, NewMesssageBoxA);
        DetourDetach(&(PVOID&)OldCreateFile, NewCreateFile);
        DetourDetach(&(PVOID&)OldCloseFILE, NewCloseFILE);
        //DetourDetach(&(PVOID&)OldReadFile, NewReadFile);
        DetourDetach(&(PVOID&)OldWriteFile, NewWriteFile);
        DetourDetach(&(PVOID&)OldHeapCreate, NewHeapCreate);
        //DetourDetach(&(PVOID&)OldHeapAlloc, NewHeapAlloc);
        //DetourDetach(&(PVOID&)OldHeapFree, NewHeapFree);
        DetourDetach(&(PVOID&)OldHeapDestroy, NewHeapDestroy);
        DetourDetach(&(PVOID&)OldRegCreateKeyEx, NewRegCreateKeyEx);
        DetourDetach(&(PVOID&)OldRegCloseKey, NewRegCloseKey);
        DetourDetach(&(PVOID&)OldRegOpenKeyEx, NewRegOpenKeyEx);
        DetourDetach(&(PVOID&)OldRegSetValueEx, NewRegSetValueEx);
        DetourTransactionCommit();//������Ч
        break;
    default:
        break;
    }
    return TRUE;
}


