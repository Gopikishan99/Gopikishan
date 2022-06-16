#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
int main(int argc , char *argv[])
{
int socket_desc , client_sock , c , read_size;//,i,len;
struct sockaddr_in server , client;
char client_message[2000];//,temp;
//Create socket
socket_desc = socket(AF_INET,SOCK_STREAM,0);
if (socket_desc == -1)
{
printf("Could not create socket");
}
puts("Socket created");
//Prepare the sockaddr_in structure
server.sin_family = AF_INET;
server.sin_addr.s_addr = INADDR_ANY;
server.sin_port = htons( 8080 );
//Bind
if(bind(socket_desc,(struct sockaddr *)&server,sizeof(s>
{
//print the error message
perror("bind failed. Error");
return 1;
}
