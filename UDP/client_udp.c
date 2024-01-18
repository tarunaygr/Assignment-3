#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

void main(int argc,char* argv[])
{   
    if(argc<2)//If IP address is not provided
    {
        printf("Server IP address not provided.\n");
        exit(1);
    }
    int port =3000,sockfd;
    struct sockaddr_in server_addr;
    char buffer[255];
    socklen_t addr_len;

    sockfd=socket(PF_INET,SOCK_DGRAM,0);//Create the socket with IPv4 and UDP connection.
    bzero(&server_addr,sizeof(server_addr)); 

    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(port);
    server_addr.sin_addr.s_addr=inet_addr(argv[1]);//Accept the server IPv4 address in the terminal
    printf("UDP Client Established\nType '###' to close the connection\n\n");
    while(1){
        printf("> ");
        fgets(buffer,sizeof(buffer),stdin);
        sendto(sockfd,buffer,sizeof(buffer),0,(struct sockaddr*)&server_addr,sizeof(server_addr));//Send the UDP packet through the socket to the server_addr
        if(strncmp("###",buffer,3)==0)
        {
            printf("Connection Terminated by You\n");
            break;
        }
        int addr_size=sizeof(server_addr);
        recvfrom(sockfd,buffer,sizeof(buffer),MSG_WAITALL,(struct sockaddr*)&server_addr,&addr_size);//Recievce the UDP packets sent from the server
        printf("Server: %s\n",buffer);
        if(strncmp("###",buffer,3)==0)
        {
            printf("Connection Terminated by Server\n");
            break;
        }
    }
    close(sockfd);//Close the socket.
}