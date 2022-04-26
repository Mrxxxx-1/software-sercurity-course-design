#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>
void Regcreate()
{
    // 创建注册表并设置键值
    HKEY hKey = NULL;
    TCHAR Data[254];
    memset(Data, 0, sizeof(Data));
    wcsncpy_s(Data, TEXT("test reg"), 254);

    size_t lRet = RegCreateKeyEx(HKEY_CURRENT_USER, (LPWSTR)L"testkey", 0, NULL, REG_OPTION_NON_VOLATILE,
        KEY_ALL_ACCESS, NULL, &hKey, NULL);
    if (lRet == ERROR_SUCCESS) {
        //printf("create successfully!\n");
    }
    else {
        //printf("failed to create!\n");
    }
    // 修改注册表键值，没有则创建它
    size_t iLen = wcslen(Data);
    // 设置键值
    lRet = RegSetValueEx(hKey, L"value", 0, REG_SZ, (CONST BYTE*)Data, sizeof(TCHAR) * iLen);
    if (lRet == ERROR_SUCCESS)
    {

        //printf("set value successfully!\n");
        return;
    }
    else {
        //printf("failed to set value!\n");
    }
    RegCloseKey(hKey);
}

void Regopen()
{
    HKEY hKey = NULL;
    size_t lRet = RegOpenKeyEx(HKEY_CURRENT_USER, (LPWSTR)L"testkey", 0, KEY_ALL_ACCESS, &hKey);
    if (lRet == ERROR_SUCCESS) {
        //printf("open successfully!\n");
    }
    else {
        //printf("open failed\n");
    }
    lRet = RegDeleteValue(hKey, L"value");
    if (lRet == ERROR_SUCCESS) {
        //printf("delete success!\n");
    }
    else {
        //printf("delete fail!\n");
    }
    RegCloseKey(hKey);
}


int main()
{
    Regcreate();
    Regopen();
	system("pause");
	return 0;
}