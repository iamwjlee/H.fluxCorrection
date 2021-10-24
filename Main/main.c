

#include <stdio.h>
#include <stdlib.h>

#include <windows.h>
#include <conio.h>

#include "timer.h"

#include "flux_correction.h"

// -----------------------------------------------------------------------------
int msec0=0;
int msec1=0;

// -----------------------------------------------------------------------------

static void flux_test();

// -----------------------------------------------------------------------------

char get_key() {
	char ch=0;
    if(kbhit()) {
        ch= getch();
		printf("%c",ch);
	}		
	return ch;
}

void timer_handler(void)
{
  if(msec0) msec0--;
  if(msec1) msec1--;

}

int main( int argc, char* args[] )
{
	printf("Hello\n");

	char key;
    start_timer(100, &timer_handler);

    while(1) {
		key=get_key();
		if(key=='q') break;
		else if(key=='\r') printf("\n");
		else if(key=='1') flux_correction_test();
		else if(key=='2') flux_test();
	
		if(!msec0) {
			msec0=10;
			printf(".");
		}


	}	
	printf("exit program\n");

	return 0;
}

void flux_test() {
	int Liter;
	int rLiter;
	int pLiter;
    //flux_parameter_set(30,570,0, 60, 0);
	int temp;
	temp=250;
	//temp=240;
    flux_parameter_set(30,570,0,temp/5, 0);
	rLiter=0;
	pLiter=20000;

	//for(int i=0;i<pLiter+1;i++) {
	while(1) {
		rLiter++;
		Liter=(int)flux_correction(rLiter);
		//printf("rLiter=%d Liter=%d\n",rLiter,Liter);
		if(Liter>=pLiter) break;
		//rLiter+=100;
		//rLiter=i;
	}

	printf("temp=%d pLiter=%d rLiter=%d Liter=%d\n",temp,pLiter,rLiter,Liter);

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
	*/

}