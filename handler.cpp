//This file contains the function that is run in the threads that handle each connection

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

//Connection handler and command interpreter
int handlerthread()
{
	//Check if this thread should be running
	if (killserv == 1)
		ExitThread((DWORD)0);
	//Variables to keep track of session
	int mysocknum=socknum2;
	int mysessionnum=sessionnum;
	int tdata2=0;
	//Session initialization confirmation
	char tosend0[]={"Welcome to RobShell \n\r"};
	int len2=strlen(tosend0);
	send(mysocknum, (const char *)&tosend0, len2, 0);
	int givecmd = 0;
	//Tracking vars for invalid command handler
	int firstloop = 1;
	int sincerun = 0;
	//Command interpreter loop
	while(1)
	{
		char torecv[5000] = { NULL };
		//Check if command has already been received
		if (givecmd == 0)
		{
			char tosend[] = { "Please enter a command: " };
			len2 = strlen(tosend);
			send(mysocknum, (const char *)&tosend, len2, 0);
			givecmd = 1;
		}
		//Receive command
		int numchars = recv(mysocknum, (char *)&torecv, 4999, 0); //Receive command
		if (numchars == 0)
			break;
		torecv[numchars] = '\0';
		//Process commands and execute appropriate actions
		if(strcmp(torecv, "exit")==0)
			break;
		//Handler for showline command
		else if(strcmp(torecv, "showline")==0)
		{
			//Disabled command handling
			if(slinedisable==1)
			{
				char tosend[]={"Show Line Command is Disabled \n\r"};
				len2=strlen(tosend);
				send(mysocknum, (const char *)&tosend, len2, 0);
			}
			else
			{
				char tosend[]={"____________________ \n\r \n\r"};
				len2=strlen(tosend);
				send(mysocknum, (const char *)&tosend, len2, 0);
			}
			givecmd=0;
			sincerun = 0;
		}
		//Handler for usersay command
		else if(strcmp(torecv, "usersay")==0)
		{
			if(usersaydisable==0)
			{
				char tosend[]={"Please Input Something To Say: "};
				len2=strlen(tosend);
				send(mysocknum, (const char *)&tosend, len2, 0);
				char torecv2[500]={NULL};
				for(int z=0; z<2; z++)
					recv(mysocknum, (char *)&torecv2, 499, 0);
				torecv2[499]='\0';
				cout << "Usersay - Session " << mysessionnum << ": ";
				for(int x=0; x<strlen(torecv2); x++)
					cout << torecv2[x];
				cout << endl;
				char tosend2[]={"\n\r"};
				len2=strlen(tosend2);
				send(mysocknum, (const char *)&tosend2, len2, 0);
			}
			//Disabled command handling
			else
			{
				char tosend[]={"Usersay Command is Disabled \n\r"};
				len2=strlen(tosend);
				send(mysocknum, (const char *)&tosend, len2, 0);
			}
			givecmd=0;
			sincerun = 0;
		}
		//Handler for giveascii command
		else if(strcmp(torecv, "giveascii")==0)
		{
			if(gasciidisable==0)
				HANDLE threadhandle3 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)giveasciifunc, &tdata2, 0, NULL);
			else
			{
				char tosend[]={"Give Ascii Command is Disabled \n\r"};
				len2=strlen(tosend);
				send(mysocknum, (const char *)&tosend, len2, 0);
			}
			givecmd=0;
			sincerun = 0;
		}
		//Command help system
		else if(strcmp(torecv, "help")==0)
		{
			char tosend[]={"RobShell Help System \n\r \n\r Command List: \n\r exit \n\r showline \n\r usersay \n\r giveascii \n\r serverbeep \n\r clientbeep \n\r clear \n\r cls \n\r consolematrix \n\r help \n\r \n\r"};
			len2=strlen(tosend);
			send(mysocknum, (const char *)&tosend, len2, 0);
			char tosend2[]={"Note: No command takes command line arguments. \n\rAny command requiring input will ask when ready.  \n\r\n\rNote2: The 'clear' and 'cls' commands are the same,\n\rthey are simply both availible for convience.  \n\r\n\r"};
			len2=strlen(tosend2);
			send(mysocknum, (const char *)&tosend2, len2, 0);
			givecmd=0;
			sincerun = 0;
		}
		//Handler for serverbeep command
		else if(strcmp(torecv, "serverbeep")==0)
		{
			if(sbeepdisable==0)
				threadhandle2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)sbeep, &tdata2, 0, NULL);
			//Disabled command handling
			else
			{
				char tosend[]={"Server Beep Command is Disabled \n\r"};
				len2=strlen(tosend);
				send(mysocknum, (const char *)&tosend, len2, 0);
			}
			givecmd=0;
			sincerun = 0;
		}
		//Handler for clientbeep command
		else if(strcmp(torecv, "clientbeep")==0)
		{
			if(cbeepdisable==0)
			{
				char tosend[]={"\a"};
				len2=strlen(tosend);
				send(mysocknum, (const char *)&tosend, len2, 0);
			}
			//Disabled command handling
			else
			{
				char tosend[]={"Client Beep Command is Disabled \n\r"};
				len2=strlen(tosend);
				send(mysocknum, (const char *)&tosend, len2, 0);
			}
			givecmd=0;
			sincerun = 0;
		}
		//Handler for clear command
		else if(strcmp(torecv, "clear")==0)
		{
			if(clsdisable==0)
			{
				char tosend[]={"\f"};
				len2=strlen(tosend);
				send(mysocknum, (const char *)&tosend, len2, 0);
			}
			//Disabled command handling
			else
			{
				char tosend[]={"Clear Screen Command is Disabled \n\r"};
				len2=strlen(tosend);
				send(mysocknum, (const char *)&tosend, len2, 0);
			}
			givecmd=0;
			sincerun = 0;
		}
		//Handler for cls command
		else if(strcmp(torecv, "cls")==0)
		{
			if(clsdisable==0)
			{
				char tosend[]={"\f"};
				len2=strlen(tosend);
				send(mysocknum, (const char *)&tosend, len2, 0);
			}
			//Disabled command handling
			else
			{
				char tosend[]={"Clear Screen Command is Disabled \n\r"};
				len2=strlen(tosend);
				send(mysocknum, (const char *)&tosend, len2, 0);
			}
			givecmd=0;
			sincerun = 0;
		}
		//Handler for consolematrix command
		else if(strcmp(torecv, "consolematrix")==0)
		{
			if (matrixdisable == 0)
			{
				for(int m=0; m<500000; m++) //Matrix output loopj
				{
					int randnumx=rand()%6;
					if(randnumx==0)
					{
						char tosendx[]={"A3B4QKJ9334J3K487FHGHA2LJSDF8VNW8RUIO34NL43J23887HFSKJK3K4K5NKJGFDKDFIERN8473KJ8"};
						len2=strlen(tosendx);
						send(mysocknum, (const char *)&tosendx, len2, 0);
					}
					else if(randnumx==1)
					{
						char tosendx1[]={"K3DC4MYZKDM823K898376KJDSFHNE834J34IDF45H4J5HK435G7SDF98SJ2JKJ234NI24K34N38DS721"};
						len2=strlen(tosendx1);
						send(mysocknum, (const char *)&tosendx1, len2, 0);
					}
					else if(randnumx==2)
					{
						char tosendx2[]={"7X7FK3K123872L3J8SDJFKNLKWEJRIOLFKNOIWEURLWNERLKWJEFIUSDLKFNWEORHOIWEURLKNFDKLJS"};
						len2=strlen(tosendx2);
						send(mysocknum, (const char *)&tosendx2, len2, 0);
					}
					else if(randnumx==3)
					{
						char tosendx3[]={"64872657863258762837598JKSHDFKJHJ873987123HKJHSF87923847JSHDFJ748778293472828221"};
						len2=strlen(tosendx3);
						send(mysocknum, (const char *)&tosendx3, len2, 0);
					}
					else if(randnumx==4)
					{
						char tosendx4[]={"K34K2348UKLJFK3J4KL32G4JH238SSKKDFJK34JLK235H588S7F89SDFJ348735JK2L34JSDF834K2KH"};
						len2=strlen(tosendx4);
						send(mysocknum, (const char *)&tosendx4, len2, 0);
					}
					else if(randnumx==5)
					{
						char tosendx5[]={"XDF87DF8SDF8SK34K28J3KDSFHADJF823487324865LJHF8374HF8K3KD834DKJS8K24K38SKXKFJXHG"};
						len2=strlen(tosendx5);
						send(mysocknum, (const char *)&tosendx5, len2, 0);
					}
					else //Error handling
						cout << "Matrix Error, randnumx out of bounds" << endl;
				}
			}
			//Disabled command handling
			else
			{
				char tosend[]={"Matrix Command is Disabled \n\r"};
				len2=strlen(tosend);
				send(mysocknum, (const char *)&tosend, len2, 0);
			}
			givecmd=0;
			sincerun = 0;
		}
		//Invalid command handler
		else
		{
			if (sincerun > 1 && firstloop == 0)
			{
				char tosend[] = { "Invalid Command \n\r" };
				len2 = strlen(tosend);
				send(mysocknum, (const char *)&tosend, len2, 0);
				givecmd = 0;
				if (numchars != 2)
					sincerun = 0;
			}
		}
		//Update tracking variables for invalid command handler
		firstloop = 0;
		sincerun++;
	}
	//Connection cleanup and session termination
	closesocket(mysocknum);
	cout << "User Disconnected: Session " << mysessionnum << "	Socket Number: " << mysocknum << endl;
	ExitThread((DWORD)0); //End connection handler thread
	return 0;
}