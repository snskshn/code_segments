#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <winsock2.h>
#include <windows.h>

#define BUF_SIZE 100
#define MAX_CLNT 256
#define READ	3
#define	WRITE	5

typedef struct    // socket info
{
	SOCKET hClntSock;
	SOCKADDR_IN clntAdr;
} PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

typedef struct    // buffer info
{
	OVERLAPPED overlapped;
	WSABUF wsaBuf;
	char buffer[BUF_SIZE];
} PER_IO_DATA, *LPPER_IO_DATA;

DWORD WINAPI EchoThreadMain(LPVOID CompletionPortIO);
void SendMsg(char * msg, DWORD len);
void ErrorHandling(char *message);

int clntCnt=0;
SOCKET clntSocks[MAX_CLNT];

int main(int argc, char* argv[])
{
	WSADATA	wsaData;
	HANDLE hComPort;	
	SYSTEM_INFO sysInfo;
	LPPER_IO_DATA ioInfo;
	LPPER_HANDLE_DATA handleInfo;

	SOCKET hServSock;
	SOCKADDR_IN servAdr;
	int recvBytes, i, flags=0;

	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!"); 

	hComPort=CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	GetSystemInfo(&sysInfo);

	for(i=0; i<sysInfo.dwNumberOfProcessors; i++)
		_beginthreadex(NULL, 0, EchoThreadMain, (LPVOID)hComPort, 0, NULL);

	hServSock=WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	servAdr.sin_family=AF_INET;
	servAdr.sin_addr.s_addr=htonl(INADDR_ANY);
	servAdr.sin_port=htons(atoi(argv[1]));

	bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr));
	listen(hServSock, 5);
	
	while(1)
	{	
		SOCKET hClntSock;
		SOCKADDR_IN clntAdr;		
		int addrLen=sizeof(clntAdr);
		
		hClntSock=accept(hServSock, (SOCKADDR*)&clntAdr, &addrLen);
		clntSocks[clntCnt++]=hClntSock;

		handleInfo=(LPPER_HANDLE_DATA)malloc(sizeof(PER_HANDLE_DATA));		
		handleInfo->hClntSock=hClntSock;
		memcpy(&(handleInfo->clntAdr), &clntAdr, addrLen);

		CreateIoCompletionPort((HANDLE)hClntSock, hComPort, (DWORD)handleInfo, 0);
		
		ioInfo=(LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
		memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));		
		ioInfo->wsaBuf.len=BUF_SIZE;
		ioInfo->wsaBuf.buf=ioInfo->buffer;

		WSARecv(handleInfo->hClntSock,	&(ioInfo->wsaBuf),	
			1, &recvBytes, &flags, &(ioInfo->overlapped), NULL);			
	}
	return 0;
}

DWORD WINAPI EchoThreadMain(LPVOID pComPort)
{
	HANDLE hComPort=(HANDLE)pComPort;
	SOCKET sock;
	DWORD bytesTrans;
	LPPER_HANDLE_DATA handleInfo;
	LPPER_IO_DATA ioInfo;
	DWORD flags=0;
	int i;
	
	while(1)
	{ 
		GetQueuedCompletionStatus(hComPort, &bytesTrans, 
			(LPDWORD)&handleInfo, (LPOVERLAPPED*)&ioInfo, INFINITE);
		sock=handleInfo->hClntSock;

		puts("message received!");
		if(bytesTrans==0)    // EOF Àü¼Û ½Ã
		{
			for(i=0; i<clntCnt; i++)   // remove disconnected client
			{
				if(sock==clntSocks[i])
				{
					while(i++<clntCnt-1)
					clntSocks[i]=clntSocks[i+1];
					break;
				}
			}
			clntCnt--;

			closesocket(sock);
			free(handleInfo); free(ioInfo);
			continue;		
		}

		SendMsg(ioInfo->buffer, bytesTrans);

		memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));			
		WSARecv(sock, &(ioInfo->wsaBuf), 
			1, NULL, &flags, &(ioInfo->overlapped), NULL);
	}
	return 0;
}

void SendMsg(char * msg, DWORD len)   // send to all
{
	int i;
	for(i=0; i<clntCnt; i++)
		send(clntSocks[i], msg, len, 0);
}
void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}