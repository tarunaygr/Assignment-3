#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

void main()
{
    int sockfd,newsockfd,portno,n;
    char buffer[255];
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;

    sockfd =socket(AF_INET,SOCK_STREAM,0);//Creating the Socket with IPv4 and TCP

    if (sockfd<0)//Socket Creation Failure
    {
        perror("Socket Error");
        exit(1);     
    }
    bzero(&server_addr,sizeof(server_addr));// Clear the server address variable
    portno =3000;

    server_addr.sin_family=AF_INET;//To indicate IPv4
    server_addr.sin_addr.s_addr=INADDR_ANY;// To indicate any IP can connect
    server_addr.sin_port=htons(portno);//Set the port number of the destination

    //Bind the socket to the destination IP and port number
    if(bind(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr))<0)
    {
        perror("Bind Failed");
        exit(1);
    }

    listen(sockfd,50);//listen on the socket and allow maximum of 50 connections at a time.
    client_len = sizeof(client_addr);

    newsockfd = accept(sockfd,(struct sockaddr *)&client_addr,&client_len);//Accept the first connection from the queue of requested connections.
    
    if(newsockfd<0)//Accept connection failed
    {
        perror("Error Accepting");
        exit(1);
    }

    fprintf(stdout,"TCP Connection Established with Client...\nType '###' to close the connection\n\nClient is typing...\n");
    while (1)
    {
        bzero(buffer,255);
        n = read(newsockfd,buffer,sizeof(buffer));//Read the message from the socket
        if(n<0)//Error reading
        {
            perror("Error Reading");
            exit(1);
        }

        printf("Client: %s\n",buffer );
        if(strncmp("###",buffer,3)==0)
        {
            printf("connection Terminated by Client\n");
            break;
        }
        
        bzero(buffer,sizeof(buffer));
        fprintf(stdout,"> ");
        fgets(buffer,sizeof(buffer),stdin);

        n=write(newsockfd,buffer,sizeof(buffer));//Write to the socket
        if(n<0)
        {
            perror("Error Writing");
            exit(1);
        }
        if(strncmp("###",buffer,3)==0)
        {
            printf("Connection Terminated by You\n");
            break;
        }

    }
    close(newsockfd);//Close the session socket
    close(sockfd);//Close the socket
    
}