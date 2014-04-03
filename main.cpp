//Include libraries and headers
#include<iostream>
#include<winsock.h>
#include<windows.h>
#include<ctime>
#include<errno.h>
#include<string>
#include<stdio.h>
#include<strsafe.h>
#include<winbase.h>
#include<cmath>

using namespace std;

#include"vars.h" //include global variables defs

//initialize global variables
volatile unsigned int socknum;
volatile unsigned int socknum2;
HANDLE threadhandle;
HANDLE threadhandle2;
volatile int killserv;
unsigned long int sessionnum;
int matrixdisable;
int usersaydisable;
int sbeepdisable;
int gasciidisable;
int clsdisable;
int cbeepdisable;
int slinedisable;
int portnum;

int main(int argc, char *argv[])
{
	//Set up variables used for command line argument handling and main connection listener loop
	socknum = 0;
	socknum2 = 0;
	killserv = 0;
	sessionnum = 0;
	matrixdisable = 0;
	usersaydisable = 0;
	sbeepdisable = 0;
	gasciidisable = 0;
	clsdisable = 0;
	cbeepdisable = 0;
	slinedisable = 0;
	portnum = 23;

	//Process command line arguments
	if(argc>1)
	{
		for(int a=1; a<argc; a++)
		{
			if(strcmp(argv[a], "--nomatrix")==0)
				matrixdisable=1;
			else if(strcmp(argv[a], "--nousersay")==0)
				usersaydisable=1;
			else if(strcmp(argv[a], "--noserverbeep")==0)
				sbeepdisable=1;
			else if(strcmp(argv[a], "--noascii")==0)
				gasciidisable=1;
			else if(strcmp(argv[a], "--nocls")==0)
				clsdisable=1;
			else if(strcmp(argv[a], "--acddemo")==0)
			{
				clsdisable=1;
				matrixdisable=1;
				usersaydisable=1;
				gasciidisable=1;
				sbeepdisable=1;
				cbeepdisable=1;
				slinedisable=1;
			}
			//Output help for command line arguments
			else if(strcmp(argv[a], "--help")==0)
			{
				cout << "Usage: " << endl << endl << "--nomatrix" << endl << "	Disables Matrix Command" << endl << endl;
				cout << "--nousersay" << endl << "	Disables Usersay Command" << endl << endl;
				cout << "--noserverbeep" << endl << "	Disables Server Beep Command" << endl << endl;
				cout << "--noascii" << endl << "	Disables Ascii Art Command" << endl << endl;
				cout << "--nocls" << endl << "	Disables the cls and clear functions" << endl << endl;
				cout << "--acddemo" << endl << "	Disables all commands (demo mode)" << endl << endl;
				cout << "--help" << endl << "	Displayes this screen" << endl << endl;
				return 0;
			}
			else
			{
				cout << "Invalid Option" << endl << endl;
				return 0;
			}
		}
	}
	srand(time(NULL));
	int tdata=0;
	threadhandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)conscmd, &tdata, 0, NULL); //Spawn thread for server console input monitoring
	//Prepare winsock for networking
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0)
	{
		fprintf(stderr, "WSAStartup failed.\n\r");
		exit(1);
	}
	//Set up network listener socket, prepare to receive connections
	sockaddr recvsock;
	sockaddr_in recvsock_in;
	recvsock.sa_family = AF_INET;
	recvsock_in.sin_family = AF_INET;
	recvsock_in.sin_port = htons(portnum);
	recvsock_in.sin_addr.s_addr = INADDR_ANY;
	memset(recvsock_in.sin_zero, '\0', sizeof(recvsock_in.sin_zero));
	socknum = socket(PF_INET, SOCK_STREAM, 0);
	//Error handling for socket setup
	if(socknum == -1)
	{
		perror("Socket Error");
	}
	if(bind(socknum, (struct sockaddr *)&recvsock_in, sizeof recvsock_in) == -1)
	{
		perror("Bind Error");
	}
	int backlog=20;
	if(listen(socknum, backlog) == -1)
	{
			perror("Listen Error");
	}
	cout << "Server Started" << endl; //Indicate startup completion
	//Loop to receive new connections
	while(killserv==0)
	{
		int len=sizeof(recvsock_in);
		socknum2=accept(socknum, (struct sockaddr *) &recvsock, &len);
		threadhandle2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)handlerthread, &tdata, 0, NULL); //Spawn handler thread for new connection
		if (killserv == 0)
		{
			if (socknum2 == -1)
			{
				perror("Accept Error");
			}
			sessionnum++;
			cout << "User Connected on socket number: " << socknum2 << "	Session Number: " << sessionnum << endl;
		}
	}
	//Network cleanup and shutdown
	closesocket(socknum);
	WSACleanup();
	cout << "Server Down" << endl; //Confirm shutdown
	system("pause"); //Wait for key press before exiting
	return 0;
}