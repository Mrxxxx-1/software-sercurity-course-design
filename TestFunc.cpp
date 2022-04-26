#include<stdio.h>
#include<windows.h>
#include <stdlib.h>
//#include <WinSock.h>
#pragma comment(lib, "ws2_32.lib")  //���� ws2_32.dll


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
        printf("�����빦�����\n");
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

//ѡ��˵�
void Menu()
{
    printf("1��2.��������   3.�Ѳ���  4.�ļ�������д��  5.�ļ���ȡ \n");
    printf("6.ע���������ֵ��ȡ  7.ע���򿪣�ɾ��  8,9.��Ϣ���ͽ��� \n");
    printf("\n");
}

//�ѵĴ������ͷ�
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

//�ļ��Ĵ������رա���д
void Fileread()
{
   
        //�ļ���С
        HANDLE hFileRead;
        //�����ļ���С
        LARGE_INTEGER liFileSize;
        //�ɹ���ȡ���ļ����ݴ�С
        DWORD dwReadedSize;
        //�ۼӼ����Ѿ���ȡ�����ݵĴ�С
        LONGLONG liTotalRead = 0;
        //�ļ����ݻ���
        BYTE lpFileDataBuffer[32];

        //���Ѿ����ڵ��ļ�����ȡ����
        hFileRead = CreateFileA("D:\\show.txt",//name
            GENERIC_READ,                   //�Զ���ʽ��
            FILE_SHARE_READ,               //�ɹ����
            NULL,                           //Ĭ�ϰ�ȫ����
            OPEN_EXISTING,                   //ֻ���Ѿ����ڵ��ļ�
            FILE_ATTRIBUTE_NORMAL,           //�����ļ�����
            NULL);                           //��ģ��
        //���ļ��Ƿ�ɹ�
        if (hFileRead == INVALID_HANDLE_VALUE)
        {
            printf("���ļ�ʧ��: %d", GetLastError());
        }
        if (!GetFileSizeEx(hFileRead, &liFileSize))
        {
            printf("��ȡ�ļ���Сʧ�ܣ� %d", GetLastError());
        }
        else
        {
            printf("�ļ���СΪ�� %d\n", liFileSize.QuadPart);
        }
        //ѭ����ȡ����ӡ����
        while (TRUE)
        {
            DWORD i;
            if (!ReadFile(hFileRead,//��ȡ�ļ����
                lpFileDataBuffer,  //�洢��ȡ�ļ�����
                32,                   //��ȡ�Ĵ�С(�ֽ�)
                &dwReadedSize,       //ʵ�ʶ�ȡ�Ĵ�С
                NULL))               //��ʹ��Overlapped
            {
                printf("��ȡ�ļ����� %d\n", GetLastError());
                break;
            }
            printf("��ȡ��%d�ֽڣ��ļ������ǣ� ", dwReadedSize);

            for (i = 0;i < dwReadedSize;i++)
            {
                printf("%c", lpFileDataBuffer[i]);
            }
            printf("\n");
            liTotalRead += dwReadedSize;
            if (liTotalRead == liFileSize.QuadPart)
            {
                printf("��ȡ�ļ�����\n");
                break;
            }
        }
        CloseHandle(hFileRead);
        //return 0;

}

void Filewrite()
{
    //LPSTR szFileData = "test";
    //�ļ����
    HANDLE hFileWrite;
    //�ɹ�д������ݴ�С
    DWORD dwWritedDateSize;
    //���Ѿ����ڵ��ļ�����ȡ����
    hFileWrite = CreateFileA("D:\\show.txt",//Ҫ�򿪵��ļ���
        GENERIC_WRITE,//��д��ʽ�򿪿�
        0,//�ɹ����
        NULL,//Ĭ�ϰ�ȫ����
        OPEN_ALWAYS,//���Ѿ����ڵ��ļ�
        FILE_ATTRIBUTE_NORMAL,//�������Դ�
        NULL);//��ģ��
    //�ж��Ƿ�ɹ���
    if (hFileWrite == INVALID_HANDLE_VALUE)
    {
        printf("���ļ�ʧ�ܣ� %d\n", GetLastError());
    }
    //�����ļ�ָ�뵽�ļ�Ϊ
    SetFilePointer(hFileWrite, 0, 0, FILE_END);
    //������д���ļ�
    if (!WriteFile(hFileWrite, "test", lstrlenA("test"), &dwWritedDateSize, NULL))
    {
        printf("д�ļ�ʧ�ܣ� %d\n", GetLastError());
    }
    else
    {
        printf("д�ļ��ɹ���д��%d�ֽڡ�\n", dwWritedDateSize);
    }
    CloseHandle(hFileWrite);
    //return 0;
}
//ע���Ĵ������رա���д
void Regcreate()
{
    // ����ע������ü�ֵ
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
    // �޸�ע����ֵ��û���򴴽���
    size_t iLen = wcslen(Data);
    // ���ü�ֵ
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

//Socket�������ͷš����ӡ����ݷ��������
void Socketsend()
{
    //��ʼ�� DLL
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    //�����׽���
    SOCKET servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    //���׽���
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));  //ÿ���ֽڶ���0���
    sockAddr.sin_family = PF_INET;  //ʹ��IPv4��ַ
    sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //�����IP��ַ
    sockAddr.sin_port = htons(1234);  //�˿�
    bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
    //�������״̬
    listen(servSock, 20);
    //���տͻ�������
    SOCKADDR clntAddr;
    int nSize = sizeof(SOCKADDR);
    SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);
    //��ͻ��˷�������
    char str[32] = "Hello World!";
    send(clntSock, str, strlen(str) + sizeof(char), NULL);
    //�ر��׽���
    closesocket(clntSock);
    closesocket(servSock);
    //��ֹ DLL ��ʹ��
    WSACleanup();
}

void Socketrec()
{
    //��ʼ��DLL
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    //�����׽���
    SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    //���������������
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));  //ÿ���ֽڶ���0���
    sockAddr.sin_family = PF_INET;
    sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sockAddr.sin_port = htons(1234);
    connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
    Sleep(500);
    //���շ��������ص�����
    char szBuffer[MAXBYTE] = { 0 };
    recv(sock, szBuffer, MAXBYTE, NULL);
    //������յ�������
    printf("Message from server: %s\n", szBuffer);
    //�ر��׽���
    closesocket(sock);
    //��ֹʹ�� DLL
    WSACleanup();
}

void openAnotherFolder() 
{
    HANDLE hOpenFile = (HANDLE)CreateFile(L"D:\\test1.dll", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, NULL, NULL);
    CloseHandle(hOpenFile);
}