//This file contains functions that are run asynchronously in thier own thread

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

//Function for giveascii command
void giveasciifunc()
{
	cout << "It's an ascii diamond" << endl << endl;
	cout << "     0     " << endl;
	cout << "    0 0    " << endl;
	cout << "   0   0   " << endl;
	cout << "    0 0    " << endl;
	cout << "     0     " << endl;
	ExitThread((DWORD)0);
}

//Server console input monitoring loop
void conscmd()
{
	while(1)
	{
		char command[50];
		fgets(command, 49, stdin);
		if(strcmp(command, "exit\n")==0)
		{
			killserv=1;
			cout << "Shutting Down Server " << endl;
			closesocket(socknum);
			WSACleanup();
			ExitThread((DWORD)0);
		}
		else
			cout << "Command Not Recognized" << endl;
	}
}

//Function for sbeep command
void sbeep()
{
	cout << "\a";
	ExitThread((DWORD)0);
}