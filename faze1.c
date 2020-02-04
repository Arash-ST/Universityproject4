#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include "cJSON.c"
#define MAX 80
#define PORT 12345
#define SA struct sockaddr
int maxsize=5000;
char MyToken[100];
// Function designed for chat between client and server.

void SendRequest(char MyRequest[MAX],char *MyResponcetype,char *MyResponcecontent)
{
    int SocketCreated;
    SocketCreated=CreatSocket();
    send(SocketCreated,MyRequest,maxsize,0);
    memset(MyRequest, 0, sizeof(MyRequest));
    recv(SocketCreated, MyRequest, maxsize, 0);
    cJSON* daryaft = cJSON_Parse(MyRequest);
    cJSON* MyTypeobject = cJSON_GetObjectItemCaseSensitive(daryaft,"type");
    cJSON* MyContentobject = cJSON_GetObjectItemCaseSensitive(daryaft,"content");

    if(cJSON_IsString(MyContentobject))
    {
        strcpy(MyResponcecontent,MyContentobject->valuestring);
        strcpy(MyResponcetype,MyTypeobject->valuestring);

    }
    else if(cJSON_IsArray(MyContentobject))
    {
        //printf("((%s))",MyContentobject.string);
        cJSON* m = NULL;
        cJSON_ArrayForEach(m, MyContentobject)
        {
            char* MyResponceSender=(char *)malloc(maxsize*sizeof(char));
            char* MyResponceContent2=(char *)malloc(maxsize*sizeof(char));
            if(cJSON_IsString(m))
            {
                strcpy(MyResponceContent2,m->valuestring);
                printf("-%s-\n",MyResponceContent2);
            }
            else
            {
                printf("---");
                cJSON* MySenderobject = cJSON_GetObjectItem(m,"sender");
                cJSON* MyContent2object = cJSON_GetObjectItem(m,"content");
                strcpy(MyResponceSender,MySenderobject->valuestring);
                strcpy(MyResponceContent2,MyContent2object->valuestring);
                printf("%s:'%s'\n",MyResponceSender,MyResponceContent2);

            }
        }
    }
}
int ChannelMembers()
{
    system("cls || clear");
    char MyRequest[maxsize];
    char *MyResponcetype=(char *)malloc(maxsize*sizeof(char));
    char *MyResponcecontent=(char *)malloc(maxsize*sizeof(char));
    memset(MyRequest, 0, sizeof(MyRequest));
    MyRequest[0]='c';
    strcat(MyRequest,"hannel members ");
    strcat(MyRequest,MyToken);
    strcat(MyRequest,"\n");
    printf("ChannelMembers:\n");
    SendRequest(MyRequest,MyResponcetype,MyResponcecontent);
    return 1;

}
int Refresh()
{
    system("cls || clear");
    char MyRequest[maxsize];
    char *MyResponcetype=(char *)malloc(maxsize*sizeof(char));
    char *MyResponcecontent=(char *)malloc(maxsize*sizeof(char));
    memset(MyRequest, 0, sizeof(MyRequest));
    MyRequest[0]='r';
    strcat(MyRequest,"efresh ");
    strcat(MyRequest,MyToken);
    strcat(MyRequest,"\n");
    printf("ChannelMassages:\n");
    SendRequest(MyRequest,MyResponcetype,MyResponcecontent);
    return 1;

}
int Login()
{
    char Username[maxsize];
    char Password[maxsize];
    char MyRequest[maxsize];
    char *MyResponcetype=(char *)malloc(maxsize*sizeof(char));
    char *MyResponcecontent=(char *)malloc(maxsize*sizeof(char));
    memset(MyRequest, 0, sizeof(MyRequest));
    printf("Enter Username:\n");
    fflush(stdin);
    gets(Username);
    printf("Enter Password:\n");
    gets(Password);
    MyRequest[0]='l';
    strcat(MyRequest,"ogin ");
    strcat(MyRequest,Username);
    strcat(MyRequest,", ");
    strcat(MyRequest,Password);
    strcat(MyRequest,"\n");
    SendRequest(MyRequest,MyResponcetype,MyResponcecontent);

    if(*MyResponcetype=='A')
    {
        printf("Logined successfuly\n");
        strcpy(MyToken,MyResponcecontent);
        return 1;

    }

    else
        printf("Can't be loged in\n");
        printf("Press any key to continue:");
        getch();
    return 0;
}
int Logout()
{
    char Username[100];
    char Password[100];
    char MyRequest[maxsize];
    char *MyResponcetype=(char *)malloc(maxsize*sizeof(char));
    char *MyResponcecontent=(char *)malloc(maxsize*sizeof(char));
    memset(MyRequest, 0, sizeof(MyRequest));
    MyRequest[0]='l';
    strcat(MyRequest,"ogout ");
    strcat(MyRequest,MyToken);
    strcat(MyRequest,"\n");
    SendRequest(MyRequest,MyResponcetype,MyResponcecontent);

    if(*MyResponcetype=='S')
    {
        printf("Logouted successfuly\n");
        strcpy(MyToken,MyResponcecontent);
        return 1;
    }
}
int JoinChannel()
{
    char MyRequest[maxsize];
    char Channelname[100];
    char *MyResponcetype=(char *)malloc(maxsize*sizeof(char));
    char *MyResponcecontent=(char *)malloc(maxsize*sizeof(char));
    memset(MyRequest, 0, sizeof(MyRequest));
    printf("Enter you channel name :\n");
    fflush(stdin);
    gets(Channelname);
    MyRequest[0]='j';
    strcat(MyRequest,"oin channel ");
    strcat(MyRequest,Channelname);
    strcat(MyRequest,", ");
    strcat(MyRequest,MyToken);
    strcat(MyRequest,"\n");
    SendRequest(MyRequest,MyResponcetype,MyResponcecontent);
    if(*MyResponcetype=='S')
    {
        printf("Channel joined successfuly\n");
        return 1;
    }
    else
        printf("This channel is not exist\n");
        printf("Press any key to continue:");
        getch();
    return 0;

}
int CreateChannel()
{
    char MyRequest[maxsize];
    char Channelname[100];
    char *MyResponcetype=(char *)malloc(maxsize*sizeof(char));
    char *MyResponcecontent=(char *)malloc(maxsize*sizeof(char));
    memset(MyRequest, 0, sizeof(MyRequest));
    printf("Enter you channel name :\n");
    fflush(stdin);
    gets(Channelname);
    MyRequest[0]='c';
    strcat(MyRequest,"reate channel ");
    strcat(MyRequest,Channelname);
    strcat(MyRequest,", ");
    strcat(MyRequest,MyToken);
    strcat(MyRequest,"\n");
    SendRequest(MyRequest,MyResponcetype,MyResponcecontent);
    if(*MyResponcetype=='S')
    {
        printf("Channel created successfuly\n");
        return 1;
    }
    else
        printf("This channel is already exist\n");
        printf("Press any key to continue:");
        getch();
    return 0;

}
int Register()
{
    char Username[100];
    char Password[100];
    char MyRequest[maxsize];
    char *MyResponcetype=(char *)malloc(maxsize*sizeof(char));
    char *MyResponcecontent=(char *)malloc(maxsize*sizeof(char));
    printf("Enter Username:\n");
    memset(MyRequest, 0, sizeof(MyRequest));
    fflush(stdin);
    gets(Username);
    printf("Enter Password:\n");
    gets(Password);
    MyRequest[0]='r';
    strcat(MyRequest,"egister ");
    strcat(MyRequest,Username);
    strcat(MyRequest,", ");
    strcat(MyRequest,Password);
    strcat(MyRequest,"\n");
    SendRequest(MyRequest,MyResponcetype,MyResponcecontent);
    //printf("\n----------------------%p-----------------------\n",MyResponcetype);

    if(*MyResponcetype=='S')
    {
        printf("Registration successfuly\n");
        return 1;
    }
    else
        printf("This user is already exist\n");
        printf("Press any key to continue:");
        getch();
    return 0;

}
int LeaveChannel()
{
    char MyRequest[maxsize];
    char Channelname[100];
    char *MyResponcetype=(char *)malloc(maxsize*sizeof(char));
    char *MyResponcecontent=(char *)malloc(maxsize*sizeof(char));
    memset(MyRequest, 0, sizeof(MyRequest));
    MyRequest[0]='l';
    strcat(MyRequest,"eave ");
    strcat(MyRequest,MyToken);
    strcat(MyRequest,"\n");
    SendRequest(MyRequest,MyResponcetype,MyResponcecontent);
    if(*MyResponcetype=='S')
    {
        printf("Channel leaved successfuly\n");
        return 1;
    }
    //else printf("This channel is not exist\n");
    return 0;

}
int Message()
{
    system("cls || clear");
    char MyRequest[maxsize];
    char MyMessage[100];
    char *MyResponcetype=(char *)malloc(maxsize*sizeof(char));
    char *MyResponcecontent=(char *)malloc(maxsize*sizeof(char));
    printf("Write your message Message:\n");
    memset(MyRequest, 0, sizeof(MyRequest));
    fflush(stdin);
    gets(MyMessage);
    MyRequest[0]='s';
    strcat(MyRequest,"end ");
    strcat(MyRequest,MyMessage);
    strcat(MyRequest,", ");
    strcat(MyRequest,MyToken);
    strcat(MyRequest,"\n");
    SendRequest(MyRequest,MyResponcetype,MyResponcecontent);
    //printf("\n----------------------%p-----------------------\n",MyResponcetype);

    printf("Message successfuly sent\n");
    return 1;
    //else printf("This user is already exist\n");

}
void chat(int server_socket)
{
    char buffer[MAX];
    int n;
    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        printf("Enter your message: ");
        n = 0;

        // Copy client message to the buffer
        while ((buffer[n++] = getchar()) != '\n')
            ;

        // Send the buffer to server
        send(server_socket, buffer, sizeof(buffer), 0);
        memset(buffer, 0, sizeof(buffer));

        // Read the message from server and copy it to buffer
        recv(server_socket, buffer, sizeof(buffer), 0);

        // Print buffer which contains the server message
        printf("From server: %s", buffer);

        // If the message starts with "exit" then client exits and chat ends
        if ((strncmp(buffer, "exit", 4)) == 0)
        {
            printf("Client stopping...\n");
            return;
        }
    }
}
int CreatSocket()
{
    int client_socket, server_socket;
    struct sockaddr_in servaddr, cli;

    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    // Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h
    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0)
    {
        // Tell the user that we could not find a usable Winsock DLL.
        printf("WSAStartup failed with error: %d\n", err);
        return 1;
    }

    // Create and verify socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        printf("Socket creation failed...\n");
        exit(0);
    }
    else
        // Assign IP and port
        memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // Connect the client socket to server socket
    if (connect(client_socket, (SA*)&servaddr, sizeof(servaddr)) != 0)
    {
        exit(0);
    }
    else
    {
        return client_socket;
    }
    return 0;

}

    void help()
{
     printf("\n\n    0 = Black       8 = Gray\n\
    1 = Blue        9 = Light Blue\n\
    2 = Green       A = Light Green\n\
    3 = Aqua        B = Light Aqua\n\
    4 = Red         C = Light Red\n\
    5 = Purple      D = Light Purple\n\
    6 = Yellow      E = Light Yellow\n\
    7 = White       F = Bright White\n(write your number) ");
}


/*0 = Black       8 = Gray
    1 = Blue        9 = Light Blue
    2 = Green       A = Light Green
    3 = Aqua        B = Light Aqua
    4 = Red         C = Light Red
    5 = Purple      D = Light Purple
    6 = Yellow      E = Light Yellow
    7 = White       F = Bright White
*/

void fChangeColor(int n){
    int user = '\0';
     char x[15] = "color ";
    x[6] = '\0';
    x[7] = '\0';

    while(1)
        {
    printf("\nChange Color \n1.Background\n2.Text\n3.Both\nenter any\
other number to exit.......... (write your number)\n");
    fflush(stdin);
    scanf("%d",&user);
    switch(user)
    {
    case 1:
                help();
                printf("\n\n choose any color code for background ....");
                fflush(stdin);
          scanf("%c",&x[6]);
          while(x[7] == '\0'){x[7] = '0';}
          system(x);
          break;

     case 2:
        help();
           printf(" \n\n choose any color code for text ...  ");
       while(x[6] == '\0'){ x[6] = '0';}
    fflush(stdin);
    scanf("%c",&x[7]);
    system(x);
    break;
     case 3:
        help();
        printf(" \n\n color code for background...  =  ");
    fflush(stdin);
    scanf("%c",&x[6]);
    printf(" \n\n color code for text...  =  ");
    fflush(stdin);
    scanf("%c",&x[7]);
    system(x);
        break;
default :
        printf("\n\n do you want to exit.......?(y/n)   ");
        char ch;
        fflush(stdin);
        scanf("%c",&ch);
        if(ch == 'y' || ch == 'Y')
        {
            fflush(stdin);
            if(n==1) FirstPage();
            else if(n==2) SecondPage();
            else if(n==3) ThirdPage();
            return;
        }
        break;
    }

    system("cls");
        }

}
void FirstPage()
{

    char option[100];
    while(true)
    {
        system("cls || clear");
        printf("Options:\n");
        printf("1-Register:\n");
        printf("2-Login:\n");
        printf("3-ChangeColor:\n");
        HANDLE handeler = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD position = {0,3};
        int y=3;
        int x=0;
        SetConsoleCursorPosition(handeler,position);
        int c = getch();
        if(c!=49 && c!=50 && c!=51)
        while(true){
            int c2 = getch();
            if(c2==80){
                if(y==3)
                {
                    y=1;
                    COORD position = {0,1};
                    SetConsoleCursorPosition(handeler,position);
                }else{
                COORD position = {0,++y};
                SetConsoleCursorPosition(handeler,position);
                }
            }
            else if(c2==72){
                if(y==1)
                {
                    y=3;
                    COORD position = {0,3};
                    SetConsoleCursorPosition(handeler,position);
                }
                else{
                COORD position = {0,--y};
                SetConsoleCursorPosition(handeler,position);
                }
            }
            else if(c2==13 || c2==49 || c2==50 || c2==51){
                c=c2;
                break;
            }
        }
        if(c==13 && y==3) c='3';
        if(c==13 && y==2) c='2';
        if(c==13 && y==1) c='1';
        option[0]=c;
        COORD position2 = {0, 0};
        SetConsoleCursorPosition(handeler,position2);
        if (option[0]=='1')
        {
            system("cls");
            Register();
        }
        else if (option[0]=='2')
        {
            system("cls");
            if(Login())
            {
                SecondPage();
                break;
            }
        }
        else if (option[0]=='3')
        {
            fChangeColor(1);
        }
    }
}
void SecondPage()
{
    char option[100];
    while(true)
    {
        system("cls || clear");
        printf("Options:\n");
        printf("1-Creatchannel:\n");
        printf("2-Joinchannel:\n");
        printf("3-Logout:\n");
        printf("4-ChangeColor:\n");

        //printf("4-Openchannel:\n");
        HANDLE handeler = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD position = {0,1};
        int y=1;
        int x=0;
        SetConsoleCursorPosition(handeler,position);
        int c = getch();
        if(c!=49 && c!=50 && c!=51 && c!=52)
        while(true){
            int c2 = getch();
            if(c2==80){
                if(y==4)
                {
                    y=1;
                    COORD position = {0,1};
                    SetConsoleCursorPosition(handeler,position);
                }else{
                COORD position = {0,++y};
                SetConsoleCursorPosition(handeler,position);
                }
            }
            else if(c2==72){
                if(y==1)
                {
                    y=4;
                    COORD position = {0,4};
                    SetConsoleCursorPosition(handeler,position);
                }
                else{
                COORD position = {0,--y};
                SetConsoleCursorPosition(handeler,position);
                }
            }
            else if(c2==13 || c2==49 || c2==50 || c2==51 || c2==52){
                c=c2;
                break;
            }
        }
        if(c==13 && y==4) c='4';
        if(c==13 && y==3) c='3';
        if(c==13 && y==2) c='2';
        if(c==13 && y==1) c='1';
        option[0]=c;
        COORD position2 = {0, 0};
        SetConsoleCursorPosition(handeler,position2);
        if (option[0]=='1')
        {
            system("cls");
            if(CreateChannel())
            {
                ThirdPage();
                break;
            }
        }
        if (option[0]=='2')
        {
            system("cls");
            if(JoinChannel())
            {
                ThirdPage();
                break;
            }
        }
        if (option[0]=='3')
        {
            if(Logout())
            {
                FirstPage();
                break;
            }
        }
        if (option[0]=='4')
        {
            fChangeColor(2);
        }
    }

}
int Dontclear=0;
void ThirdPage()
{
    Dontclear=0;
    char option[100];
    while(true)
    {
        system("cls || clear");
        printf("Options:\n");
        printf("1-Send Message:\n");
        printf("2-Refresh:\n");
        printf("3-Channel Members:\n");
        printf("4-Leave Channel:\n");
        printf("5-Find Member:\n");
        printf("6-ChangeColor:\n");

        HANDLE handeler = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD position = {0,1};
        int y=1;
        int x=0;
        SetConsoleCursorPosition(handeler,position);
        int c = getch();
        if(c!=49 && c!=50 && c!=51 && c!=52 && c!=53 && c!=54)
        while(true){
            int c2 = getch();
            if(c2==80){
                if(y==6)
                {
                    y=1;
                    COORD position = {0,1};
                    SetConsoleCursorPosition(handeler,position);
                }else{
                COORD position = {0,++y};
                SetConsoleCursorPosition(handeler,position);
                }
            }
            else if(c2==72){
                if(y==1)
                {
                    y=6;
                    COORD position = {0,6};
                    SetConsoleCursorPosition(handeler,position);
                }
                else{
                COORD position = {0,--y};
                SetConsoleCursorPosition(handeler,position);
                }
            }
            else if(c2==13 || c2==49 || c2==50 || c2==51 || c2==52 || c2==53 || c2==54){
                c=c2;
                break;
            }
        }
        if(c==13 && y==6) c='6';
        if(c==13 && y==5) c='5';
        if(c==13 && y==4) c='4';
        if(c==13 && y==3) c='3';
        if(c==13 && y==2) c='2';
        if(c==13 && y==1) c='1';
        option[0]=c;
        COORD position2 = {0, 0};
        SetConsoleCursorPosition(handeler,position2);
        if (option[0]=='1')
        {
            if(Message())
                ThirdPage();
        }
        if (option[0]=='4')
        {
            if(LeaveChannel())
                SecondPage();
        }
        if (option[0]=='2')
        {
            if(Refresh())
            {

                Dontclear=1;
                printf("\nPress any key to continue:");
                getch();
                ThirdPage();

            }
        }
        if (option[0]=='3')
        {
            if(ChannelMembers())
            {
                Dontclear=1;
                printf("\nPress any key to continue:");
                getch();
                ThirdPage();
            }
        }
        if (option[0]=='5'){
            FindMember();
        }
        if (option[0]=='6')
        {
            fChangeColor(3);
        }
    }
}
void FindMember(){
    system("cls");
    char MyRequest[maxsize];
    char Name[100];
    printf("Enter Member Name:\n");
    fflush(stdin);
    scanf("%s",Name);
    sprintf(MyRequest,"find %s %s",Name,MyToken);
    int SocketCreated;
    SocketCreated=CreatSocket();
    send(SocketCreated,MyRequest,maxsize,0);
    memset(MyRequest, 0, sizeof(MyRequest));
    recv(SocketCreated, MyRequest, maxsize, 0);
    if(MyRequest[0]=='0') printf("Can't find member -%s-\n",Name);
    else printf("member -%s- founded\n",Name);
    printf("Press any key to continue\n");
    getch();
}
int main()
{
    FirstPage();



    // Function for chat
    //chat(client_socket);
    // Close the socket
    //closesocket(client_socket);
}
