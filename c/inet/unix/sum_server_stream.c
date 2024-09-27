#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

struct data
{
    int a;
    int b;
    int sum;
};

int main(int argc, char **argv)
{
    int sockfd, client_sockfd;
    int clilen;
    struct data mydata;
    struct sockaddr_un clientaddr, serveraddr;
    int state, pid;

    if (argc != 2)
    {
        printf("Usage : ./a.out FILE_NAME\n");
        exit(0);
    }

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0); 
    if (sockfd < 0)
    {
        perror("socket error : ");
        exit(0);
    }    
    unlink(argv[1]);

    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sun_family = AF_UNIX;
    strcpy(serveraddr.sun_path, argv[1]);

    state = bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if ( state < 0 )
    {
        perror("bind error : ");
        exit(0);
    }

    state = listen(sockfd, 5);
    if ( state < 0 )
    {
        perror("listen error : ");
        exit(0);
    }

    while (1)
    {
	printf("accepting...\n");

	clilen  = sizeof(clientaddr); 
	client_sockfd = accept(sockfd, (struct sockaddr *)&clientaddr, &clilen);

	printf("accepted, client.sun_path: %s\n", clientaddr.sun_path);

	pid = fork();
	if ( pid == 0 )
	{
	    read(client_sockfd, (void *)&mydata, sizeof(mydata)); 
	    mydata.sum = mydata.a + mydata.b;    
	    printf("%d + %d = %d\n", mydata.a, mydata.b, mydata.sum);    
	    write(client_sockfd, (void *)&mydata, sizeof(mydata));
	    exit(0);
	}
    }

    close(sockfd);
    exit(0);
}
