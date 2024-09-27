#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>


#define LOCALFILE	"/tmp/sum_client_uds"
struct data
{
    int a;
    int b;
    int sum;
};
int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_un serveraddr, cliaddr;
    int  addrlen;
    struct data mydata;
    int rv;

    if (argc != 4)
    {
        printf("Usage : ./a.out FLIE_NAME val1 val2\n");
        exit(0);
    }

    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0); 
    if (sockfd < 0)
    {
        perror("exit : ");
        exit(0);
    }

    unlink ( LOCALFILE );

    bzero(&cliaddr, sizeof(cliaddr));
    cliaddr.sun_family = AF_UNIX;
    strcpy(cliaddr.sun_path, LOCALFILE);

    rv = bind(sockfd, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
    if ( rv < 0 )
    {
        perror("bind error : ");
        exit(0);
    }

    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sun_family = AF_UNIX;
    strcpy(serveraddr.sun_path, argv[1]);
    addrlen = sizeof(serveraddr);

    mydata.a = atoi(argv[2]);
    mydata.b = atoi(argv[3]);
    mydata.sum =0;

    rv = sendto(sockfd, (void *)&mydata, sizeof(mydata), 0, 
                (struct sockaddr *)&serveraddr, addrlen);
    if ( rv < 0 )
    {
        perror("send error : ");
        exit(0);
    }

    addrlen = sizeof(serveraddr);

    rv = recvfrom(sockfd, (void *)&mydata, sizeof(mydata), 0,
                (struct sockaddr *)&serveraddr, &addrlen);
    if ( rv < 0 )
    {
        perror("recv error : ");
        exit(0);
    }

    printf("result is : %d\n", mydata.sum);

    close(sockfd);
    exit(0);
}
