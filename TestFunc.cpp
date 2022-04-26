#include<stdio.h>
#include<windows.h>
#include <stdlib.h>
//#include <WinSock.h>
#pragma comment(lib, "ws2_32.lib")  //加载 ws2_32.dll


void Menu();
void Heap();
void Fileread();
void Filewrite();
void Regcreate();
void Regopen();
void Socketsend();
void Socketrec();
void openAnotherFolder();

int main()
{
	int i = -1;
	do
	{
        Menu();
        printf("请输入功能序号\n");
        scanf("%d", &i);
        getchar();
		switch (i)
		{
		case 0:
			printf("bye\n");
            system("pause");
			break;
		case 1:
			MessageBoxA(NULL, "MessageBoxA", "Test", MB_OK);
			break;
		case 2:
			MessageBoxW(NULL, L"MessageBoxW",L"Test", MB_OK);
			break;
		case 3:
			Heap();
			break;
		case 4:
            Filewrite();
			break;
        case 5:
            Fileread();
            break;
        case 6:
            Regcreate();
            break;
        case 7:
            Regopen();
            break;
        case 8:
            Socketsend();
            break;
        case 9:
            Socketrec();
            break;
        case 10:
            openAnotherFolder();
            break;
		default:
			printf("error!\n");
			break;
		}
	} while (i != 0);
	return 0;
}

//选项菜单
void Menu()
{
    printf("1，2.弹窗操作   3.堆操作  4.文件创建，写入  5.文件读取 \n");
    printf("6.注册表创建，键值读取  7.注册表打开，删除  8,9.信息发送接收 \n");
    printf("\n");
}

//堆的创建，释放
void Heap()
{
	HANDLE hHeap = HeapCreate(HEAP_NO_SERIALIZE, 0, 0);
	if (hHeap == NULL)
	{
		printf("erro!\n");
		return ;
	}
	void *p = HeapAlloc(hHeap, 0, sizeof(int));
	if (p != NULL)
		printf("HeapCreate success\n");
	if(HeapFree(hHeap, 0, p))
		printf("Successfully free!\n");
	if(HeapDestroy(hHeap))
		printf("Successfully destory!\n");
}

//文件的创建、关闭、读写
void Fileread()
{
   
        //文件大小
        HANDLE hFileRead;
        //保存文件大小
        LARGE_INTEGER liFileSize;
        //成功读取的文件数据大小
        DWORD dwReadedSize;
        //累加计算已经读取的数据的大小
        LONGLONG liTotalRead = 0;
        //文件数据缓存
        BYTE lpFileDataBuffer[32];

        //打开已经存在的文件，读取内容
        hFileRead = CreateFileA("D:\\show.txt",//name
            GENERIC_READ,                   //以读方式打开
            FILE_SHARE_READ,               //可共享读
            NULL,                           //默认安全设置
            OPEN_EXISTING,                   //只打开已经存在的文件
            FILE_ATTRIBUTE_NORMAL,           //常规文件属性
            NULL);                           //无模板
        //打开文件是否成功
        if (hFileRead == INVALID_HANDLE_VALUE)
        {
            printf("打开文件失败: %d", GetLastError());
        }
        if (!GetFileSizeEx(hFileRead, &liFileSize))
        {
            printf("获取文件大小失败： %d", GetLastError());
        }
        else
        {
            printf("文件大小为： %d\n", liFileSize.QuadPart);
        }
        //循环读取并打印内容
        while (TRUE)
        {
            DWORD i;
            if (!ReadFile(hFileRead,//读取文件句柄
                lpFileDataBuffer,  //存储读取文件内容
                32,                   //读取的大小(字节)
                &dwReadedSize,       //实际读取的大小
                NULL))               //不使用Overlapped
            {
                printf("读取文件错误： %d\n", GetLastError());
                break;
            }
            printf("读取了%d字节，文件内容是： ", dwReadedSize);

            for (i = 0;i < dwReadedSize;i++)
            {
                printf("%c", lpFileDataBuffer[i]);
            }
            printf("\n");
            liTotalRead += dwReadedSize;
            if (liTotalRead == liFileSize.QuadPart)
            {
                printf("读取文件结束\n");
                break;
            }
        }
        CloseHandle(hFileRead);
        //return 0;

}

void Filewrite()
{
    //LPSTR szFileData = "test";
    //文件句柄
    HANDLE hFileWrite;
    //成功写入的数据大小
    DWORD dwWritedDateSize;
    //打开已经存在的文件，读取内容
    hFileWrite = CreateFileA("D:\\show.txt",//要打开的文件名
        GENERIC_WRITE,//以写方式打开开
        0,//可共享读
        NULL,//默认安全设置
        OPEN_ALWAYS,//打开已经存在的文件
        FILE_ATTRIBUTE_NORMAL,//常规属性打开
        NULL);//无模板
    //判断是否成功打开
    if (hFileWrite == INVALID_HANDLE_VALUE)
    {
        printf("打开文件失败： %d\n", GetLastError());
    }
    //设置文件指针到文件为
    SetFilePointer(hFileWrite, 0, 0, FILE_END);
    //将数据写入文件
    if (!WriteFile(hFileWrite, "test", lstrlenA("test"), &dwWritedDateSize, NULL))
    {
        printf("写文件失败： %d\n", GetLastError());
    }
    else
    {
        printf("写文件成功，写入%d字节。\n", dwWritedDateSize);
    }
    CloseHandle(hFileWrite);
    //return 0;
}
//注册表的创建、关闭、读写
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

//Socket创建、释放、连接、数据发送与接受
void Socketsend()
{
    //初始化 DLL
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    //创建套接字
    SOCKET servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    //绑定套接字
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
    sockAddr.sin_family = PF_INET;  //使用IPv4地址
    sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
    sockAddr.sin_port = htons(1234);  //端口
    bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
    //进入监听状态
    listen(servSock, 20);
    //接收客户端请求
    SOCKADDR clntAddr;
    int nSize = sizeof(SOCKADDR);
    SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);
    //向客户端发送数据
    char str[32] = "Hello World!";
    send(clntSock, str, strlen(str) + sizeof(char), NULL);
    //关闭套接字
    closesocket(clntSock);
    closesocket(servSock);
    //终止 DLL 的使用
    WSACleanup();
}

void Socketrec()
{
    //初始化DLL
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    //创建套接字
    SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    //向服务器发起请求
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
    sockAddr.sin_family = PF_INET;
    sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sockAddr.sin_port = htons(1234);
    connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
    Sleep(500);
    //接收服务器传回的数据
    char szBuffer[MAXBYTE] = { 0 };
    recv(sock, szBuffer, MAXBYTE, NULL);
    //输出接收到的数据
    printf("Message from server: %s\n", szBuffer);
    //关闭套接字
    closesocket(sock);
    //终止使用 DLL
    WSACleanup();
}

void openAnotherFolder() 
{
    HANDLE hOpenFile = (HANDLE)CreateFile(L"D:\\test1.dll", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, NULL, NULL);
    CloseHandle(hOpenFile);
}