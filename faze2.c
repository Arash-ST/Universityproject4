#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include "cJSON.c"
#include <dir.h>
#include <process.h>
#define MAX 80
#define PORT 12345
#define SA struct sockaddr
int UserId;
int UsersNumbers=0;
int maxsize=5000;
char MyToken[100];
char MyRequest[5000];
char MyName[1000];
char MyChannelName[1000];
int MsgNum=0;
int MsgReadNum=0;
int ChannelNum=0;
// Function designed for chat between client and server.
struct ChannelMembers
{
    int Size;
    char Name[1000];
    char Members[1000][100];
} MyChannels[1000];
struct SUser
{
    char username[100];
    char password[100];
    char token[100];
    int isloggined;
    char channel[100];
    int MsgReadNum;
} MyUsers[1000];
int UserNum=0;



void CreatToken()
{
    srand(time(NULL));
    for (int i=0; i<32; i++)
    {
        MyToken[i]=65+(rand()%26);
    }
}
int FindClientSocket(int server_socket)
{
    struct sockaddr_in client;
    int client_socket;
    if ((listen(server_socket, 5)) != 0)
    {
        exit(0);
    }

    // Accept the data packet from client and verify
    int len = sizeof(client);
    client_socket = accept(server_socket, (SA *)&client, &len);
    if (client_socket < 0)
    {
        exit(0);
    }
    else
    {
        return client_socket;
    }
}
int CreatServerSocket()
{
    int server_socket, client_socket;
    struct sockaddr_in server;

    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    // Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h
    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0)
    {
        // Tell the user that we could not find a usable Winsock DLL.
        return 1;
    }

    // Create and verify socket
    server_socket = socket(AF_INET, SOCK_STREAM, 6);
    if (server_socket == INVALID_SOCKET)

        // Assign IP and port
        memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);

    // Bind newly created socket to given IP and verify
    if ((bind(server_socket, (SA *)&server, sizeof(server))) != 0)
    {
        exit(0);
    }
    else
    {
        return server_socket;
    }

    // Now server is ready to listen and verify


    // Function for chatting between client and server
    //chat(client_socket);

    // Close the socket
    //closesocket(server_socket);

}

void fLogin()
{
    char Username[100],Pass[100];
    sscanf(MyRequest,"%*s%*c%s%s",Username,Pass);
    Username[strlen(Username)-1]=0;
    for(int i=0; i<UsersNumbers; i++)
    {
        if(strcmp(MyUsers[i].username,Username)==0)
        {
            printf("-%s : %s Can't log in-\n",Username,Pass);
            return;
        }

    }
    mkdir("Resources");
    mkdir("Resources/Users");
    mkdir("Resources/Channels");
    char Address[1000];
    sprintf(Address,"%s%s%s","Resources/Users/",Username,".json");
    FILE *MyFile;
    if ((MyFile = fopen(Address, "r"))==NULL)
    {
        if ((MyFile = fopen(Address, "w"))==NULL)
        {
            puts("Cant open the file\n");
        }
        else
        {
            cJSON* Type=cJSON_CreateObject();
            cJSON_AddStringToObject(Type,"type","Error");
            cJSON_AddStringToObject(Type,"content","This Username is not exist");
            strcpy(MyRequest,cJSON_PrintUnformatted(Type));
            cJSON* User = cJSON_CreateObject();
            fclose(MyFile);
        }
    }
    else
    {
        fclose(MyFile);
        MyFile = fopen(Address, "r");
        char FileString[10000];
        fscanf(MyFile,"%[^\n]s",FileString);
        fclose(MyFile);
        cJSON* Daryaft=cJSON_Parse(FileString);
        cJSON* Password = cJSON_GetObjectItem(Daryaft,"password");
        if(strcmp(Password->valuestring,Pass)!=0)
        {
            cJSON* Type=cJSON_CreateObject();
            cJSON_AddStringToObject(Type,"type","Error");
            cJSON_AddStringToObject(Type,"content","Password is not correct");
            strcpy(MyRequest,cJSON_PrintUnformatted(Type));
            cJSON* User = cJSON_CreateObject();
            fclose(MyFile);
            return;
        }
        else
        {
            printf("-%s : %s Loged in-\n",Username,Pass);
            cJSON* Type=cJSON_CreateObject();
            CreatToken();
            cJSON_AddStringToObject(Type,"type","AuthToken");
            cJSON_AddStringToObject(Type,"content",MyToken);
            strcpy(MyRequest,cJSON_PrintUnformatted(Type));
            strcpy(MyUsers[UsersNumbers].username,Username);
            strcpy(MyUsers[UsersNumbers].token,MyToken);
            MyUsers[UsersNumbers].isloggined=1;
            UsersNumbers++;
            strcpy(MyName,Username);
            strcpy(MyUsers[UsersNumbers].token,MyToken);
            MyUsers[UsersNumbers].MsgReadNum=0;
        }
    }
}
void fChannelMembers()
{
    FindUsernamebytoken();
    char Members[1000][1000];
    int Num;
    for(int i=0; i<ChannelNum; i++)
    {
        if(strcmp(MyChannelName,MyChannels[i].Name)==0)
        {
            Num=i;
            for(int j=0; j<MyChannels[i].Size; j++)
            {
                strcpy(Members[j],MyChannels[i].Members[j]);
            }
            break;
        }
    }
    cJSON* Type=cJSON_CreateObject();
    cJSON* WholeContent=cJSON_CreateArray();
    cJSON_AddStringToObject(Type,"type","List");

    int Size=MyChannels[Num].Size;
    for(int i=0; i<Size; i++)
    {
        cJSON_AddItemToArray(WholeContent,cJSON_CreateString(Members[i]));
    }

    cJSON_AddItemToObject(Type,"content",WholeContent);
    //printf("\n\n\n%s\n\n\n",cJSON_PrintUnformatted());
    strcpy(MyRequest,cJSON_PrintUnformatted(Type));
    printf("-Members list sent-\n");
}
void fRefresh()
{
    char Message[100];
    sscanf(MyRequest,"%*s%*c%s",MyToken);
    FindUsernamebytoken();
    Message[strlen(Message)-1]=0;
    char Address[1000];
    sprintf(Address,"%s%s%s","Resources/Channels/",MyChannelName,".json");
    FILE *MyFile;
    MyFile = fopen(Address, "r");
    char FileString[10000];
    fscanf(MyFile,"%[^\n]s",FileString);
    fclose(MyFile);
    cJSON* Ch = cJSON_Parse(FileString);
    cJSON* Msg = cJSON_GetObjectItemCaseSensitive(Ch,"message");
    cJSON* AMsg;
    printf("-Messages sent-\n");
    strcpy(MyRequest,"");
    cJSON* Type=cJSON_CreateObject();
    cJSON* WholeContent=cJSON_CreateArray();
    cJSON_AddStringToObject(Type,"type","List");
    int Size=cJSON_GetArraySize(Msg);
    FindUsernamebytoken();
    for(int i=0; i<MyUsers[UserId].MsgReadNum; i++)
    {
        cJSON_DeleteItemFromArray(Msg,0);
    }
    cJSON_AddItemToObject(Type,"content",Msg);
    //printf("\n\n\n%s\n\n\n",cJSON_PrintUnformatted());
    strcpy(MyRequest,cJSON_PrintUnformatted(Type));

    fclose(MyFile);
    MyUsers[UserId].MsgReadNum=Size;
}

void fLogout()
{
    sscanf(MyRequest,"%*s%*c%s",MyToken);
    FindUsernamebytoken();
    printf("-%s : **** Loged out-\n",MyName);
    for(int i=0; i<UserNum; i++)
    {
        if(strcmp(MyUsers[i].token,MyToken)==0)
        {
            if(MyUsers[i].isloggined==1)
            {
                MyUsers[i].isloggined=0;
                cJSON* Type=cJSON_CreateObject();
                cJSON_AddStringToObject(Type,"type","Successful");
                cJSON_AddStringToObject(Type,"content","");
                strcpy(MyRequest,cJSON_PrintUnformatted(Type));
            }
            return;
        }
    }
    for(int i=UserId; i<UsersNumbers; i++)
    {
        MyUsers[i]=MyUsers[i+1];
    }
    UsersNumbers--;
}
void fRegister()
{
    char Username[100],Pass[100];
    sscanf(MyRequest,"%*s%*c%s%s",Username,Pass);
    Username[strlen(Username)-1]=0;
    mkdir("Resources");
    mkdir("Resources/Users");
    mkdir("Resources/Channels");
    char Address[1000];
    sprintf(Address,"%s%s%s","Resources/Users/",Username,".json");
    FILE *MyFile;
    if ((MyFile = fopen(Address, "r"))==NULL)
    {
        if ((MyFile = fopen(Address, "w"))==NULL)
        {
            puts("Cant open the file\n");
        }
        else
        {
            printf("-%s : %s Registered-\n",Username,Pass);

            cJSON* User = cJSON_CreateObject();
            cJSON_AddStringToObject(User,"username",Username);
            cJSON_AddStringToObject(User,"password",Pass);
            char* UserUnformatted = cJSON_PrintUnformatted(User);
            fprintf(MyFile,"%s",UserUnformatted);
            cJSON* Type=cJSON_CreateObject();
            cJSON_AddStringToObject(Type,"type","Successful");
            cJSON_AddStringToObject(Type,"content","");
            strcpy(MyRequest,cJSON_PrintUnformatted(Type));
            fclose(MyFile);
            CreatToken();
        }


    }
    else
    {
        printf("-%s : %s Can't Register-\n",Username,Pass);
        cJSON* Type=cJSON_CreateObject();
        cJSON_AddStringToObject(Type,"type","Error");
        cJSON_AddStringToObject(Type,"content","This Username has been taken");
        strcpy(MyRequest,cJSON_PrintUnformatted(Type));
    }

}
void FindUsernamebytoken()
{
    for(int i=0; i<UsersNumbers; i++)
    {
        if(strcmp(MyUsers[i].token,MyToken)==0)
        {
            strcpy(MyName,MyUsers[i].username);
            UserId=i;
            strcpy(MyChannelName,MyUsers[i].channel);
        }

    }
}
void fCreatChannel()
{

    char ChannelName[100];
    sscanf(MyRequest,"%*s%*s%*c%s%*c%s",ChannelName,MyToken);
    ChannelName[strlen(ChannelName)-1]=0;
    mkdir("Resources");
    mkdir("Resources/Users");
    mkdir("Resources/Channels");
    char Address[1000];
    sprintf(Address,"%s%s%s","Resources/Channels/",ChannelName,".json");
    FILE *MyFile;
    if ((MyFile = fopen(Address, "r"))==NULL)
    {
        if ((MyFile = fopen(Address, "w"))==NULL)
        {
            puts("Cant open the file\n");
        }
        else
        {
            printf("-%s : %s Channel created-\n",ChannelName,MyToken);
            cJSON* Ch = cJSON_CreateObject();
            cJSON* Msg = cJSON_CreateArray();
            cJSON* AMsg = cJSON_CreateObject();
            cJSON_AddStringToObject(Ch,"name",ChannelName);
            cJSON_AddStringToObject(AMsg,"sender","server");
            char Content[1000];
            sprintf(Content,"%s Created -%s-",MyName,ChannelName);
            cJSON_AddStringToObject(AMsg,"content",Content);
            cJSON_AddItemToArray(Msg,AMsg);
            cJSON_AddItemToObject(Ch,"message",Msg);
            char* ChUnformatted = cJSON_PrintUnformatted(Ch);
            fprintf(MyFile,"%s",ChUnformatted);
            cJSON* Type=cJSON_CreateObject();
            cJSON_AddStringToObject(Type,"type","Successful");
            cJSON_AddStringToObject(Type,"content","");
            strcpy(MyRequest,cJSON_PrintUnformatted(Type));
            fclose(MyFile);
            strcpy(MyChannels[ChannelNum].Name,ChannelName);
            MyChannels[ChannelNum].Size=1;
            strcpy(MyChannels[ChannelNum].Members[0],MyName);
            ChannelNum++;
            strcpy(MyChannelName,ChannelName);
        }
        FindUsernamebytoken();
        strcpy(MyUsers[UserId].channel,ChannelName);

    }
    else
    {
        printf("-%s : %s Channel can't be created-\n",ChannelName,MyToken);
        cJSON* Type=cJSON_CreateObject();
        cJSON_AddStringToObject(Type,"type","Error");
        cJSON_AddStringToObject(Type,"content","This Username has been taken");
        strcpy(MyRequest,cJSON_PrintUnformatted(Type));
    }

}
void fLeaveChannel()
{
    char ChannelName[1000];
    strcpy(ChannelName,MyChannelName);
    sscanf(MyRequest,"%*s%*c%s",MyToken);
    FindUsernamebytoken();
    mkdir("Resources");
    mkdir("Resources/Users");
    mkdir("Resources/Channels");
    char Address[1000];
    sprintf(Address,"%s%s%s","Resources/Channels/",ChannelName,".json");
    FILE *MyFile;
    if ((MyFile = fopen(Address, "r"))==NULL)
    {
        if ((MyFile = fopen(Address, "w"))==NULL)
        {
            puts("Cant open the file\n");
        }
        else
        {
            printf("-%s : %s Can't leave the channel-\n",ChannelName,MyToken);
            cJSON* Type=cJSON_CreateObject();
            cJSON_AddStringToObject(Type,"type","Error");
            cJSON_AddStringToObject(Type,"content","This Channel is not exist");
            strcpy(MyRequest,cJSON_PrintUnformatted(Type));
            fclose(MyFile);
        }


    }
    else
    {
        printf("-%s : %s Leaved the channel-\n",ChannelName,MyToken);
        MsgReadNum=0;
        char FileString[10000];
        fscanf(MyFile,"%[^\n]s",FileString);
        fclose(MyFile);
        MyFile = fopen(Address, "w");
        cJSON* Ch = cJSON_Parse(FileString);
        cJSON* Msg = cJSON_GetObjectItem(Ch,"message");
        cJSON* AMsg;
        cJSON_AddItemToArray(Msg, AMsg = cJSON_CreateObject());
        cJSON_AddStringToObject(AMsg,"sender","server");
        char Content[1000];
        sprintf(Content,"%s Leaved -%s-",MyName,MyChannelName);
        cJSON_AddStringToObject(AMsg,"content",Content);
        cJSON* Ch2 = cJSON_CreateObject();

        cJSON_AddItemToObject(Ch2,"message",Msg);
        char* ChUnformatted = cJSON_PrintUnformatted(Ch2);
        fprintf(MyFile,"%s",ChUnformatted);
        cJSON* Type=cJSON_CreateObject();
        cJSON_AddStringToObject(Type,"type","Successful");
        cJSON_AddStringToObject(Type,"content","");
        strcpy(MyRequest,cJSON_PrintUnformatted(Type));
        fclose(MyFile);
        for(int i=0; i<ChannelNum; i++)
        {
            if(strcmp(MyChannelName,MyChannels[i].Name)==0)
            {
                for(int j=0; j<MyChannels[i].Size-1; j++)
                    strcpy(MyChannels[i].Members[j],MyChannels[i].Members[j+1]);
                MyChannels[i].Size--;
            }
        }
        FindUsernamebytoken();
        MyUsers[UserId].MsgReadNum=0;
    }
}
void fJoinChannel()
{
    //printf("mammm[%s]mmamm",MyName);
    char ChannelName[100];
    sscanf(MyRequest,"%*s%*s%*c%s%*c%s",ChannelName,MyToken);
    FindUsernamebytoken();
    ChannelName[strlen(ChannelName)-1]=0;
    mkdir("Resources");
    mkdir("Resources/Users");
    mkdir("Resources/Channels");
    char Address[1000];
    sprintf(Address,"%s%s%s","Resources/Channels/",ChannelName,".json");
    FILE *MyFile;
    if ((MyFile = fopen(Address, "r"))==NULL)
    {

        printf("-%s : %s Can't join the channel-\n",ChannelName,MyToken);

        cJSON* Type=cJSON_CreateObject();
        cJSON_AddStringToObject(Type,"type","Error");
        cJSON_AddStringToObject(Type,"content","This Channel is not exist");
        strcpy(MyRequest,cJSON_PrintUnformatted(Type));
        fclose(MyFile);
    }


    else
    {
        printf("-%s : %s Joined the channel-\n",ChannelName,MyToken);
        strcpy(MyChannelName,ChannelName);
        char FileString[10000];
        fscanf(MyFile,"%[^\n]s",FileString);
        fclose(MyFile);
        MyFile = fopen(Address, "w");
        cJSON* Ch = cJSON_Parse(FileString);
        cJSON* Msg = cJSON_GetObjectItem(Ch,"message");
        cJSON* AMsg;
        cJSON_AddItemToArray(Msg, AMsg = cJSON_CreateObject());
        cJSON_AddStringToObject(AMsg,"sender","server");
        char Content[1000];
        sprintf(Content,"%s Joined -%s-",MyName,ChannelName);
        cJSON_AddStringToObject(AMsg,"content",Content);
        cJSON* Ch2 = cJSON_CreateObject();
        cJSON_AddItemToObject(Ch2,"message",Msg);
        char* ChUnformatted = cJSON_PrintUnformatted(Ch2);
        fprintf(MyFile,"%s",ChUnformatted);
        cJSON* Type=cJSON_CreateObject();
        cJSON_AddStringToObject(Type,"type","Successful");
        cJSON_AddStringToObject(Type,"content","");
        strcpy(MyRequest,cJSON_PrintUnformatted(Type));
        fclose(MyFile);
        int Flag=0;
        for(int i=0; i<ChannelNum; i++)
        {
            if(strcmp(ChannelName,MyChannels[i].Name)==0)
            {
                strcpy(MyChannels[i].Members[MyChannels[i].Size],MyName);
                MyChannels[i].Size++;
                Flag=1;
            }
        }
        if(Flag==0)
        {

            strcpy(MyChannels[ChannelNum].Members[0],MyName);
            strcpy(MyChannels[ChannelNum].Name,MyChannelName);
            MyChannels[ChannelNum].Size=1;
            ChannelNum++;
        }
        FindUsernamebytoken();
        strcpy(MyUsers[UserId].channel,ChannelName);

    }
}

void fSendMsg()
{
    char Message[1000];
    sscanf(MyRequest,"%*s%*c%s%*c%s",Message,MyToken);
    for(int i=0; i<strlen(Message); i++)
        if(Message[i]=='$')
            Message[i]=' ';
    FindUsernamebytoken();
    Message[strlen(Message)-1]=0;
    char Address[1000];
    sprintf(Address,"%s%s%s","Resources/Channels/",MyChannelName,".json");
    FILE *MyFile;
    MyFile = fopen(Address, "r");
    char FileString[10000];
    fscanf(MyFile,"%[^\n]s",FileString);
    fclose(MyFile);
    MsgNum++;
    MyFile=fopen(Address, "w");
    cJSON* Ch = cJSON_Parse(FileString);
    cJSON* Msg = cJSON_GetObjectItemCaseSensitive(Ch,"message");
    cJSON* AMsg = cJSON_CreateObject();
    cJSON_AddStringToObject(AMsg,"sender",MyName);
    cJSON_AddStringToObject(AMsg,"content",Message);
    cJSON_AddItemToArray(Msg,AMsg);
    cJSON* Name;
    Name =cJSON_GetObjectItemCaseSensitive(Ch,"name");
    cJSON* Ch2 = cJSON_CreateObject();
    cJSON_AddStringToObject(Ch2,"name",Name);
    cJSON_AddItemToObject(Ch2,"message",Msg);
    char* ChUnformatted = cJSON_PrintUnformatted(Ch2);
    fprintf(MyFile,"%s",ChUnformatted);
    cJSON* Type=cJSON_CreateObject();
    cJSON_AddStringToObject(Type,"type","Successful");
    cJSON_AddStringToObject(Type,"content","");
    strcpy(MyRequest,cJSON_PrintUnformatted(Type));
    fclose(MyFile);
    printf("-Messages recieved-\n");
}
void fFindMember()
{
    char Mem[100];
    sscanf(MyRequest,"%*s%*c%s%*c%s",Mem,MyToken);
    printf("try to find member -%s-\n",Mem);
    FindUsernamebytoken();
    for(int i=0; i<UsersNumbers; i++)
    {
        if(strcmp(MyUsers[UserId].channel,MyUsers[i].channel)==0 && strcmp(Mem,MyUsers[i].username)==0)
        {
            strcpy(MyRequest,"1");
            return;
        }
    }
    strcpy(MyRequest,"0");

}
int main()
{

    int ServerSocket=CreatServerSocket();
    while(1)
    {
        int ClientSocket=FindClientSocket(ServerSocket);
        recv(ClientSocket, MyRequest, maxsize, 0);
        if(strncmp(MyRequest,"register ",9)==0)
            fRegister();
        if(strncmp(MyRequest,"login ",6)==0)
            fLogin();
        if(strncmp(MyRequest,"create channel ",15)==0)
            fCreatChannel();
        if(strncmp(MyRequest,"join channel ",13)==0)
            fJoinChannel();
        if(strncmp(MyRequest,"leave ",6)==0)
            fLeaveChannel();
        if(strncmp(MyRequest,"send ",5)==0)
        {
            for(int i=strlen(MyRequest)-36; i>=5; i--)
            {
                if(MyRequest[i]==' ')
                    MyRequest[i]='$';
            }
            fSendMsg();
        }

        if(strncmp(MyRequest,"refresh ",8)==0)
            fRefresh();
        if(strncmp(MyRequest,"channel members ",16)==0)
            fChannelMembers();
        if(strncmp(MyRequest,"logout ",7)==0)
            fLogout();
        if(strncmp(MyRequest,"find ",5)==0)
            fFindMember();
        send(ClientSocket,MyRequest,strlen(MyRequest),0);
        memset(MyRequest, 0, sizeof(MyRequest));
    }

}
