#include&lt;sys/types.h&gt;
#include&lt;sys/socket.h&gt;
#include&lt;netinet/in.h&gt;
#include&lt;string.h&gt;
#include&lt;arpa/inet.h&gt;
#include&lt;string.h&gt;
#include&lt;arpa/inet.h&gt;
#include&lt;stdio.h&gt;
#define MAXLINE 1024
int main(int argc,char* argv[])
{
int sockfd;
int n;

socklen_t len;
char sendline[1024],recvline[1024];
struct sockaddr_in servaddr;
strcpy(sendline,&quot;&quot;);
printf(&quot;\n Enter the message : &quot;);
scanf(&quot;%s&quot;,sendline);
sockfd=socket(AF_INET,SOCK_DGRAM,0);
bzero(&amp;servaddr,sizeof(servaddr));
servaddr.sin_family=AF_INET;
servaddr.sin_addr.s_addr=inet_addr(&quot;127.0.0.1&quot;);
servaddr.sin_port=htons(5035);
connect(sockfd,(struct sockaddr*)&amp;servaddr,sizeof(servaddr));
len=sizeof(servaddr);
sendto(sockfd,sendline,MAXLINE,0,(struct sockaddr*)&amp;servaddr,len);
n=recvfrom(sockfd,recvline,MAXLINE,0,NULL,NULL);
recvline[n]=0;
printf(&quot;\n Server&#39;s Echo : %s\n\n&quot;,recvline);
return 0;
}
