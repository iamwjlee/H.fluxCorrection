



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
#include "soc.h"
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
static void unionTest();
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



 
// -------------------- 여기서 부터 ------

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
	// 	printf("Test1 기본생성자\n");
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

	//구조체 멤버 접근
	Test2 t2; //객체생성(인스턴스)
	t2.a=1;
	t2.b=2;
	printf("Test2 구조체 멤버 접근 %d %d\n",t2.a,t2.b);
	//구조체 초기화(중첩된)
	Test2 t22={{1965,2}, 10,11};
	//Test2 t22={{}, 10,11};	//객체생성(인스턴스)
	printf("Test2 구조체 초기화 %d %d %d %d\n",t22.a,t22.b,t22.bir.year,t22.bir.month);

	t22.bir.setYear(1966);
	printf("Test2 구조체 멤버값 변경 %d %d %d %d\n",t22.a,t22.b,t22.bir.year,t22.bir.month);
	

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
#include "windows.h"
#include "Psapi.h"
#include "Shlwapi.h"
#include <string>
#include <iostream>
int ProcessName(ULONG_PTR ProcessId, char *p)
{
    char szBuffer[MAX_PATH+1]="unknown";
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_TERMINATE, FALSE, ProcessId);

     if(GetModuleFileNameEx(hProcess, NULL, szBuffer, MAX_PATH) == 0) {
         sprintf(szBuffer, "null");
	 }

    CloseHandle(hProcess);
	printf("szBuffer=%s\n",szBuffer);
	strcpy(p,szBuffer);
    return 0;
}


void myFunction5() {

 
    wchar_t buffer[MAX_PATH] = {};
    if (GetModuleFileNameW(nullptr, buffer, MAX_PATH)) { // current HMODULE = nullptr
    std::wcout << L"current process name : " << buffer << std::endl;
    } 
	char p[64];
	ProcessName(18692,p);
	printf("결과 %s\n",p);


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

void polling_rx() {
	/*
	qr,print,
	호기, pp,setup data,tm,st에 대한 시간 응답
	*/
	printf("폴링 먼저 받어\n");

}

void polling_tx() {
	static UINT8 FFCheck=0;	
	for(int i=0;i<10;i++) {
		if(!FFCheck--) {
			FFCheck=1;
			//호기++
			printf("호기변경후에 en 보내\n");
		}
		else {
			//만약 보낼것이 있다면 보내고
			//아니면 en 보낸다
			printf("en 또는 cmd전송\n");
		}
	}
}
void polling_test() {
	// 마스터는 먼저 폴링을 시작해야 하는데 먼저보내고 응답을 받아야 
	/*
		보내고
		받고,확인후 
		다시 보내고 를 반복해야
	*/
	polling_rx();
	polling_tx();

}
//구상

void DisplayError() {
	printf("printer error!!\n");
}
typedef void (*ShowDialog)(void);
typedef void (*SendState)(void);
typedef struct {
	int errorCode;
	int displayed;
	//다이얼로그 화면에 표시되어야
	ShowDialog show;
	//SK인경우는 프린트상태를 전송해주어야 
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
	// 시간되면 얘불러서 
	// 응답을 얻어온다.
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
	//printf("시간 됐다!\n");
}
void MyTimerFunction2() {
	printf("10초\n");
}


void setInterval2(int *timer, int interval, TimerCallback callback) {
	if(*timer==0) {
		*timer=interval;
		callback();
	}
}

void setInterval(TimerCallback callback,unsigned char delaySecond) {
	//이거는 한번만 사용가능, 여기 저기서 마구 쓸려면?
	static int first=0;
	static signed char interval;
	if(first==0) {
		interval=count128;
		first=1;
	}
	if( (unsigned char)(count128-interval) >delaySecond) {
		//printf("100초 지났다\n");
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
		printf("char 포인터의 내용유무 확인\n");
		if(*tmp) {
			printf("tmp is [%s]  [0x%x] [0x%x]\n",tmp,*tmp,*(tmp+1));

		}
		else {
			printf("tmp ~is [%s]  [0x%x] [0x%x]\n",tmp,*tmp,*(tmp+1));
		}
	}

	int testCount=0;
	printf("보이는지 여부\n");
	if(testCount++%100==0) {
		printf("이거보여!\n");
	}
	{
		unsigned char a='a';
		unsigned char b='b';
		a&=~0x20;
		b&=~0x20;
		printf("a=%02x a[%c] b=%02x\n",a,a,b);

	}
	unionTest();

	soc_init(8764);
    while(1) {
		soc_action();

		key=get_key();
		if(key=='q') {
			soc_stop();
			// Sleep(100);
			break;
		}
		else if(key=='\r') printf("\n");
		else if(key=='1') flux_correction_test();
		else if(key=='2') flux_test();
		else if(key=='3') band_maid();
		else if(key=='4') {
			if(++JobPtr>=3) JobPtr=0;
			JobInit=1; //잡실행
			signed char a=100;
			signed char b=-100;
			signed char r=b-a;
			printf("결과=%d\n",r);

			 a=-100;
			 b=10;
			 r=b-a;
			printf("결과=%d\n",r);

		}
		else if(key=='5') {
			printf("key5 pressed\n");
			myFunction5();
		}
		else if(key=='6') {
			polling_test();
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


	soc_close();
	printf("main exit(1)\n");
	exit(1);
	return 0;
}

void unionTest() {
	union {
		struct {
			char b;
			char c;
			char d;
			char e;
		}s;
		// char b;
		// char c;
		// char d;
		// char e;
		int a;
	}u;
	u.a=0x12345678;
	printf("u.a=0x%x 0x%x 0x%x 0x%x 0x%x\n",u.a,u.s.b,u.s.c,u.s.d,u.s.e);
	u.s.b=0x11;
	u.s.c=0x22;
	u.s.d=0x33;
	u.s.e=0x44;
	printf("u.a=0x%x 0x%x 0x%x 0x%x 0x%x\n",u.a,u.s.b,u.s.c,u.s.d,u.s.e);

	//okay
	// union {
		
	// 	char b[4];
	// 	int a;
	// }u;
	// u.a=0x12345678;
	// printf("u.a=0x%x 0x%x 0x%x 0x%x 0x%x\n",u.a,u.b[0],u.b[1],u.b[2],u.b[3]);
	// u.b[0]=0x11;
	// u.b[1]=0x22;
	// u.b[2]=0x33;
	// u.b[3]=0x44;
	// printf("u.a=0x%x 0x%x 0x%x 0x%x 0x%x\n",u.a,u.b[0],u.b[1],u.b[2],u.b[3]);


}
// struct {
//     uint32_t	Data;
// 	uint32_t     Otp;
// 	union {
// 		uint32_t  LogCount;
// 		uint8_t     Temperature; //15도 고정값
// 		uint8_t 	CodeCostUse; //거래처별단가사용여부 사용안함
// 		uint8_t	 	rTemp; //기준온도(30)고정값 위에 Temperature와 별개!
// 		uint8_t     momo1;
// 	};
// 	uint16_t 	Bejung; //기준비중(570)고정값
// 	uint8_t 	FlowMeter; //유량계 보정값 사용안함
// 	uint8_t 	momo2;
// }Setup_t;





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
		실제로 비중이 온도에 따라 0.001 차이가 
		10도 564
		24도 581
		차이는 17
		0.017*20000=340 ?

		사실 LPG는 1도가 오르면 부피가 0.2~0.3%(동일한 압력일 때) 늘어난다. 
		1L당 0.002~0.003L의 오차가 생긴다는 거다.
		출처 : 더스쿠프(http://www.thescoop.co.kr)
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
		printf("----< 이상행\n");
	}
	printf("a-b=%d\n",c);

}