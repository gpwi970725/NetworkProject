#include "headerFiles.h"

#define BUF_SIZE 128
#define MAX_CLNT 2
#define NAME_SIZE 20

void * handle_clnt(void * arg);
void send_msg(char * msg, int len);
void error_handling(char * msg);

int clnt_cnt=0;
int clnt_socks[MAX_CLNT];
char clnt_name[NAME_SIZE]= {NULL};
char clnt_names[MAX_CLNT][NAME_SIZE]= {NULL};


pthread_mutex_t mutx;

int main(int argc, char *argv[]) {
	int serv_sock, clnt_sock, i;
	struct sockaddr_in serv_adr, clnt_adr;

	char key[4], id[20],pw[20],testpw[3]="hi" ,testid[4]="asd", clog[1];
	int clnt_adr_sz;
	pthread_t t_id;

	if(argc!=2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	pthread_mutex_init(&mutx, NULL);
	serv_sock=socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET; 
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error");
	if(listen(serv_sock, 5)==-1)
		error_handling("listen() error");

	while(1) {
		clnt_adr_sz=sizeof(clnt_adr);
		clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr,&clnt_adr_sz);
		
		while(1){
			id[0]='\0';
			pw[0]='\0';
			read(clnt_sock, id, sizeof(id));
			id[strlen(id)] = '\0';
			read(clnt_sock, pw, sizeof(pw));
			pw[strlen(pw)] = '\0';

			printf("%s %s\n",id, testid);
			printf("%s %s\n",pw, testpw);
			int ret = strcmp(id , testid);
			int ret2 = strcmp(pw , testpw);

			if(ret == 0&&ret2 == 0){
				printf("login success\n");
				clog[0] = 't';
				clog[1] = '\0';
			}else{
				printf("login fail\n");
				clog[0] = 'f';
				clog[1] = '\0';
			}
			write(clnt_sock, clog , strlen(clog));	
			
			if(clog[0]=='t'){
				break;
			}
		}

		if(clnt_cnt >= MAX_CLNT) {
			printf("CONNECT FAIL : %d \n", clnt_sock);
			write(clnt_sock, "too many users. sorry", BUF_SIZE);
			continue;
		}


		pthread_mutex_lock(&mutx);

		clnt_socks[clnt_cnt]=clnt_sock;
		read(clnt_sock, clnt_name, NAME_SIZE);		
		strcpy(clnt_names[clnt_cnt++], clnt_name);
		// ㄴ 클라이언트로부터 받은 접속자 이름입력
		pthread_mutex_unlock(&mutx);

		pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt_sock);
		pthread_detach(t_id);
		printf("Connected client IP: %s \n", inet_ntoa(clnt_adr.sin_addr));
	}
	close(serv_sock);
	return 0;
}

void * handle_clnt(void * arg) {
	int clnt_sock=*((int*)arg);
	int str_len=0, i;
	int fileReturn = 0;
	const char sig_file[BUF_SIZE] = {"file : cl->sr"};
	const char sig_file_all[BUF_SIZE] = {"file : cl->sr_all"};
	const char sig_whisper[BUF_SIZE] = {"whisper : cl->sr"};
	char msg[BUF_SIZE] = {NULL};

	while((str_len=read(clnt_sock, msg, BUF_SIZE))!=0) {
		if( !strcmp(msg, sig_file) ) {
			fileReturn = fileSend(0, clnt_sock, clnt_cnt, clnt_names, clnt_socks);
		}
		else if ( !strcmp(msg, sig_file_all) ) {
			fileReturn = fileSend(1, clnt_sock, clnt_cnt, clnt_names, clnt_socks);
		}
		if(fileReturn == 1) {
			continue;
		}

		if(!strcmp(msg, sig_whisper)) {
			int j=0;
			int noCli = 0;
			int mGo = 0;
			char tmpName[NAME_SIZE]= {NULL};
			char msg[NAME_SIZE]= {NULL};

			read(clnt_sock, tmpName, NAME_SIZE);

			pthread_mutex_lock(&mutx);
			for(j=0; j<clnt_cnt; j++) {
				if(!strcmp(tmpName, clnt_names[j]) ) {
					noCli = 0;
					mGo = j; // 보낼 소켓 번호를 저장
					break;
				}
				else if(j == clnt_cnt - 1) {
					noCli = 1; // 그런 사용자가 없을 때 표시
					break;
				}
			}
			pthread_mutex_unlock(&mutx);

			read(clnt_sock, msg, BUF_SIZE);

			if(noCli == 1) { // 귓속말할 사용자 없을 때
				write(clnt_sock, "sorry. no client like that", BUF_SIZE);
			} else { // 귓속말할 사용자가 존재할 때
				write(clnt_socks[j], msg, BUF_SIZE);
			}

		} else {
			printf("(!Notice)Chatting message transfered \n");
			send_msg(msg, str_len);
		}// ㄴ 메시지 전송
	}


	pthread_mutex_lock(&mutx);
	for(i=0; i<clnt_cnt; i++) {   // remove disconnected client
		if(clnt_sock==clnt_socks[i]) {
			while(i++<clnt_cnt-1) {
				clnt_socks[i]=clnt_socks[i+1];
				strcpy(clnt_names[i], clnt_names[i+1]);
			}
			break;
		}
	}
	clnt_cnt--;
	pthread_mutex_unlock(&mutx);
	close(clnt_sock);
	return NULL;
}

void send_msg(char * msg, int len) {   // send to all
	int i;
	pthread_mutex_lock(&mutx);
	for(i=0; i<clnt_cnt; i++)
		write(clnt_socks[i], msg, BUF_SIZE);
	pthread_mutex_unlock(&mutx);
}

void error_handling(char * msg) {
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

