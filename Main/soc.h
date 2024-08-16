#ifndef _soc_h
#define _soc_h

typedef struct {
    int cs;
    char name[32];
}clients_info_t;
// extern clients_info_t;

int soc_test();


// extern int socRun;
int soc_test_stop();

void log_msg(const char *msg,...);
#endif
