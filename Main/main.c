



#include <stdio.h>
#include <stdlib.h>

#include <windows.h>
//#include <iostream>
#include <string>

#include <conio.h>
#include "timer.h"

#include "flux_correction.h"
#include "db.h"
#include <time.h>
#include "file.h"
/*
dlltool -D sqlite3.dll -d sqlite3.def -l libsqlite3dll.a
*/
using namespace std;
// -----------------------------------------------------------------------------
int msec0=0;
int msec1=0;

// -----------------------------------------------------------------------------

static void flux_test();
static void band_maid();
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
void printHello() {
	printf("my test[%s]\n",__FUNCTION__);
}
void printHello1() {
	printf("my test[%s]\n",__FUNCTION__);
}

void (*funcJob[2])()={
	printHello,
	printHello1
};



 
// -------------------- ���⼭ ���� ------

typedef void (*job_function)();

typedef struct {
	char name[20];
	job_function job; 

}job_t;
void MSetup();
void MCharge();
void MPayment();

job_t workUI[3]={
	"setup",MSetup,
	"charge",MCharge,
	"payment",MPayment
};

int JobPtr=0;
int JobInit=0;
void MSetup() {
	static int ptr=0;
	if(JobInit) {
		JobInit=0;
		ptr=0;
		printf("setup\n");
	}
	switch (ptr)
	{
	case 0:
		/* code */
		printf("\tsetup 0\n");
		ptr++;
		break;
	case 1:
		printf("\tsetup 1\n");
		ptr++;

		break;
	default:
		break;
	}

}
void MCharge() {
	static int ptr=0;
	if(JobInit) {
		JobInit=0;
		ptr=0;
		printf("charge\n");
	}
	switch (ptr)
	{
	case 0:
		/* code */
		printf("\tcharge 0\n");
		ptr++;
		break;
	case 1:
		printf("\tcharge 1\n");
		ptr++;
		break;
	default:
		break;
	}
}
void MPayment() {
	static int ptr=0;
	if(JobInit) {
		JobInit=0;
		ptr=0;
		printf("payment\n");
	}
	switch (ptr)
	{
	case 0:
		/* code */
		printf("\tpayment 0\n");
		ptr++;
		break;
	case 1:
		printf("\tpayment 0\n");
		ptr++;
		break;
	default:
		break;
	}
}

void my_test() {
	workUI[0].job();
	workUI[1].job();
	printf("[%s]\n",workUI[1].name);

	funcJob[0]();

}

struct Test1 {
public:
	int year;
	int month;
	// Test1() {
	// 	printf("Test1 �⺻������\n");
	// 	year=2020;
	// 	month=1;
	// }
	void setYear(int data) {
		year=data;
	}
};

struct Test2 {
	
	Test1 bir;
	int a;
	int b;
};


void my_test2() {
	int *p = new int;
	*p=120;
	printf("> %d\n",*p);
	delete p;

	//����ü ��� ����
	Test2 t2; //��ü����(�ν��Ͻ�)
	t2.a=1;
	t2.b=2;
	printf("Test2 ����ü ��� ���� %d %d\n",t2.a,t2.b);
	//����ü �ʱ�ȭ(��ø��)
	Test2 t22={{1965,2}, 10,11};
	//Test2 t22={{}, 10,11};	//��ü����(�ν��Ͻ�)
	printf("Test2 ����ü �ʱ�ȭ %d %d %d %d\n",t22.a,t22.b,t22.bir.year,t22.bir.month);

	t22.bir.setYear(1966);
	printf("Test2 ����ü ����� ���� %d %d %d %d\n",t22.a,t22.b,t22.bir.year,t22.bir.month);
	

}
void my_test3() {
	char tmp[8+1]={0};
	memset(tmp,0x20,sizeof(tmp)-1);
	for(int i=0;i<9;i++)
		printf(" size= %d tmp=0x%x \n",sizeof(tmp) ,tmp[i]);


	char muffin[10]="SETUP ab";
	//memset((void *)muffin,0,sizeof(muffin));
	if(!strncmp("SETUP",muffin,5)) {
		printf("setup hit\n");
	}
	else {
		printf("setup not found!\n");
	}

	int exception=4;
	if(exception==3) throw "critical exception";
	if(exception==4) throw 1001;
	printf("next line of exception\n");
	exception++;

}

void myVariableFunction(const char *msg,...) {
	char tmp[1024]={0};
	va_list arg;
	va_start(arg,msg);
	vsprintf(tmp,msg,arg);
	va_end(arg);
	printf("tmp[%s]\n",tmp);
	printf("msg[%s]\n",msg);
	// printf("arg1=%d\n",*(int*)arg++);
	// printf("arg2=%d\n",*(int*)arg++);
	// printf("arg3=%d\n",*(int*)arg++);
}


//����

void DisplayError() {
	printf("printer error!!\n");
}
typedef void (*ShowDialog)(void);
typedef void (*SendState)(void);
typedef struct {
	int errorCode;
	int displayed;
	//���̾�α� ȭ�鿡 ǥ�õǾ��
	ShowDialog show;
	//SK�ΰ��� ����Ʈ���¸� �������־�� 
	SendState send;
	//
}print_state_t;
print_state_t pstate;

void InitPrintState(print_state_t *p) {

	p->errorCode=0;
	p->displayed=0;
	p->show=DisplayError;
	p->send=NULL;
}

void RequestPrintState() {
	// �ð��Ǹ� ��ҷ��� 
	// ������ ���´�.
}
void ResponsePrintState(print_state_t *p) {
	int res=1;
	p->errorCode=res;
	if(p->errorCode && p->displayed==0) {
		p->displayed=1;
		p->show();
	}
	// pstate.errorCode=res;
	// pstate.show=DisplayError;
	// if(pstate.errorCode && pstate.displayed==0) {
	// 	pstate.show();
	// 	pstate.displayed=1;
	// }


}
signed char count128=0;
int count001=0;

typedef void (*TimerCallback)(void);

void MyTimerFunction() {
	//printf("�ð� �ƴ�!\n");
}
void MyTimerFunction2() {
	printf("10��\n");
}


void setInterval2(int *timer, int interval, TimerCallback callback) {
	if(*timer==0) {
		*timer=interval;
		callback();
	}
}

void setInterval(TimerCallback callback,unsigned char delaySecond) {
	//�̰Ŵ� �ѹ��� ��밡��, ���� ���⼭ ���� ������?
	static int first=0;
	static signed char interval;
	if(first==0) {
		interval=count128;
		first=1;
	}
	if( (unsigned char)(count128-interval) >delaySecond) {
		//printf("100�� ������\n");
		callback();
		first=0;
	}
	//printf("Hello d=%d \n",d);

}

int main( int argc, char* args[] )
{
	printf("Hello\n");

	char key;
    start_timer(100, &timer_handler);

	//my_test();
	my_test2();
	try {
		my_test3();
	}
	catch(const char *txtException) {
		printf("Exception %s\n",txtException);
	}
	catch(int exceptionCode) {
		printf("Exception %d\n",exceptionCode);
	}
	catch(...) {
		printf("Exception happened!\n");
	}
	int testId=24;
	myVariableFunction("variable Test key=%d %d %s",testId,testId+100,"Hello");
	JobInit=0;
	JobPtr=0;
	InitPrintState(&pstate);
	{
		const char *tmp="";
		printf("char �������� �������� Ȯ��\n");
		if(*tmp) {
			printf("tmp is [%x]  [%x]\n",tmp,*tmp);

		}
		else {
			printf("tmp ~is [%x]  [%x]\n",tmp,*tmp);
		}
	}
    while(1) {
		key=get_key();
		if(key=='q') break;
		else if(key=='\r') printf("\n");
		else if(key=='1') flux_correction_test();
		else if(key=='2') flux_test();
		else if(key=='3') band_maid();
		else if(key=='4') {
			if(++JobPtr>=3) JobPtr=0;
			JobInit=1; //�����
			signed char a=100;
			signed char b=-100;
			signed char r=b-a;
			printf("���=%d\n",r);

			 a=-100;
			 b=10;
			 r=b-a;
			printf("���=%d\n",r);

		}
		else if(key=='s') booking_test(DB_SHOW);
		else if(key=='r') booking_test(DB_DROP);
		else if(key=='d') booking_test(DB_DELETE);
		else if(key=='i') {
			char latest[32]={0};
			get_latest_log_file_name(latest); //booking_test(DB_INSERT);
			if(strlen(latest)!=0)
				printf("latest log file[%s]\n",latest);
		}
		else if(key=='t') save_binary(); //booking_test(DB_TABLE);
		else if(key=='l') save_text_append(); //booking_test(DB_LAST);
		else if(key=='u') read_text_line(); ////booking_test(DB_UPDATE);
		if(!msec0) {
			msec0=10;
			count128++;
			//msec0=1;
			if(count001>0) count001--;
			//printf("%d ",count128);
		}
		workUI[JobPtr].job();

		//setInterval(MyTimerFunction,7);
		setInterval2(&count001,10,MyTimerFunction);
		ResponsePrintState(&pstate);
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
	//temp=100;
	temp=240;
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

	printf("pLiter=%d rLiter=%d Liter=%d\n",pLiter,rLiter,Liter);

	/*
		������ ������ �µ��� ���� 0.001 ���̰� 
		10�� 564
		24�� 581
		���̴� 17
		0.017*20000=340 ?

		��� LPG�� 1���� ������ ���ǰ� 0.2~0.3%(������ �з��� ��) �þ��. 
		1L�� 0.002~0.003L�� ������ ����ٴ� �Ŵ�.
		��ó : ��������(http://www.thescoop.co.kr)
	*/

}



int IsEmpyString(char *p){
    int len=0;
    int count=0;
    len=strlen(p);
    for(int i=0;i<len;i++) {
        if(*(p+i)==0x20) count++;
    }
	printf("len=%d count=%d\n",len,count);
    if(count==len) return 1;
    else return 0;

}
void band_maid() {

	printf("Hello!\n");
	char passwd[10];
	char pin1[17]={0};
	char pin2[17]={0};
	char pin3[17]={0};
	strcpy(passwd,"1234");
	sprintf(pin1,"%-16s",passwd);
	printf("pin1[%s] %c %c\n",pin1,pin1[0],pin1[15]);
	sprintf(pin2,"%16s",passwd);
	printf("pin2[%s] %c %c\n",pin2,pin2[0],pin2[15]);
	strcpy(pin3,passwd);	
	printf("pin3[%s]\n",pin3);

	string s;
	s+=pin1;
	s+=pin2;
	s+=pin3;
	printf("ss=[%s]\n",s.c_str());
	char tot[64]={0};
	strcpy(tot,s.c_str());
	printf("tot=[%s]\n",tot);

	char tmp[10];
	strcpy(tmp,"         ");
	IsEmpyString(tmp);


	int a=9999;
	unsigned int b=10000;
	int c=abs(a-(int)b);
	if(abs(a-(int)b)>4) {
		printf("----< �̻���\n");
	}
	printf("a-b=%d\n",c);

}