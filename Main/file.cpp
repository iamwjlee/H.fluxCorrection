
#include <stdio.h>
#include <stdlib.h>

#include <windows.h>


#include <time.h>
#include <SDL2/SDL.h>


/*
    로그기록 
        테스트파일어펜드방식
        바이너리파일어펜드방식

	파일에 내용을 계속 추가만한다. 10개이상의 라인이 쌓이면 파일을 다른 이름으로 저장하고
	새롭게 파일을 만들고 추가를 시작한다!
	-> 바이너리파일로 적용가능한지
*/
/*

https://github.com/iamwjlee/H.fluxCorrection.git
*/
typedef struct {
    int a;
    int b;
}food_s;
food_s food;
void save_binary() {

    FILE *fp;
    static int a=1;
    static int b=1;
    clock_t start;
    start=clock();
    fp=fopen("meb.log","a+b");
    food.a=a;
    food.b=b;
    a++;
    b++;
	fseek(fp,0,SEEK_END);
	int size=ftell(fp);
	int food_size=size/(sizeof(food_s));

  	fseek(fp,0,SEEK_SET);
    fwrite(&food,sizeof(food_s),1,fp);
    fclose(fp);
    printf("append writing food.a=%d food.b=%d food_size=%d filesize=%d time=%d\n",food.a,food.b,food_size,size,clock()-start);

    food_s res;
    fp=fopen("meb.log","r+b");
   	fseek(fp,sizeof(food_s)*food_size,SEEK_SET);
    fread(&res,sizeof(food_s),1,fp);
    fclose(fp);
    printf("last reading res.a=%d res.b=%d\n",res.a,res.b);
}
void save_me() {
	struct tm lt;
	time_t t=time(NULL);
	lt=*localtime(&t);

	char line[128];
	char column[32];
	sprintf(column,"%d-%02d-%02d:%02d:%02d:%02d ",lt.tm_year+1900,lt.tm_mon+1,lt.tm_mday,lt.tm_hour,lt.tm_min,lt.tm_sec);
	//printf(tmp);
	memset(line,0,sizeof(line));
	strcat(line,column);

	static int a=1000;
	static int b=230;
	sprintf(column,"a:%d b:%d\n",a,b);
	strcat(line,column);
	a++;
	b++;
	FILE *fp;
    //int currentTime;
    //currentTime=SDL_GetTicks();

	fp=fopen("me.log","a");
	fseek(fp,0,SEEK_END);
	int size=ftell(fp);
	int current_lines=size/(strlen(line)+1);
	if(current_lines>=10) {
		fseek(fp,0,SEEK_SET);
		fprintf(fp,"%s",line);
		fclose(fp);
		char newfileName[64];
		sprintf(newfileName,"me-%02d%02d%02d-%02d%02d%02d.log",lt.tm_year+1900,lt.tm_mon+1,lt.tm_mday,lt.tm_hour,lt.tm_min,lt.tm_sec);
		rename("me.log",newfileName);
		return;
	}


	fseek(fp,0,SEEK_SET);
	fprintf(fp,"%s",line);
	fclose(fp);
   	printf("file size=%d line size=%d actual current lines=%d \n",size,strlen(line),current_lines);

	return;	

}

void read_text_line() {
	FILE *fp;
	int lines=0;
	fp=fopen("me.log","r");
	if(fp==NULL) return;
	char strLine[128];
	while(fgets(strLine,sizeof(strLine),fp)!=NULL){
		printf(strLine)	;
		lines++;
	}
	
	fclose(fp);
	printf("lines=%d\n",lines);

}

