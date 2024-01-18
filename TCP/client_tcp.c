#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>

void main(int argc, char *argv[])
{
    if(argc<2)//If IP address is not provided
    {
        printf("Server IP address not provided.\n");
        exit(1);
    }
    int sockfd,portno=3000 ,n;
    char buffer[255];
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    struct hostent  *server;

    sockfd = socket(AF_INET,SOCK_STREAM,0);//Create a socket with IPv4 and TCP connection
    if (sockfd<0)//Socket Creation Failed
    {
        perror("Socket Error");
        exit(1);     
    }
    server =  gethostbyname(argv[1]);//Convert the IP address of server passed into a hostent object
    if(server==NULL)//If conversion failed
    {
        fprintf(stderr,"Host error\n");
        exit(1);
         
    }
    bzero((char *)&server_addr,sizeof(server_addr));//Clear the server address struct object
    server_addr.sin_family = AF_INET;//Set the sin family to IPv4
    bcopy((char *)server->h_addr,(char*)&server_addr.sin_addr.s_addr,server->h_length); //Copy the server IP address from the hostent object to the server address object.

    server_addr.sin_port=htons(portno);//Set the destination port

    if(connect(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr))<0)//Connect the socket to the destination IP address using TCP
    {
        perror("Connection Error");
        exit(1);
    }
    printf("TCP Connection Established with the Server...\nType '###' to close the connection\n\n");
    while(1)
    {
        bzero(buffer,sizeof(buffer));
        fprintf(stdout,">");
        fgets(buffer,sizeof(buffer),stdin);//Accept message from the keyboard
        n = write(sockfd,buffer,strlen(buffer));//Write the accepted message into the socket
        if(n<0)//Write failed
        {
            perror("error writing client");
            exit(1);
        }
        if(strncmp("###",buffer,3)==0)
        {
            printf("Connection Terminated by You\n");
            break;
        }
        bzero(buffer,sizeof(buffer));
        n= read(sockfd,buffer,sizeof(buffer));//Read message from the socket
        if(n<0)
        {
            perror("error Reading client");
            exit(1);
        }
        fprintf(stdout,"Server: %s\n",buffer);
        
        if(strncmp("###",buffer,3)==0)
        {
            printf("Connection Terminated by Server\n");
            break;
        }
    }
    close(sockfd); //Close the socket
}