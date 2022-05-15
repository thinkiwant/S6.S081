#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

int
main(int argc, char ** argv) {
	if(argc<2){
		fprintf(2, "Usage: %s cmdline [arg...]\n", argv[0]);
		exit(1);
	}

	char *buf[MAXARG];
	memset(buf, 0, sizeof(buf));
	for(int i=0;i<argc;i++)
		buf[i] = argv[i+1]; // omit the 0th argument "xargs"
	char argNew[512];
	memset(argNew, '\0', sizeof(argNew));
	if(read(0, argNew, sizeof(argNew)) <= 0){
		fprintf(2, "PID %d: read other arguments failed\n", getpid());
		exit(1);
	}

	char* p=argNew;
	for(int offset=0; (p+offset)<argNew+sizeof(argNew)&&*(p+offset)!='\0'; offset++){
		if(*(p+offset)==' ')	continue;
		int i_newarg = 0;
		char* ap = p+offset;
		while(*ap!='\n'&& *ap!='\0'){	// collect argument for one cmdline
			buf[argc-1+i_newarg] = ap;
			i_newarg++;
			while(*ap!=' '&&*ap!='\n')	ap++;	// skim rest of the argument
			if(*ap=='\n'){
				*ap = '\0';
				break;
			}
			*ap = '\0';
			while(*++ap==' '); 	//skim spaces
		}
		if(fork()==0){
			/*
			for(int i=0; buf[i]!=0&& i < sizeof(buf); i++){
				printf("buff%d: \"%s\", ", i, buf[i]);
			}
			printf("\n");
			*/
			exec(argv[1], buf);
			fprintf(2, "child exec failed\n");
			exit(1);
		}
	memset(buf+argc-1, 0, i_newarg);
	p=ap+1;
	offset = 0;
	wait(0);
	}
	//printf("xargs ended\n");
	exit(0);
}
