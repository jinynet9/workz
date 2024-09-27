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
    int sockfd;
    int clilen;
    struct data mydata;
    struct sockaddr_un clientaddr, serveraddr;
    int rv;

    if (argc != 2)
    {
        printf("Usage : ./a.out FILE_NAME\n");
        exit(0);
    }

    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0); 
    if (sockfd < 0)
    {
        perror("socket error : ");
        exit(0);
    }    
    unlink(argv[1]);

    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sun_family = AF_UNIX;
    strcpy(serveraddr.sun_path, argv[1]);

    rv = bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if ( rv < 0 )
    {
        perror("bind error : ");
        exit(0);
    }


    while(1)
    {
	clilen  = sizeof(clientaddr); 
        recvfrom(sockfd, (void *)&mydata, sizeof(mydata), 0, (struct sockaddr *)&clientaddr, &clilen); 
	printf("client.sun_path = %s\n", clientaddr.sun_path);

	mydata.sum = mydata.a + mydata.b;    
        printf("%d + %d = %d\n", mydata.a, mydata.b, mydata.sum);    
        sendto(sockfd, (void *)&mydata, sizeof(mydata), 0, (struct sockaddr *)&clientaddr, clilen);
    }
    close(sockfd);
    exit(0);
}
