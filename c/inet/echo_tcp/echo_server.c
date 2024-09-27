#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>

#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[])
{
    int ld;	/* listen */
    int cd;	/* connect */
    char message[BUF_SIZE];
    int str_len, i;

    struct sockaddr_in serv_adr;
    struct sockaddr_in clnt_adr;
    socklen_t clnt_adr_sz;

    if(argc!=2) {
	printf("Usage : %s <port>\n", argv[0]);
	exit(1);
    }

    ld=socket(PF_INET, SOCK_STREAM, 0);   
    if(ld==-1)
	error_handling("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_adr.sin_port=htons(atoi(argv[1]));

    if(bind(ld, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
	error_handling("bind() error");

    if(listen(ld, 5)==-1)
	error_handling("listen() error");

    clnt_adr_sz=sizeof(clnt_adr);

    for(i=0; i<5; i++)
    {
	cd=accept(ld, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
	if(cd==-1)
	    error_handling("accept() error");
	else
	    printf("Connected client %d \n", i+1);

	while((str_len=read(cd, message, BUF_SIZE))!=0)
	    write(cd, message, str_len);

	close(cd);
    }

    close(ld);

    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
