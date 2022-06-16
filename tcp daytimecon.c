!= 1 )
{
printf(&quot;usage: %s \n&quot;, argv[0] );
exit(1);
}
// IPv4 TCP socket is created.
ser_sd = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );
if( ser_sd == -1 ) { perror(&quot;socket-call&quot;); exit(1); }

// members of server address are initialised
server_addr.sin_family = AF_INET;
server_addr.sin_addr.s_addr = htonl( INADDR_ANY );
server_addr.sin_port = htons( PORT );

// socket is bound to [server_addr]
n = bind( ser_sd, (struct sockaddr *) &amp;server_addr,
sizeof(server_addr) );
if( n == -1 ) { perror(&quot;bind-call&quot;); exit(1); }

n = listen( ser_sd, 5 );
if( n == -1 ) { perror(&quot;listen-call&quot;); exit(1); }

while (1)
{
printf(&quot;%s: daytime server listening on TCP port %u\n&quot;,
argv[0], PORT );

temp_sd = accept( ser_sd, NULL, 0 );
if( temp_sd == -1 ) perror(&quot;accept-call&quot;);

pid = fork();
if( pid == -1 ) {perror(&quot;fork-call&quot;); exit(1); }

if( pid == 0 ) // child&#39;s block
{
memset( buffer, &#39;\0&#39;, BUFFERSIZE );
time( &amp;now );
strcpy( buffer, ctime( &amp;now ) );
// now [buffer] contains time in human readable form
i = write( temp_sd, buffer, strlen(buffer) );
if( i == -1 ) { perror(&quot;socket-write&quot;); exit(1); }
shutdown( temp_sd, SHUT_RDWR );
close( temp_sd );
return 0;
} // child&#39;s block

// parent&#39;s block
// useless descriptor is closed
close( temp_sd );

} // endless-loop block

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
