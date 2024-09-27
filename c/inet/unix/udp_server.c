#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/un.h>

#define  BUFF_SIZE   1024
#define  SOCK_LOCALFILE   "/tmp/process_a"

int   main( void)
{
   int    sd, rv;
   size_t addr_size;
   struct sockaddr_un   local_addr;
   struct sockaddr_un   guest_addr;
   char   buff_rcv[BUFF_SIZE+5];
   char   buff_snd[BUFF_SIZE+5];

   rv = access( SOCK_LOCALFILE, F_OK );
   if ( rv == 0 )
      unlink( SOCK_LOCALFILE);

   sd = socket ( PF_FILE, SOCK_DGRAM, 0 );
   if ( sd < 0 )
   {
      printf( "socket 생성 실패\n");
      exit( 1);
   }

   memset ( &local_addr, 0, sizeof local_addr );
   local_addr.sun_family = AF_UNIX;
   strcpy( local_addr.sun_path, SOCK_LOCALFILE);

   rv = bind ( sd, (struct sockaddr*)&local_addr, sizeof( local_addr) );
   if ( rv < 0 )
   {
      printf( "bind() 실행 에러\n");
      exit( 1);
   }

   while ( 1 )
   {
      addr_size = sizeof ( guest_addr );
      recvfrom ( sd, buff_rcv, BUFF_SIZE, 0 ,
                     ( struct sockaddr*)&guest_addr, &addr_size );
      printf( "receive: %s\n", buff_rcv);
      printf( "%s\n", guest_addr.sun_path);     // 게스트 프로세스의 소켓용 파일 이름을 출력해 봅니다.

      sprintf( buff_snd, "%s%s", buff_rcv, buff_rcv);
      printf( "send: %s\n", buff_snd);

      sendto ( sd, buff_snd, strlen( buff_snd)+1, 0,
                     ( struct sockaddr*)&guest_addr, sizeof guest_addr );// +1: NULL까지 포함해서 전송
   }
}

