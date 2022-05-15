#include "kernel/types.h"
#include "user/user.h"
#define BUFFSIZE 100

int main(int argc, char ** argv) {
	int p1[2], p2[2];
	char buff1[BUFFSIZE];
	char buff2[BUFFSIZE];
	memset(buff1, 0, BUFFSIZE);
	memset(buff2, 0, BUFFSIZE);
	pipe(p1);
	pipe(p2);
	int fdpr = dup(p1[0]);
	int fdpw = dup(p2[1]);
	int fdcr = dup(p2[0]);
	int fdcw = dup(p1[1]);
	close(p1[0]);
	close(p1[1]);
	close(p2[0]);
	close(p2[1]);
	if(fork()==0){
		close(fdpw);
		close(fdpr);
		if(read(fdcr, buff1, BUFFSIZE)<=0){
			fprintf(2, "child %n read failed\n", getpid());
			exit(1);
		}
		fprintf(1, "%d: received %s\n", getpid(), buff1);
		write(fdcw, "pong", 5);
		close(fdcw);
		exit(0);
	}
	close(fdcw);
	close(fdcr);
	write(fdpw, "ping", 5);
	close(fdpw);
	wait(0);
	if(read(fdpr, buff2, BUFFSIZE)<=0){
		fprintf(2, "parent %n read failed\n", getpid());
		exit(1);
	}
	fprintf(1, "%d: received %s\n", getpid(), buff2);
	close(fdpr);
	exit(0);
}

