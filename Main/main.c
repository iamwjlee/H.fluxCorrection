

#include <stdio.h>
#include <stdlib.h>

#include <windows.h>
#include <conio.h>

#include "timer.h"

#include "flux_correction.h"

// -----------------------------------------------------------------------------
int msec0=0;
int msec1=0;
int msec2=0;
int state=0;
int cliter=0;
	int temp;
int pStop=0;
int smallValve=0;

unsigned int pulseCnt=0;
// -----------------------------------------------------------------------------

static void flux_test();

// -----------------------------------------------------------------------------

char get_key() {
	char ch=0;
    if(kbhit()) {
        ch= getch();
		//printf("%c",ch);
	}		
	return ch;
}

void timer_handler(void)
{
	static int delay=0;
	if(msec0) msec0--;
	if(msec1) msec1--;
	if(smallValve) {
		if(++delay) {
			if(delay>10) {
				pulseCnt++;
				delay=0;
			}

		}
	}
	else {
		pulseCnt++;
	}

	if(msec2) msec2--;
	if(!msec2) { //every 20msec
		msec2=10;
		if(state==0) return;
		cliter=flux_correction(pulseCnt,0);
		if(cliter > pStop) {
			smallValve=1;
		}

		//flux_parameter_set(30,570,0,temp/5, 0,0);
	}



}



int main( int argc, char* args[] )
{
	//static int count=0;
	printf("Hello\n");

	char key;
	char keyBuffer[64];
	int keyIndex=0;
    start_timer(2, &timer_handler);
	memset(keyBuffer,0,sizeof(keyBuffer));

    while(1) {
		key=get_key();
		if(key==0) continue;
		printf("%c",key);			
		keyBuffer[keyIndex]=key;

		if(keyBuffer[keyIndex]=='\r') {
			keyBuffer[keyIndex]=0;
			keyIndex=0;
			printf("\n");

			char *ptr = strtok(keyBuffer, " ");
			char argv[8][8]={0};
			int argc=0;
			while(ptr!=NULL) {
				strcpy(argv[argc++],ptr);
				ptr=strtok(NULL," ");
			}

			if(!strcmp(keyBuffer,"quit")) {
				break;
			}
			else if(!strcmp(keyBuffer,"test")) {
				flux_correction_test();
				
			}
			else if(!strcmp(keyBuffer,"flux")) {
				flux_test();
				
			}	
			else if(!strcmp(keyBuffer,"abc")) {
				if(argv[1]) {
					printf("my test[%s]\n",argv[1]);
				}
				else {		
					printf("my test\n");
				}
			}		

			memset(keyBuffer,0,sizeof(keyBuffer));
		}
		else {
			keyIndex++;
		}


	}

	#if 0
    while(1) {
		
		//keyBuffer[keyIndex++]=get_key();
		
		key=get_key();
		if(key=='q') break;
		else if(key=='\r') {
			//keyIndex=0;
			printf("\n");
			//memset(keyBuffer,0,sizeof(keyBuffer));


		}
		else if(key=='1') flux_correction_test();
		else if(key=='2') flux_test();
	
		if(!msec0) {
			msec0=500;
			//printf("%d",count++);
			//printf("\r");
			//printf(".");
		}

	}	
	#endif
	printf("exit program\n");

	return 0;
}

void flux_test() {
	int pLiter;
	temp=100;
	//temp=240;
    flux_parameter_set(30,570,0,temp/5, 0,1);
	//rLiter=0;
	pLiter=2000;
	
	//charge start init
	pulseCnt=0;
	state=1;
	cliter=0;	
	pStop=pLiter-100;
	smallValve=0;

	//for(int i=0;i<pLiter+1;i++) {
	while(1) {
		//rLiter++;
		//Liter=(int)flux_correction(pulseCnt);
		//if(Liter>=pLiter) break;
		if(cliter>=pLiter) {
			printf("temp=%d pLiter=%d cliter=%d\n",temp,pLiter,cliter);
			state=0;
			break;
		}
	}
	
	printf("pulseCnt=%d cliter=%d\n",pulseCnt,cliter);
	/*
		실제로 비중이 온도에 따라 0.001 차이가 
		10도 564
		24도 581
		차이는 17
		0.017*20000=340 ?

		사실 LPG는 1도가 오르면 부피가 0.2~0.3%(동일한 압력일 때) 늘어난다. 
		1L당 0.002~0.003L의 오차가 생긴다는 거다.
		출처 : 더스쿠프(http://www.thescoop.co.kr)
temp=10 pLiter=20000 rLiter=19420 
temp=100 pLiter=20000 rLiter=19792 
temp=200 pLiter=20000 rLiter=20213 
temp=300 pLiter=20000 rLiter=20617
temp=400 pLiter=20000 rLiter=21031 

temp=240 pLiter=20000 rLiter=20398 
temp=250 pLiter=20000 rLiter=20436 


		10 1.011 0.011
		20 0.989 0.011
	*/

}