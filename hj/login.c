#include "headerFiles.h"  User login() { 	User user, loginUser; 	FILE *fp = fopen("userInfomation.txt", "r");

	printf("ID 입력 : ");
	fgets(loginUser.userID, sizeof(loginUser.userID), stdin);
	printf("PW 입력 : ");
	scanf("%d", &loginUser.userPW);

	while (feof(fp) == 0) {
		fread(&user, sizeof(user), 1, fp); 		if(strcmp(user.userID, loginUser.userID) == 0) { 			if(user.userPW == loginUser.userPW) { 				loginUser = user; 				return 0; 			} 		} 	} 	fclose(fp); 	 	return -1; }