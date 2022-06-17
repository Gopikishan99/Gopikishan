#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <wait.h>

#define  PORT           50001
#define  BUFFERSIZE     50     // must be greater than 26

void child_handler( int signumber );

int main (int argc, char *argv[])
{
  int                   ser_sd, temp_sd, n ;
  struct  sockaddr_in   server_addr; 
  pid_t                 pid;
  time_t                now;
  char                  buffer[BUFFERSIZE];
  ssize_t               i;
  struct  sigaction     act;

  // SIGCHLD handler is used to reap zombie
  act.sa_handler = child_handler;
  act.sa_flags = SA_RESTART;
  sigaction( SIGCHLD, &act, NULL );


  if( argc != 1 )
  {
    printf("usage: %s \n", argv[0] );
    exit(1);
  }
  // IPv4 TCP socket is created.
  ser_sd = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );
  if( ser_sd == -1 ) { perror("socket-call"); exit(1); }

  // members of server address are initialised
  server_addr.sin_family = AF_INET; 
  server_addr.sin_addr.s_addr = htonl( INADDR_ANY );
  server_addr.sin_port = htons( PORT );  

  // socket is bound to [server_addr]
  n = bind( ser_sd, (struct sockaddr *) &server_addr,
            sizeof(server_addr) );
  if( n == -1 ) { perror("bind-call"); exit(1); }

  n = listen( ser_sd, 5 );
  if( n == -1 ) { perror("listen-call"); exit(1); }

  while (1)
  {
    printf("%s: daytime server listening on TCP port %u\n",
           argv[0], PORT );

    temp_sd = accept( ser_sd, NULL, 0 );
    if( temp_sd == -1 ) perror("accept-call");

    pid = fork();
    if( pid == -1 ) {perror("fork-call"); exit(1); } 

    if( pid == 0 ) // child's block
    {
       memset( buffer, '\0', BUFFERSIZE );
       time( &now );
strcpy( buffer, ctime( &now ) );
       // now [buffer] contains time in human readable form
       i = write( temp_sd, buffer, strlen(buffer) );
       if( i == -1 ) { perror("socket-write"); exit(1); }
       shutdown( temp_sd, SHUT_RDWR );
       close( temp_sd );
 return 0; 
    } // child's block

    // parent's block
    // useless descriptor is closed
    close( temp_sd );

  } // endless-loop block

} // end of main

// signal handler
void child_handler( int signumber )
{
  pid_t  p;

  printf("signal-handler: received %u \n", signumber );
  p = waitpid( -1, NULL, WNOHANG );
  if( p == -1 )  perror("waitpid");

  return;
}
