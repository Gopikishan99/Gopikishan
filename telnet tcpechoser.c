#include&lt;stdio.h&gt;
#include&lt;netinet/in.h&gt;
#include&lt;netdb.h&gt;
#define SERV_TCP_PORT 5035
int main(int argc,char**argv)
{
       int sockfd,newsockfd,clength;
       struct sockaddr_in serv_addr,cli_addr;
       char buffer[4096];
       sockfd=socket(AF_INET,SOCK_STREAM,0);
       serv_addr.sin_family=AF_INET;
       serv_addr.sin_addr.s_addr=INADDR_ANY;
       serv_addr.sin_port=htons(SERV_TCP_PORT);
       printf(&quot;\nStart&quot;);
       bind(sockfd,(struct sockaddr*)&amp;serv_addr,sizeof(serv_addr));
       printf(&quot;\nListening...&quot;);
       printf(&quot;\n&quot;);
       listen(sockfd,5);
       clength=sizeof(cli_addr);
       newsockfd=accept(sockfd,(struct sockaddr*)&amp;cli_addr,&amp;clength);
       printf(&quot;\nAccepted&quot;);
       printf(&quot;\n&quot;);
       read(newsockfd,buffer,4096);
       printf(&quot;\nClient message:%s&quot;,buffer);
       write(newsockfd,buffer,4096);
       printf(&quot;\n&quot;);       return 0;
}
