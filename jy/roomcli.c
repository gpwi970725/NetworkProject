#include "cli.h"
#include <string.h>

#define MAX_NUM 20

typedef struct {
		char roomname[30];
}Roomlist;

void Insert(Roomlist*ptr, int*num);
void Display(Roomlist*ptr, int*num);
void JoinChat(int ssd);

int main(void){
	Roomlist roomlist[MAX_NUM];
	int input;
	int num;
	int room = 0;
	while(1){
		printf("================\n");
		printf("|     MENU      |\n");
		printf("================\n");
		printf("| (1)채팅방 생성|\n");
		printf("| (2)채팅방 조회|\n");
		printf("================\n");

		printf("\n원하는 작업을 선택하세요.:");
		scanf("%d",&input);

		if(input == 1){
			Insert(roomlist,&room);
	     //   	Join Chat(sd);	
		}
		else if(input == 2){
			printf("\n현재 채팅방 목록\n");
			Display(roomlist,&room);
		}
		else
			printf("\n잘못된 값을 입력하였습니다.\n");
	}
	return 0;
}

void Insert(Roomlist *ptr, int*num){
	if(*num<MAX_NUM){
		printf("생성할 채팅방 이름:");
		scanf("%s", ptr[*num].roomname);
		(*num)++;
		printf("생성 완료!\n\n");
	}
	else printf("fail!\n");
}

void Display(Roomlist*ptr, int*num){
	int i=0;

	if(*num > 0) {
		for(i=0; i<*num; i++){
			printf("%s\n\n",ptr[i].roomname);
		}
	}
	else
		printf("존재하는 채팅방이 없습니다.\n");
}

/*int main() {
	int port;
	char pt[MAX_MESSAGE_LEN];

	printf("포트를 입력하세요:");
	scanf("%d", &port);
	getchar();
	signal(SIGINT, SigExit);

	pthread_mutex_init(&usermutex, NULL);

	if((sd = SockSetting(SERV_IP, port)) == -1) {
		perror("socket");
		return 0;
	}

	JoinChat(sd);

	return 0;
}*/

void JoinChat(int ssd) {
	User user;
	pthread_t ptr[2];
	char nick[MAX_NICK_LEN] = "";
	char rbuf[MAX_MESSAGE_LEN] = "";

	recv(ssd, rbuf, 100, 0);
	fputs(rbuf, stdout);
	fgets(nick,sizeof(nick), stdin);
	send(ssd, nick, strlen(nick),0);
	user.usd = ssd;
	strcmp(user.unick, nick);

//	pthread_create(&ptr[0], NULL, RecvMsg, &user);
//	pthread_detach(ptr[0]);
//	pthread_create(&ptr[1],NULL,SendMsg, &user);
//	pthread_detach(ptr[1]);

	while(1)
		pause();
}

void *RecvMsg(void *user){
	User us = *(User*)user;
	char rbuf[MAX_MESSAGE_LEN];
	while(1)
	{
		if(flag ==0){
			recv(us.usd, rbuf, sizeof(rbuf), 0);
		}

		while(recv(us.usd, rbuf, sizeof(rbuf), 0)>0)
		{
		       	fputs(rbuf, stdout);
			memset(rbuf, 0, sizeof(rbuf));
		}
	}
}


int SockSetting(char *ip, int port)
{
	int ssd;
	if((ssd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("sock error");
		return -1;
	}

	struct sockaddr_in servaddr = {0};
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(ip);
	servaddr.sin_port = htons(port);

	if(connect(ssd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
	{
		perror("connect");
		return -1;
	}
	return ssd;
}
void SigExit(int signo)
{
	printf("클라이언트를 종료합니다.\n");
	close(sd);
	exit(0);
}
