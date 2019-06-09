#include "headerFiles.h"  #define SERVER_IP "127.0.0.1" #define BUF_SIZE 1024 #define MAX_USER_NUM 10 #define NAME_SIZE 20 #define NOTSET 0 #define EXIST 1 #define NOTEXIST 2  void showMenu(); void JoinChat(int ssd); void *RecvMsg(void *user); void *SendMsg(void *user); int SockSetting(char *ip, int port);  int main(int argc, char *argv[]) {  	scanf("%d"  	return 0; }  void showMenu() { 	printf("\n");
	printf("[MENU]\n\n");
	printf("1. /menu -> some orders \n");
	printf("2. /sendfile -> 1:N file transfer \n");
	printf("3. /exit -> chatting program exit \n");
	printf("\n[END MENU] \n\n"); }  void joinChat(int ssd) {
	User user;
	pthread_t ptr[2];
	char userID[MAX_USER_ID_LEN] = "";
	char receiveBuf[MAX_MESSAGE_LEN] = "";

	recv(ssd, receiveBuf, 100, 0);
	fputs(receiveBuf, stdout);
	user.usd = ssd;

	pthread_create(&ptr[0], NULL, recvMessage, &user);
	pthread_detach(ptr[0]);
	pthread_create(&ptr[1],NULL, sendMessage, &user);
	pthread_detach(ptr[1]);

	while(1) {
		pause();
	}
}

void *recvMessage(void *room) {
	Room currentRoom = *(Room*)room;
	User userList[MAX_USER_NUM] = currentRoom.userList;
	char receiveBuf[BUF_SIZE];
	int userCnt=0;

	while(1) {
		if(flag == 0) {
			recv(us.usd, rbuf, sizeof(rbuf), 0);
		}

		while(recv(userList[userCnt].userCnt, receiveBuf, sizeof(receiveBuf), 0) > 0) {
			fputs(receiveBuf, stdout);
			memset(receiveBuf, 0, sizeof(receiveBuf));
			userCnt++;
		}
	}
}

void *sendMessage(void *room) {
	Room currentRoom = *(Room*)room;
	User userList[MAX_USER_NUM] = currentRoom.userList;
	char sendBuf[BUF_SIZE];

	while(1) {
		fgets(sendBuf, sizeof(sendBuf), stdin);
		send(us.usd, sendBuf, sizeof(sendBuf),0);
		if(!strncmp(sendBuf,"/sendfile", 9)) {
			flag = 1;
		}

		memset(sbuf, 0, sizeof(sbuf));
	}
}

int sockSetting(int portNumber[MAX_LIST_LEN]) {
	int client_socket;

	if((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket error");
		return -1;
	}

	struct sockaddr_in server_addr = {0};
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(ip);
	servaddr.sin_port = htons(port);

	if(connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
		perror("connect error");
		return -1;
	}

	return client_socket;
}