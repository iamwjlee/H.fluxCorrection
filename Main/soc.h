#ifndef _soc_h
#define _soc_h


#define MAX_DATASIZE 512
#define MAX_CLIENTS 5

typedef struct {
    int cs;
    char name[64];//ip
}clients_info_t;
// extern clients_info_t;
typedef struct {
	unsigned char data[2048];
	int size;
    int fd;
    int idx;
    int portNumber;
    char ipAddress[64];
}rcv_text_t;



int soc_init(int port);


// extern int socRun;
void soc_stop(); 
int soc_close();
int soc_action();
void log_msg(const char *msg,...);
#endif
