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
    struct sockaddr_un serveraddr;
    int  addrlen;
    struct data mydata;
    int rv;

    if (argc != 4)
    {
        printf("Usage : ./a.out FLIE_NAME val1 val2\n");
        exit(0);
    }

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0); 
    if (sockfd < 0)
    {
        perror("exit : ");
        exit(0);
    }

    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sun_family = AF_UNIX;
    strcpy(serveraddr.sun_path, argv[1]);
    addrlen = sizeof(serveraddr);

    rv = connect(sockfd, (struct sockaddr *)&serveraddr, addrlen);
    if ( rv < 0 )
    {
	perror("connect error: ");
	exit(0);
    }

    mydata.a = atoi(argv[2]);
    mydata.b = atoi(argv[3]);
    mydata.sum =0;

    rv = write(sockfd, (void *)&mydata, sizeof(mydata));
    if ( rv < 0 )
    {
        perror("send error : ");
        exit(0);
    }

    rv = read(sockfd, (void *)&mydata, sizeof(mydata));
    if ( rv < 0 )
    {
        perror("recv error : ");
        exit(0);
    }

    printf("result is : %d\n", mydata.sum);

    close(sockfd);
    exit(0);
}
