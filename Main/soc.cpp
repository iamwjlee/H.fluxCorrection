#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <conio.h>
#include <windows.h>
#include <pthread.h>
#include <queue>
#include <vector>
#include <iostream>

#include <unistd.h>
#include <fcntl.h>

using namespace std;
#define MAX_DATASIZE 512

static pthread_t socTid;
static SOCKET sc; //server socket
static SOCKET cs; //client socket
static struct sockaddr_in serverAddr;
static struct sockaddr_in clientAddr;


static void *socThread(void * arg) {
    int *running=(int *)arg;
    printf("soc thread starting\n");
    WSADATA wsa;

    if(WSAStartup(MAKEWORD(2,2), &wsa) !=0 ) {
        printf("WSAStartup failed with error=%d\n", WSAGetLastError());
    }
    if((sc = socket(AF_INET , SOCK_STREAM , 0 )) < 0)   
    {
        printf("socket fail\n");
        closesocket(sc);
        return 0;
    }
    int val=1;
    if (setsockopt(sc, SOL_SOCKET, SO_REUSEADDR, (char *) &val, sizeof(val)) < 0) {
            printf("setsockopt\n");
            closesocket(sc);
            return 0;
    }


    memset((char*)&serverAddr,0, sizeof(serverAddr));
    serverAddr.sin_family       = AF_INET;
    serverAddr.sin_port         = htons(8764);
    serverAddr.sin_addr.s_addr  = INADDR_ANY;

    if(bind(sc, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == -1) {
		printf("--->bind fail\n");
		return 0;
	
	}
    listen(sc, 16);
    printf("Waiting for incoming connections...\n");
    unsigned char buf[MAX_DATASIZE];
    int len=0;
    while(*running) {     //serverAddrThread
        int c = sizeof(struct sockaddr_in);
        cs = accept(sc , (struct sockaddr *)&clientAddr, &c); //여기가 블럭이네!!!! 왜냐 하면 클라가 보내고 연결을 끊는다
        if(cs==INVALID_SOCKET) {
            printf("--->accept failed!  WSAGetLastError:%d" , WSAGetLastError());
            //break;
            continue;
        }
        char *client_ip=inet_ntoa(clientAddr.sin_addr);  
        int client_port=ntohs(clientAddr.sin_port);

        printf("server: Connection accepted ip[%s] port=%d\n", client_ip,client_port);

        //message receive/send thread if needed
        while(*running) {
            
            len=recv(cs,(char *)buf,MAX_DATASIZE,0);  // blocking funtion
            if(len>0) {
                printf("received=%d [%s] client_port=%d\n",len,buf,client_port);
                memset(buf,0,MAX_DATASIZE);
                //send message to client
                const char *msg="Hello";
                if(send(cs,msg,strlen(msg),0)==-1) {
                    printf("send error: %d\n",WSAGetLastError());
                    // break;
                }
            }
            else {
                memset(buf,0,MAX_DATASIZE);                
                break;
            }
            //  

            // Sleep(10);
        }
    }


    printf("soc thread exiting\n");
    void *ret;
    pthread_join(socTid, &ret);
    printf("soc thread exiting\n");

    return 0;


}
int socRun=0;

int soc_test() {
    socRun=1;
    pthread_create(&socTid,NULL,socThread,(void*)&socRun);

    printf("soc test\n");
    return 0;
}

int soc_test_stop() {
    // void *ret;
    socRun=0;
    // pthread_join(socTid, &ret);
    return 0;

}