#include "headerFiles.h"

int signUp() { 	User user, newUser; 	FILE *fp = fopen("userInfomation.txt", "w");

	printf("ID 입력 : ");
	fgets(newUser.userID, sizeof(newUser.userID), stdin);

	if(strcmp(user.userID, newUser.userID) == 0) {
		return -1;
	}
	else {
		printf("이름 입력 : ");
		fgets(newUser.userName, sizeof(newUser.userName), stdin);
		printf("PW 입력 : ");
		scanf("%d", &newUser.userPW);
		userCnt++;
		newUser.userCnt = userCnt;

		fwrite(&newUser, sizeof(newUser), 1, fp);
	}
	userList[userCnt] = newUser; 	fclose(fp); 	return 0; }