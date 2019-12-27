#include <stdio.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")
int main()
{
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
	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);
	//AF_INET,代表IPv4  SOCK_STREAM代表采用TCP协议，0默认值
	if (sockSrv == INVALID_SOCKET)
	{
		printf("创建套接字失败,错误代码:%d\n", GetLastError());
		return 0;
	}

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY); //l: long
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6940); //h:host   n:net   s:short

	//4. 绑定套接字
	if (SOCKET_ERROR == bind(sockSrv, (sockaddr*)&addrSrv, sizeof(addrSrv)))
	{
		printf("绑定套接字失败,错误代码:%d\n", GetLastError());
		return 0;
	}

	//5.将套接字设置为监听模式
	if (SOCKET_ERROR == listen(sockSrv, 5))
	{
		printf("监听失败,错误代码:%d\n", GetLastError());
		return 0;
	}

	//6.等待客户端的连接请求到来
	SOCKADDR_IN addrClient; //客户端地址
	int length = sizeof(SOCKADDR);
	while (1)
	{
		printf("正在等待客户端的连接...\n");
		//返回一个已经建立连接的套接字,阻塞函数。
		//inet_ntoa  将ip转换为字符串
		SOCKET sockConn = accept(sockSrv, (SOCKADDR*)&addrClient, &length);
		printf("欢迎客户端:%s:%d连接服务器\n", inet_ntoa(addrClient.sin_addr), addrClient.sin_port);

		//发送信息给客户端(发给建立连接的套接字)
		char szSendMsg[100];
		sprintf(szSendMsg, "【欢迎%s进入聊天系统】", inet_ntoa(addrClient.sin_addr));
		send(sockConn, szSendMsg, strlen(szSendMsg), 0);

		while (1)
		{
			printf("请输入待发送的信息【按q退出】: ");
			scanf("%s", szSendMsg);

			if (strcmp(szSendMsg, "q") == 0)
			{
				//先发送结束聊天命令
				strcpy(szSendMsg, "<QUIT>");
				send(sockConn, szSendMsg, strlen(szSendMsg), 0);
				printf("结束聊天。\n");
				closesocket(sockConn);
				break;
			}
			
			//发送数据
			send(sockConn, szSendMsg, strlen(szSendMsg), 0);


			//接受数据
			char szRecvMsg[100] = { 0 };
			recv(sockConn, szRecvMsg, 100, 0);

			if (strcmp(szRecvMsg, "<QUIT>") == 0)
			{
			
				printf("客户端关闭聊天,聊天终止。\n");
				closesocket(sockConn);
				break;
			}
			else
			{
				printf("【客户端说】:%s\n", szRecvMsg);
			}

		}
	}
	return 0;
}