#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/un.h> 

#define  BUFF_SIZE   1024
#define  SOCK_LOCALFILE   "/tmp/process_b"
#define  SOCK_SERVERFILE  "/tmp/process_a"

int   main( int argc, char **argv)
{
   int    sd, rv;
   struct sockaddr_un   local_addr;
   struct sockaddr_un   server_addr;
   char   buff_rcv[BUFF_SIZE+5];

   if ( argc < 2 )
   {
       printf("a.out STRING\n");
       exit(1);
   }

   rv = access ( SOCK_LOCALFILE, F_OK );
   if ( rv == 0 )
      unlink( SOCK_LOCALFILE);

   sd = socket( PF_FILE, SOCK_DGRAM, 0);
   if ( sd < 0 )
   {
      printf( "socket 생성 실패\n");
      exit( 1);
   }

   memset ( &local_addr, 0, sizeof local_addr );
   local_addr.sun_family = AF_UNIX;
   strcpy ( local_addr.sun_path, SOCK_LOCALFILE );

   rv = bind ( sd, (struct sockaddr*)&local_addr, sizeof( local_addr) );
   if ( rv < 0 )
   {
      printf( "bind() 실행 에러\n");
      exit( 1);
   }

   memset ( &server_addr, 0, sizeof server_addr );
   server_addr.sun_family = AF_UNIX;
   strcpy ( server_addr.sun_path, SOCK_SERVERFILE );

   sendto ( sd, argv[1], strlen( argv[1])+1, 0, 
            ( struct sockaddr*)&server_addr, sizeof server_addr );          // +1: NULL까지 포함해서 전송

   recvfrom ( sd, buff_rcv, BUFF_SIZE, 0 , NULL, 0 );

   printf( "recvfrom: %s\n", buff_rcv);
   close ( sd );
   
   return 0;
}
