#include "headerFiles.h"  #define SERVER_IP "127.0.0.1" #define BUF_SIZE 1024 #define MAX_USER_NUM 10 #define MAX_USER_NAME_LEN 20 #define MAX_USER_ID_LEN 30 #define MAX_ROOM_NAME_LEN 30 #define MAX_LIST_LEN 10  typedef struct user { 	int userPW; 	int userCnt; 	char userNAME[MAX_USER_NAME_LEN]; 	char userID[MAX_USER_ID_LEN]; }User;  typedef struct room { 	int portNumber; 	char roomName[MAX_ROOM_NAME_LEN]; 	int placeOrInterest; 	char placeOrInterestDetail[30]; 	User userList[MAX_LIST_LEN]; }Room;  typedef struct message { 	Room room; 	char content[BUF_SIZE]; }Message;  User userList[MAX_LIST_LEN]; Room roomList[MAX_LIST_LEN];  int userCnt=0, roomCnt=0; int client_sockets[MAX_USER_NUM]; int portNumberList[MAX_LIST_LEN] = {2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009}; int mainPortNumber[MAX_LIST_LEN] = {1000};  pthread_mutex_t userMutex, roomMutex;  void makeRoom(void *room); void *makeRoomThread(void *room); int serverSetting(int portNumberList[MAX_LIST_LEN]); void *joinChat(void *user, int server_socket); void *sendMessage(void *room); void *notice(void *user, void *room);  int main(int argc, char *argv[]) { 	int server_socket, client_socket, client_addr_size, userSelect; 	struct sockaddr_in server_addr={0}, client_addr={0}; 	User user;  	pthread_mutex_init(&userMutex, NULL); 	pthread_mutex_init(&roomMutex, NULL);  	pthread_t main_thread; 	pthread_t room_thread[256];  	server_socket = serverSetting(mainPortNumber);  	while(1) { 		client_addr_size = sizeof(client_addr); 		client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_size);  		printf("1. 회원가입 : 1 입력\n"); 		printf("2. 로그인 : 2 입력\n"); 		fgets(&userSelect, sizeof(userSelect), stdin);  		pthread_mutex_lock(&userMutex);  		client_sockets[userCnt] = client_socket; 		 		switch(userSelect) { 			case 1: 				if(signUp() == -1) { 					perror("존재하는 ID 입니다\n"); 				} 				break;  			case 2: 				if(login() == -1) { 					perror("Login error\n"); 				} 				else { 					printf("1. 채팅방 생성 : 1 입력\n"); 					printf("2. 채팅방 검색 : 2 입력\n"); 					fgets(&userSelect, sizeof(userSelect), stdin);  					switch(userSelect) { 						case 1: 							makeRoom(roomList, roomCnt); 							joinChat(user, server_socket); 							break; 						case 2: 							searchRoom(roomList, roomCnt); 							break; 					} 				} 				break; 		}  		pthread_mutex_unlock(&userMutex);  	return 0; }   void makeRoom(void *room, int roomCnt) { 	char roomName[MAX_ROOM_NAME_LEN]; 	int placeOrInterest; 	char placeOrInterestDetail[30]; 	 	printf("Enter Room Name : "); 	fgets(roomName, sizeof(roomName), stdin); 	roomList[roomCnt].roomName = roomName;  	printf("Enter 0(=Place) or 1(=Interest) : "); 	fgets(placeOrInterest, sizeof(placeOrInterest), stdin); 	roomList[roomCnt].placeOrInterest = placeOrInterest;  	printf("Enter Place or Interest Detail : "); 	fgets(placeOrInterestDetail, sizeof(placeOrInterestDetail), stdin); 	roomList.[roomCnt].placeOrInterestDetail = placeOrInterestDetail;  	pthread_create(&room_thread[roomCnt], NULL, makeRoomThread, &room); 	pthread_detach(room_thread[roomCnt]); 	printf("<%s> room created.", roomName); 	 	roomCnt++; }  void *makeRoomThread(void *room) { 	pthread_mutex_lock(&roomMutex); 	int server_socket=0;  	server_socket = serverSetting(portNumberList);  	pthread_mutex_unlock(&roomMutex); }  int serverSetting(int portNumber[MAX_LIST_LEN]) { 	int server_socket;  	server_socket = socket(AF_INET, SOCK_STREAM, 0);  	memset(&server_addr, 0, sizeof(server_addr)); 	server_addr.sin_family = AF_INET;  	server_addr.sin_port = htons(SERVER_IP); 	server_addr.sin_addr.s_addr = htonl(portNumber[roomCnt]);  	roomList[roomCnt].portNumber = portNumber[roomCnt];  	if(bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) { 		perror("bind error"); 		close(server_socket); 		return -1; 	}  	if(listen(server_socket,10) == -1) { 		perror("listen error"); 		close(server_socket); 		return -1; 	}

	return server_socket; }  void *joinChat(void *user, int server_socket) {
	User newUser = *(User*)user;
	pthread_t ptr[2];
	int client_socket;

	client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_size);

	printf("<%s> 접속\n", newUser.userID);

	userList[userCnt] = newUser;

	pthread_create(&ptr[0], NULL, sendMessage, &room);
	pthread_create(&ptr[1], NULL, notice, &newUser, &room);
	pthread_join(ptr[0], NULL);
	pthread_join(ptr[1], NULL);
}

void *sendMessage(void *room) {
	Room currentRoom = *(Room*)room;
	User users[MAX_LIST_LEN] = currentRoom.userList;
	char messageBuf[BUF_SIZE];
	int i;

	fgets(messageBuf, sizeof(messageBuf), stdin);

	pthread_mutex_lock(&mutx);
	if(!strcmp(messageBuf, "/sendfile") {
		fileSend(client_socket, client_sockets, currentRoom.userCntInRoom);
	}
	else {
		for(i=0; i<currentRoom.userCntInRoom; i++)
			write(client_sockets[i], msg, BUF_SIZE);
	}
	pthread_mutex_unlock(&mutx);
}

void *notice(void *user, void *room) {
	int i;
	User newUser = *(User*)user;
	Room currentRoom = *(User*)room;
	char stringBuf[BUF_SIZE];
	char userInputBuf[BUF_SIZE];

	sprintf(stringBuf, "%s님이 접속하였습니다.\n", newUser.userID)
	for(i=0; i<currentRoom.userCntInRoom; i++){
		send(currentRoom[i].usd, sbuf, strlen(sbuf), 0);
	}

	while(1) {
		memset(sbuf, 0, sizeof(sbuf));
		memset(inbuf, 0, sizeof(inbuf));
		fgets(inbuf, sizeof(inbuf), stdin);
		sprintf(sbuf,"[공지]:%s\n", inbuf);
		for(i=0; i<usernum; i++) {
			send(list[i].usd, sbuf, strlen(sbuf),0);
		}
	}
}