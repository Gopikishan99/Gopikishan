#include <sys/types.h>
#include <sys/socket.h>
#include &lt;stdio.h&gt;
#include &lt;string.h&gt;
#include &lt;unistd.h&gt;
#include &lt;stdlib.h&gt;
#include &lt;arpa/inet.h&gt;

#define BUFFERSIZE 40 // must be greater than 26

int main(int argc, char *argv[])
{
int ser_sd,n;
ssize_t i,j;
char buffer[BUFFERSIZE];
struct sockaddr_in server_addr;

if( argc != 3 )
{
printf( &quot;Usage: %s server-address server-port\n&quot;, argv[0] );
exit(1);
}

ser_sd = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );
if( ser_sd == -1 ) { perror(&quot;socket-call&quot;); exit( 1 ); }

server_addr.sin_family = AF_INET;
n = inet_aton( argv[1], &amp;(server_addr.sin_addr) );
if( n == 0 ) { printf(&quot;Invalid address\n&quot;); exit(1); }
server_addr.sin_port = htons( atoi(argv[2]) );

n = connect( ser_sd, (struct sockaddr *) &amp;server_addr,
sizeof(server_addr) );
if( n == -1 ) { perror(&quot;connect-call&quot;); exit(1); }

// nothing needs to be written on server&#39;s socket.

memset( buffer, &#39;\0&#39;, BUFFERSIZE );
i = read( ser_sd, buffer, BUFFERSIZE );
if( i == -1 ) { perror(&quot;read&quot;); exit(1); }
printf(&quot;bytes read from server&#39;s socket=%u\n&quot;, i );

write( STDOUT_FILENO,&quot;Reply from server-&gt;&quot;, 19 );
j = write( STDOUT_FILENO, buffer, i );
if( j == -1 ) { perror(&quot;write&quot;); exit(1); }

shutdown(ser_sd, SHUT_RDWR);
close(ser_sd);
return 0;
}
