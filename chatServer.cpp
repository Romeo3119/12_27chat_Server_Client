#include <stdio.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")
int main()
{
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
	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);
	//AF_INET,����IPv4  SOCK_STREAM�������TCPЭ�飬0Ĭ��ֵ
	if (sockSrv == INVALID_SOCKET)
	{
		printf("�����׽���ʧ��,�������:%d\n", GetLastError());
		return 0;
	}

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY); //l: long
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6940); //h:host   n:net   s:short

	//4. ���׽���
	if (SOCKET_ERROR == bind(sockSrv, (sockaddr*)&addrSrv, sizeof(addrSrv)))
	{
		printf("���׽���ʧ��,�������:%d\n", GetLastError());
		return 0;
	}

	//5.���׽�������Ϊ����ģʽ
	if (SOCKET_ERROR == listen(sockSrv, 5))
	{
		printf("����ʧ��,�������:%d\n", GetLastError());
		return 0;
	}

	//6.�ȴ��ͻ��˵�����������
	SOCKADDR_IN addrClient; //�ͻ��˵�ַ
	int length = sizeof(SOCKADDR);
	while (1)
	{
		printf("���ڵȴ��ͻ��˵�����...\n");
		//����һ���Ѿ��������ӵ��׽���,����������
		//inet_ntoa  ��ipת��Ϊ�ַ���
		SOCKET sockConn = accept(sockSrv, (SOCKADDR*)&addrClient, &length);
		printf("��ӭ�ͻ���:%s:%d���ӷ�����\n", inet_ntoa(addrClient.sin_addr), addrClient.sin_port);

		//������Ϣ���ͻ���(�����������ӵ��׽���)
		char szSendMsg[100];
		sprintf(szSendMsg, "����ӭ%s��������ϵͳ��", inet_ntoa(addrClient.sin_addr));
		send(sockConn, szSendMsg, strlen(szSendMsg), 0);

		while (1)
		{
			printf("����������͵���Ϣ����q�˳���: ");
			scanf("%s", szSendMsg);

			if (strcmp(szSendMsg, "q") == 0)
			{
				//�ȷ��ͽ�����������
				strcpy(szSendMsg, "<QUIT>");
				send(sockConn, szSendMsg, strlen(szSendMsg), 0);
				printf("�������졣\n");
				closesocket(sockConn);
				break;
			}
			
			//��������
			send(sockConn, szSendMsg, strlen(szSendMsg), 0);


			//��������
			char szRecvMsg[100] = { 0 };
			recv(sockConn, szRecvMsg, 100, 0);

			if (strcmp(szRecvMsg, "<QUIT>") == 0)
			{
			
				printf("�ͻ��˹ر�����,������ֹ��\n");
				closesocket(sockConn);
				break;
			}
			else
			{
				printf("���ͻ���˵��:%s\n", szRecvMsg);
			}

		}
	}
	return 0;
}