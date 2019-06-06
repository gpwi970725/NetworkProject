#include "serv.h"
#include <string.h>

int main()
{
       int room, i;
        signal(SIGINT, SigExit);

        pthread_mutex_init(&usermutex, NULL);
        pthread_mutex_init(&roommutex, NULL);
    	printf("방 개수를 입력하세요\n");
	scanf("%d", &room);
	getchar();
        pthread_t ptr[256];

	for(i=0; i<room; i++) {
	
		pthread_create(&ptr[i], NULL, thrdmain, &room);
		pthread_detach(ptr[i]);
	}
	while(1)
		pause();
	return 0;
}

void *thrdmain(void *room)
{
	 pthread_mutex_lock(&roommutex);
	 User user;
         Room rm;

         int ssd;
 	 int port = SERV_PORT +rcnt;
	 rm.ucnt = rcnt;
         ssd = socket(AF_INET,SOCK_STREAM,0);

         struct sockaddr_in servaddr = {0};
	 servaddr.sin_family = AF_INET;
	 servaddr.sin_port = htons(port);
	 servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	 if(bind(ssd, (struct sockaddr*)&servaddr, sizeof(servaddr)))
	 {
		 perror("bind error");
		 close(ssd);
	 }
	 if(listen(ssd,5) == -1)
	 {
		 perror("listen error");
		 close(ssd);
	 }

        printf("port%d\n", port);

        rcnt++;

        pthread_mutex_unlock(&roommutex);

        struct sockaddr_in cliaddr = {0};
        int clen = sizeof(cliaddr);
        pthread_t ptr;

        while((user.usd = accept(ssd, (struct sockaddr*)&cliaddr, &clen)))
        {
                if(user.usd == -1)
                {
	                        perror("accept");
	                        return 0;
	                }

                pthread_mutex_lock(&usermutex);

                user.unum = usernum;

                user.rsd = ssd;
       
      		rm.rsd = ssd;
                rm.usd[rm.ucnt] == user.usd;

                rlist[rm.ucnt] = rm;
                pthread_create(&ptr, NULL, JoinChat, &user);
                pthread_detach(ptr);
                usernum++;
                rm.ucnt++;
		pthread_mutex_unlock(&usermutex);						   
	}
	return 0;
 }

 void *JoinChat(void *user)
 {
	 User us = *(User*)user;
	 char nick[MAX_NICK_LEN]="";
	 pthread_t ptr[2];

	 send(us.usd,"Hello!\n닉네임을 입력하세요.\n", 100,0);
         recv(us.usd, nick,sizeof(nick),0);
       	 nick[strlen(nick)-1] = '\0';
	 printf("%s접속\n", nick);
			
	 strcpy(us.unick, nick);
				
	 list[us.unum] = us;
					
//	 pthread_create(&ptr[0], NULL, DeliveryMessage, &us);
//	 pthread_create(&ptr[1], NULL, Notice, &us);
	 pthread_join(ptr[0], NULL);
	 pthread_join(ptr[1], NULL);
 }


 int ServerSetting(char *ip, int port) {
	 int ssd;
	 ssd = socket(AF_INET, SOCK_STREAM,0);
	 struct sockaddr_in servaddr = {0};
	 servaddr.sin_family = AF_INET;
	 servaddr.sin_port = htons(port);
	 servaddr.sin_addr.s_addr = inet_addr(ip);
	 if(bind(ssd, (struct sockaddr*)&servaddr, sizeof(servaddr))){
		 perror("bind error");
		 close(ssd);
		 return -1;
	 }
	 if(listen(ssd,5) == -1) {
		 perror("listen error");
		 close(ssd);
		 return -1;
	 }
	 return ssd;
 }

 void SigExit(int signo) {
	 printf("서버를 종료합니다.\n");
	 close(rsd);
	 close(sd);
	 exit(0);
 }
