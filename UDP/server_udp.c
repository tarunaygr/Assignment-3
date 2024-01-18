#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

void main()
{
    int port =3000,sockfd;
    struct sockaddr_in server_addr,client_addr;
    char buffer[255];
    socklen_t addr_len;

    sockfd=socket(AF_INET,SOCK_DGRAM,0);//Create a IPv4 UDP socket

    if (sockfd<0)//If socket creation failed
    {
        perror("Socket Error");
        exit(1);     
    }
    bzero(&server_addr,sizeof(server_addr));//Clear the server address object
    server_addr.sin_family=AF_INET;//Set the sin family as IPv4
    server_addr.sin_port=htons(port);//Set the destination port
    server_addr.sin_addr.s_addr=INADDR_ANY;//Accept connections from any IP address

    bind(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr));//Bind the socket to the server address
    addr_len=sizeof(client_addr);
    printf("UDP Server Established\nType '###' to close the connection\n\n");
    while(1){
        recvfrom(sockfd,buffer,sizeof(buffer),MSG_WAITALL,(struct sockaddr*)&client_addr,&addr_len);//Recieve UDP packets from the socket from the IP address in client_addr.
        printf ("Client: %s",buffer);
        if(strncmp("###",buffer,3)==0)
        {
            printf("connection Terminated by Client\n");
            break;
        }
        bzero(buffer,sizeof(buffer));
        printf("> ");
        fgets(buffer,sizeof(buffer),stdin);
        sendto(sockfd,buffer,sizeof(buffer),0,(struct sockaddr*)&client_addr,sizeof(client_addr));//Send the message from the server to the client.
        if(strncmp("###",buffer,3)==0)
        {
            printf("Connection Terminated by You\n");
            break;
        }
        bzero(buffer,sizeof(buffer)); 
    }
    close(sockfd);//Close the socket
}