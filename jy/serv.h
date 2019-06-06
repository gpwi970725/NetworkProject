#include "info.h"

User list[5];
Room rlist[5];

int sd;
int rsd;
int rcnt = 0;
int usernum = 0;
int rusernum = 0;
pthread_mutex_t usermutex, roommutex;

void SigExit(int signo);
int ServerSetting(char *ip, int port);

void *JoinChat(void *user);
void *DeliveryMessage(void *user);
void *Notice(void *user);
void Whisper(User user, char *rbuf);
void ClientExit(User user);
void *thrdmain(void *room);
void FileIO(User user, char *rbuf);

