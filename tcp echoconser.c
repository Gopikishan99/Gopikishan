#include &lt;stdio.h&gt;
#include &lt;unistd.h&gt;
#include &lt;stdlib.h&gt;
#include &lt;signal.h&gt;
#include &lt;wait.h&gt;
#include &lt;errno.h&gt;
#include &lt;string.h&gt;
#include &lt;sys/types.h&gt;
#include &lt;sys/socket.h&gt;
#include &lt;netinet/in.h&gt;

#include &lt;arpa/inet.h&gt;

#define SERVER_PORT 65535

#define BUFFERSIZE 1024

void pipe_handler( int signumber );
void child_handler( int signumber );

extern int errno;

int main( int argc, char *argv[] )
{
int ser_sd;
int tempsockfd, n ;
ssize_t i, j ;
char buffer[BUFFERSIZE];
struct sockaddr_in server_addr;
struct sigaction act;

act.sa_handler = child_handler;
act.sa_flags = SA_RESTART;
sigaction( SIGCHLD, &amp;act, NULL );

ser_sd = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );
if( ser_sd == -1 ) { perror(&quot;socket-call&quot;); exit(1); }

server_addr.sin_family = AF_INET;
server_addr.sin_addr.s_addr = htonl( INADDR_ANY );
server_addr.sin_port = htons(SERVER_PORT);

n = bind( ser_sd, (struct sockaddr *) &amp; server_addr,
sizeof(server_addr) );
if( n == -1) { perror(&quot;bind-call&quot;); exit(1); }

n = listen( ser_sd, 1 );
if( n == -1 ) { perror(&quot;listen-call&quot;); exit(1); }

while ( 1 ) // endless loop
{
pid_t pid;

printf( &quot;\n %s : waiting for client&#39;s request on port %u \n&quot;,
argv[0], SERVER_PORT );

tempsockfd = accept( ser_sd, NULL, 0 );
if( tempsockfd == -1 ) perror(&quot;accept-call&quot;);

// server( main ) forks a child to handle a client&#39;s request
pid = fork();
if ( pid == -1 ) { perror(&quot;fork-call&quot;); exit(1); }

if ( pid == 0 ) // child process handles a client&#39;s request

{
// clear buffer before reading into it
memset( buffer, &#39;\0&#39;, BUFFERSIZE );
i = read( tempsockfd, buffer, BUFFERSIZE );
if( i == -1 ) { perror(&quot;tempsockfd-read&quot;); exit(1); }

j = write( STDOUT_FILENO,&quot;Received from client-&gt;&quot;, 22 );
if( j == -1 ) { perror(&quot;screen-write&quot;); exit(1); }

printf(&quot;%s \n&quot;, buffer );

j = write( tempsockfd, buffer, i );
if( j == -1 ) { perror(&quot;socket-write&quot;); exit(1); }

n = shutdown(tempsockfd,SHUT_RDWR);
if ( n == -1 ) { perror(&quot;shutdown&quot;); exit(1); }

n = close(tempsockfd);
if( n == -1 ) { perror(&quot;close-tempsockfd&quot;); exit(1); }

exit(0);
} // end of child&#39;s block

// rest is parent&#39;s block. Main closes useless socket
close(tempsockfd);

} // end of endless-loop block

} // end of main

// signal handler
void child_handler( int signumber )
{
pid_t p;

printf(&quot;signal-handler: received %u \n&quot;, signumber );
p = waitpid( -1, NULL, WNOHANG );
if( p == -1 ) perror(&quot;waitpid&quot;);

return;
}
