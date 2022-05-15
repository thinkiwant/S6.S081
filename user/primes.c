#include "kernel/types.h"
#include "user/user.h"
#define UPPER_LIMIT 34

int
main(int argc, char **argv){
	if(argc != 1){
		fprintf(2, "No other argument except the %s command is needed.\n", argv[0]);
		exit(1);
	}
	
	int p[2];
	int nums[UPPER_LIMIT];
	pipe(p);
	for(int i =0; i<UPPER_LIMIT-1; i++)
		nums[i] = i+2;
	write(p[1], (char*) nums, UPPER_LIMIT * sizeof(int));
	//fprintf(1, "array initiated\n");
	while(1){
		//printf("new iritation\n");
		if(fork()==0){
			//fprintf(1, "child %d created\n", getpid());
			int buff[UPPER_LIMIT];
			int count = read(p[0], (char*) buff, sizeof(buff));
			count/=sizeof(int);
			//fprintf(1, "%d inputs\n", count);
			int curNum = buff[0];
			printf("prime %d\n", curNum);
			int needNewChild = 0;
			for(int i =1;i<count;i++)
				if(buff[i]%curNum){
					needNewChild = 1;
					write(p[1], (char*) (buff+i), sizeof(int));
					//fprintf(1, "%d %d put\n", getpid(), buff[i]);
				}
			close(p[0]);
			close(p[1]);
			if(needNewChild)
				exit(0);
			else
				exit(1);
		}
		else{
			int cst=0;
			//printf("start waitting\n");
			wait(&cst);
			if(cst)
				break;
		}
		//printf("end waitting\n");
	}
	close(p[0]);
	close(p[1]);
	exit(0);
}
