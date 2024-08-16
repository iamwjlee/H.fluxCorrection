
/**
 ******************************************************************************
 * File Name          : soc.cpp
 * Description        : Andariel's Visage in D4S5
 ******************************************************************************
 *
 * Copyright (C) 2005 JANGWEE DATA SYSTEM
 *
 *
 *
 * History :
 *         2024-08-14 initiated by Andariel
 *         tcpip server with multiple client connections 
 *              client is connected to server always  
 * 
 *  
 ******************************************************************************
 */ 

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

#include "soc.h"

using namespace std;
#define MAX_DATASIZE 512
#define MAX_CLIENTS 5


static pthread_t socTid;
static pthread_t clients[MAX_CLIENTS];

static SOCKET sc; //server socket fd
static SOCKET cs; //client socket fd



clients_info_t csArray[MAX_CLIENTS];

static struct sockaddr_in serverAddr;
static struct sockaddr_in clientAddr;



static void *clientThread(void *index) {
    int idx=  *(int *)index;
    unsigned char buf[MAX_DATASIZE];
    int len=0;

    while(1) {
        len=recv(csArray[idx].cs,(char *)buf,MAX_DATASIZE,0);  // non-blocking funtion 
        if(len>0) {
            log_msg("idx=%d ip[%s] received=%d [%s]\n",idx,csArray[idx].name, len,buf);
            memset(buf,0,MAX_DATASIZE);
            //send message to client
            const char *msg[10]={"Hello0","Hello1","Hello2","Hello3","Hello4","Hello5","Hello6","Hello7","Hello8","Hello9"};
            if(send(csArray[idx].cs,msg[idx],strlen(msg[idx]),0)==-1) {
                printf("send error: %d\n",WSAGetLastError());
                // break;
            }
        }
        else if(len==0) { //여기 계속들어온다
        }
        else {
            /*
                클라이언트 프로그램이 종료시  바로 여기로 들어온다.
                    -> 신기방통이네
                랜선이 끊기면 이리로 오지 않는다 모른다
                    -> 서버는 그사실을 알수 없고, 클라는 전송후 응답이 없으면 리컨넥션 한다


            */
            printf("recv error[%s] idx=%d len=%d\n",csArray[idx].name,idx,len);
            memset(&csArray[idx],0,sizeof(clients_info_t));
            memset(buf,0,MAX_DATASIZE);                
            break;
        }
    }
    return 0;



}

static void *socThread(void * arg) {
    int *running=(int *)arg;
    #ifdef __moved__
    printf("soc test starting\n");
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
    #endif
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

        int marking=-1;
        for(int i=0;i<MAX_CLIENTS;i++) {
            if( !strcmp(client_ip,csArray[i].name) || csArray[i].cs==0) {
                csArray[i].cs=cs;
                strcpy(csArray[i].name,client_ip);
                marking=i;
                pthread_create(&clients[i],NULL,clientThread,(void*)&i);
                break;
            }
        }
        printf("server: Connection accepted ip[%s] port=%d\n", client_ip,client_port);
        /*
 
            클라최초 접속시
            클라재실행시 
            네트웍이 끊기고 컨넥션을 재시도 하기 전에 연결복구 되면 여기에 안들어 오고 정상 작동
            네트웍이 끊기고 컨넥션 재시도 하면 여기에 들어온다
            즉, 클라가 컨넥션을 하기만 하면 여기로 무조건 들어와 -> 중복아이피 방지 해야
             
        */
        for(int i=0;i<MAX_CLIENTS;i++) {
            printf("\tcsArray[%d].name[%s]%s\n",i,csArray[i].name,marking==i?"<<":"");

        }


        //message receive/send thread if needed
        while(0 && *running ) {
            
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

    //----------------------------------------------------------------
    printf("soc test starting\n");
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

    memset(csArray,0,sizeof(csArray));
    if(bind(sc, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == -1) {
		printf("--->bind fail\n");
		return 0;
	
	}
    listen(sc, 16);
    printf("Waiting for incoming connections...\n");
    //----------------------------------------------------------------

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

struct tm local_time;
void log_msg(const char *msg,...) {
    va_list arg;
    char buf[1024];
    char timeBuf[64];
    // pthread_mutex_lock(&mutx);

    time_t t=time(NULL); local_time= *localtime(&t);
    memset(buf, 0, 1024);
    memset(timeBuf, 0, sizeof(timeBuf));
    sprintf(timeBuf,"[%04d-%02d-%02d %02d:%02d:%02d] ",
         local_time.tm_year+1900, local_time.tm_mon+1, local_time.tm_mday,
         local_time.tm_hour, local_time.tm_min, local_time.tm_sec);

    strcat(buf,timeBuf);


    va_start(arg,msg);
    vsprintf(buf+strlen(buf) ,msg,arg);
    // vsprintf(buf,msg,arg);
    va_end(arg);
    printf(buf);  // standard ouput
    // pthread_mutex_unlock(&mutx);

}