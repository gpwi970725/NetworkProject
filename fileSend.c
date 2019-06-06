#include "headerFiles.h"

#define BUF_SIZE 128
#define MAX_CLNT 2
#define NAME_SIZE 20

pthread_mutex_t mutx;

int fileSend(int oneOrAll, int clnt_sock, int clnt_cnt, char clnt_names[MAX_CLNT][NAME_SIZE], int clnt_socks[MAX_CLNT]) {
	int i, j, fSize = 0, noCli = 0, fileGo = NULL;
	char file_msg[BUF_SIZE]={NULL}, tmpName[NAME_SIZE]={NULL};
	const char Fmsg_end[BUF_SIZE] = {"FileEnd_cl->sr"};
	pthread_mutex_init(&mutx, NULL);

	switch (oneOrAll) {
		case 0 :	// 파일 전송
			read(clnt_sock, tmpName, NAME_SIZE);
			pthread_mutex_lock(&mutx);

			for(j=0; j<clnt_cnt; j++) {
				if(!strcmp(tmpName, clnt_names[j]) ) {
					noCli = 0;
					fileGo = j; // 보낼 소켓 번호를 저장
					break;
				} else if(j == clnt_cnt - 1) {
					noCli = 1; // 그런 사용자가 없을 때 표시
					break;
				}
			}

			if(noCli == 1) {
				write(clnt_sock, "[NoClient_sorry]", BUF_SIZE);
				pthread_mutex_unlock(&mutx);
				return 1;
			}
			else if(noCli == 0) {	// 해당 사용자가 존재하는지 찾기
				write(clnt_sock, "[continue_ok_nowgo]", BUF_SIZE);
			}

			write(clnt_socks[fileGo], "file : sr->cl", BUF_SIZE);	// 데이터를 보낸다는 신호를 먼저 보냄

			// 파일 크기 정보를 보냄.(진행중)
			read(clnt_sock, &fSize, sizeof(int));
			printf("File size %d Byte\n", fSize);
			write(clnt_socks[fileGo], &fSize, sizeof(int));

			while(1) {
				read(clnt_sock, file_msg, BUF_SIZE);
				if(!strcmp(file_msg, Fmsg_end))
					break;
				write(clnt_socks[fileGo], file_msg, BUF_SIZE);
			}

			write(clnt_socks[fileGo], "FileEnd_sr->cl", BUF_SIZE);
			pthread_mutex_unlock(&mutx);
			printf("(!Notice)File data transfered \n");

		case 1 :
			pthread_mutex_lock(&mutx);						

			for(i=0; i<clnt_cnt; i++) {
				if(clnt_sock == clnt_socks[i])
					continue;
				write(clnt_socks[i], "file : sr->cl", BUF_SIZE);
				// ㄴ 데이터를 보낸다는 신호를 먼저 보냄
			}

			read(clnt_sock, &fSize, sizeof(int));
			printf("File size %d Byte\n", fSize);

			for(i=0; i<clnt_cnt; i++) {		// 파일 크기 정보를 보냄.
				if(clnt_sock == clnt_socks[i])
					continue;
				write(clnt_socks[i], &fSize, sizeof(int));
			}

			while(1) {
				read(clnt_sock, file_msg, BUF_SIZE);
				if(!strcmp(file_msg, Fmsg_end)) {
					break;
				}
				for(i=0; i<clnt_cnt; i++) {
					if(clnt_sock == clnt_socks[i]) {
						continue;
					}
					write(clnt_socks[i], file_msg, BUF_SIZE);
				}
			}

			for(i=0; i<clnt_cnt; i++) {
				if(clnt_sock == clnt_socks[i]) {
					continue;
				}
				write(clnt_socks[i], "FileEnd_sr->cl", BUF_SIZE);
			}

			pthread_mutex_unlock(&mutx);
			printf("(!Notice)File data transfered \n");
	}

	return 0;
}