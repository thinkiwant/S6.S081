#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char ** argv) {

		if(argc != 2){
				fprintf(2, "command usage : %s clicks_to_sleep\n", argv[0]);
				exit(1);
		}
			
		int n = 0;
		n = atoi(argv[1]);
		if(n<=0){
			
			fprintf(2, "invalid argument: %s\n", argv[1]);
			exit(1);
		}
		sleep(n);
		fprintf(1, "sleep over\n");
		exit(0);
}
