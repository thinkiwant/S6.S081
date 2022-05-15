#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char* fmtname(char * path){
	static char buf[DIRSIZ+1];
	char* p;

	for(p=path+strlen(path); p>=path && *p!='/'; p--);
	p++;
	if(strlen(p) >= DIRSIZ)
		return p;
	memmove(buf, p, strlen(p));
	memset(buf+strlen(p), '\0', DIRSIZ-strlen(p));
	return buf;
}

void
find(char* path, char* fileName){
	char buf[512], *p;
	int fd;
	struct dirent de;
	struct stat st;

	if((fd = open(path,0)) < 0){
		fprintf(2, "find: open %s failed.\n", path);
		return;
	}
	//printf("open %s successfully\n", path);
	
	if(fstat(fd, &st) < 0){
		fprintf(2, "find: get stat %s failed\n", path);
		return;
	}

	if(st.type != T_DIR){
		fprintf(2, "find: %s is not a directory\n", path);
		close(fd);	
		return;
	}
	
	if(strlen(path) + 1 + DIRSIZ + 1> sizeof(buf)){
		fprintf(2, "find: path too long\n");
		close(fd);
		return;
	}

	strcpy(buf, path);
	p = buf+strlen(buf);
	*p++ = '/';
	while(read(fd, &de, sizeof(de)) == sizeof(de)){
		if(de.inum == 0)	// what is inum?
			continue;
		memmove(p, de.name, DIRSIZ);
		p[DIRSIZ] = 0;
		if(stat(buf, &st) < 0){
			printf("find: cannot stat %s\n", buf);
			continue;
		}
		if(!strcmp(fmtname(p), fileName)){
			printf("%s\n", buf);
		}
		//printf("print current fn: \"%s\", target fn: \"%s\"\n",fmtname(p), fileName);

		if(st.type == T_DIR&& strcmp(fmtname(p),".") && strcmp(fmtname(p), "..")){
			find(buf, fileName);
		}
	}
	close(fd);
}

int
main(int argc, char** argv){
	if(argc < 3){
		fprintf(2, "usage: %s path fileName\n", argv[0]);
		exit(1);
	}
	find(argv[1], argv[2]);
	exit(1);
}


		
