#include&lt;sys/types.h&gt;
#include&lt;sys/socket.h&gt;
#include&lt;netinet/in.h&gt;
#include&lt;unistd.h&gt;
#include&lt;netdb.h&gt;
#include&lt;stdio.h&gt;
#include&lt;string.h&gt;
#include&lt;arpa/inet.h&gt;
#define MAXLINE 1024
int main(int argc,char **argv)
{
int sockfd;
int n;

socklen_t len;
char msg[1024];
struct sockaddr_in servaddr,cliaddr;
sockfd=socket(AF_INET,SOCK_DGRAM,0);
bzero(&amp;servaddr,sizeof(servaddr));
servaddr.sin_family=AF_INET;
servaddr.sin_addr.s_addr=INADDR_ANY;
servaddr.sin_port=htons(5035);
printf(&quot;\n\n Binded&quot;);
bind(sockfd,(struct sockaddr*)&amp;servaddr,sizeof(servaddr));
printf(&quot;\n\n Listening...&quot;);
for(;;)
{
printf(&quot;\n &quot;);
len=sizeof(cliaddr);
n=recvfrom(sockfd,msg,MAXLINE,0,(struct sockaddr*)&amp;cliaddr,&amp;len);
printf(&quot;\n Client&#39;s Message : %s\n&quot;,msg);
if(n&lt;6)
perror(&quot;send error&quot;);
sendto(sockfd,msg,n,0,(struct sockaddr*)&amp;cliaddr,len);
}
return 0;
}
