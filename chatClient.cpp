#include <stdio.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")
int main()
{
	char szIPAddress[16];
	printf("请输入需要连接的服务器IP地址:\n");
	scanf("%s", szIPAddress);


	//1.加载套接字库，初始化
	WSADATA wd;
	if (0 != WSAStartup(MAKEWORD(2, 2), &wd))
	{
		printf("加载套接字库失败,错误代码:%d\n", GetLastError());
		return 0;
	}

	//2.判断请求的版本是否一致
	if (LOBYTE(wd.wVersion) != 2 || HIBYTE(wd.wVersion) != 2)
	{
		printf("请求的套接字版本不一致，错误代码:%d\n", GetLastError());
		return 0;
	}

	//3.创建套接字
	//TCP/IP => TCP, UDP
	//TCP: 可靠传输，必须建立连接 SOCK_STREAM
	//UDP: 不可靠传输，可以不需要建立连接 SOCK_DGRAM
	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);
	//AF_INET,代表IPv4  SOCK_STREAM代表采用TCP协议，0默认值
	if (sockClient == INVALID_SOCKET)
	{
		printf("创建套接字失败,错误代码:%d\n", GetLastError());
		return 0;
	}

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(szIPAddress);//将字符串转为IP地址
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6940); //h:host   n:net   s:short


	//4. 连接服务器
	if (SOCKET_ERROR == connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)))
	{
		printf("连接服务器失败，错误代码:%d\n", GetLastError());
		return 0;
	}

	//接受信息
	char szRecvMsg[100];
	memset(szRecvMsg, 0, 100);


	recv(sockClient, szRecvMsg, 100, 0);
	printf("服务器说: %s\n", szRecvMsg);

	while (1)
	{
		//接受信息
		memset(szRecvMsg, 0, 100);
		recv(sockClient, szRecvMsg, 100, 0);

		//是否是结束标志
		if (strcmp(szRecvMsg, "<QUIT>") == 0)
		{
			printf("服务器结束聊天，聊天终止。\n");
			//关闭套接字
			closesocket(sockClient);
			break;
		}
		else
		{
			printf("【服务器说】: %s\n", szRecvMsg);
		}


		//发送信息
		char szSendMsg[100];
		printf("请输入待发送的信息【按q退出】: ");
		scanf("%s", szSendMsg);

		if (strcmp(szSendMsg, "q") == 0)
		{
			//先发送结束聊天命令
			strcpy(szSendMsg, "<QUIT>");
			send(sockClient, szSendMsg, strlen(szSendMsg), 0);
			printf("结束聊天。\n");
			closesocket(sockClient);
			break;
		}

		//发送数据
		send(sockClient, szSendMsg, strlen(szSendMsg), 0);
	}
	return 0;
}