#include <stdio.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")
int main()
{
	char szIPAddress[16];
	printf("��������Ҫ���ӵķ�����IP��ַ:\n");
	scanf("%s", szIPAddress);


	//1.�����׽��ֿ⣬��ʼ��
	WSADATA wd;
	if (0 != WSAStartup(MAKEWORD(2, 2), &wd))
	{
		printf("�����׽��ֿ�ʧ��,�������:%d\n", GetLastError());
		return 0;
	}

	//2.�ж�����İ汾�Ƿ�һ��
	if (LOBYTE(wd.wVersion) != 2 || HIBYTE(wd.wVersion) != 2)
	{
		printf("������׽��ְ汾��һ�£��������:%d\n", GetLastError());
		return 0;
	}

	//3.�����׽���
	//TCP/IP => TCP, UDP
	//TCP: �ɿ����䣬���뽨������ SOCK_STREAM
	//UDP: ���ɿ����䣬���Բ���Ҫ�������� SOCK_DGRAM
	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);
	//AF_INET,����IPv4  SOCK_STREAM�������TCPЭ�飬0Ĭ��ֵ
	if (sockClient == INVALID_SOCKET)
	{
		printf("�����׽���ʧ��,�������:%d\n", GetLastError());
		return 0;
	}

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(szIPAddress);//���ַ���תΪIP��ַ
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6940); //h:host   n:net   s:short


	//4. ���ӷ�����
	if (SOCKET_ERROR == connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)))
	{
		printf("���ӷ�����ʧ�ܣ��������:%d\n", GetLastError());
		return 0;
	}

	//������Ϣ
	char szRecvMsg[100];
	memset(szRecvMsg, 0, 100);


	recv(sockClient, szRecvMsg, 100, 0);
	printf("������˵: %s\n", szRecvMsg);

	while (1)
	{
		//������Ϣ
		memset(szRecvMsg, 0, 100);
		recv(sockClient, szRecvMsg, 100, 0);

		//�Ƿ��ǽ�����־
		if (strcmp(szRecvMsg, "<QUIT>") == 0)
		{
			printf("�������������죬������ֹ��\n");
			//�ر��׽���
			closesocket(sockClient);
			break;
		}
		else
		{
			printf("��������˵��: %s\n", szRecvMsg);
		}


		//������Ϣ
		char szSendMsg[100];
		printf("����������͵���Ϣ����q�˳���: ");
		scanf("%s", szSendMsg);

		if (strcmp(szSendMsg, "q") == 0)
		{
			//�ȷ��ͽ�����������
			strcpy(szSendMsg, "<QUIT>");
			send(sockClient, szSendMsg, strlen(szSendMsg), 0);
			printf("�������졣\n");
			closesocket(sockClient);
			break;
		}

		//��������
		send(sockClient, szSendMsg, strlen(szSendMsg), 0);
	}
	return 0;
}