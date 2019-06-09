#include "headerFiles.h"

int fileSend(int client_sock, int clnt_socks[MAX_USER_NUM], int userCntInRoom) {
	int i;
	char file_msg[BUF_SIZE]={NULL}, tmpName[NAME_SIZE]={NULL};
	const char Fmsg_end[BUF_SIZE] = {"FileEnd_cl->sr"};

	pthread_mutex_init(&userMutex, NULL);
	pthread_mutex_lock(&userMutex);						

	while(1) {
		read(client_socket, file_msg, BUF_SIZE);
		if(!strcmp(file_msg, Fmsg_end)) {
			break;
		}
		for(i=0; i<userCntInRoom; i++) {
			if(client_socket == client_socks[i]) {
				continue;
			}
			write(client_sockets[i], file_msg, BUF_SIZE);
		}
	}

	for(i=0; i<userCntInRoom; i++) {
		if(client_socket == client_sockets[i]) {
			continue;
		}
		write(clnt_socks[i], "FileEnd_sr->cl", BUF_SIZE);
	}

	pthread_mutex_unlock(&userMutx);
	printf("(!Notice)File data transfered \n");

	return 0;
}