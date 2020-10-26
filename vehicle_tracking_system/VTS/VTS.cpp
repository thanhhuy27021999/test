#include <stdio.h>
#include <iostream>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include<pthread.h> 
#include "Define.h"
using namespace std;
int  client_socket[30];
struct sockaddr_in address;   
struct timeval timeout;
int cout = 0;
void *End_user (void *arg)
{
    int opt = TRUE;   
    int escape = 1;
    int master_socket , addrlen , new_socket ,  
          max_clients = 30 , activity, i , valread , sd;   
    int max_sd;   
    pthread_t pt[30];         
    //set of socket descriptors  
    fd_set readfds;   
         
    //a message  
    char *message = "ECHO Daemon v1.0 \r\n";   
     
    //initialise all client_socket[] to 0 so not checked  
    for (i = 0; i < max_clients; i++)   
    {   
        client_socket[i] = 0;   
    }   
         
    //create a master socket  
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)   
    {   
        perror("socket failed");   
        exit(EXIT_FAILURE);   
    }   
     
    //set master socket to allow multiple connections ,  
    //this is just a good habit, it will work without this  
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,  
          sizeof(opt)) < 0 )   
    {   
        perror("setsockopt");   
        exit(EXIT_FAILURE);   
    }   
     
    //type of socket created  
    address.sin_family = AF_INET;   
    address.sin_addr.s_addr = INADDR_ANY;   
    address.sin_port = htons(PORT);   
         
    //bind the socket to localhost port 8888  
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)   
    {   
        perror("bind failed");   
        exit(EXIT_FAILURE);   
    }   
    printf("Listener on port %d \n", PORT);   
         
    //try to specify maximum of 3 pending connections for the master socket  
    if (listen(master_socket, 3) < 0)   
    {   
        perror("listen");   
        exit(EXIT_FAILURE);   
    }   
         
    //accept the incoming connection  
    addrlen = sizeof(address);   
    puts("Waiting for connections ...");   
         
    while(TRUE)   
    {   
        timeout.tv_sec = 10;
        //clear the socket set  
        FD_ZERO(&readfds);   
        //add master socket to set  
        FD_SET(master_socket, &readfds);   
        max_sd = master_socket;   
             
        //add child sockets to set
        if(cout != 0 || escape != 0)
        {
            for ( i = 0 ; i < max_clients ; i++)   
            {   
                //socket descriptor  
                sd = client_socket[i];   

                //if valid socket descriptor then add to read list  
                if(sd > 0)   
                FD_SET( sd , &readfds);   
                 
                //highest file descriptor number, need it for the select function  
                if(sd > max_sd)   
                max_sd = sd;   
            } 
        }  
  
     
        //wait for an activity on one of the sockets , timeout is NULL ,  
        //so wait indefinitely  
        activity = select( max_sd + 1 , &readfds , NULL , NULL , &timeout);   //block chuong trinh tai day de doi
        if (activity == 0 && cout==0 && escape==0)
        {
            close(master_socket);
            break;
        }     
        if ((activity < 0) && (errno!=EINTR))   
        {   
            printf("select error");   
        }   
  
       //If something happened on the master socket ,  
        //then its an incoming connection  
        if (FD_ISSET(master_socket, &readfds)) // check lai master co mat trong readfds hay khong    
        {  
            if ((new_socket = accept(master_socket,  
                    (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)   
            {   
                perror("accept");   
                exit(EXIT_FAILURE);   
            }     
                 
            puts("Welcome message sent successfully");   
                 
            //add new socket to array of sockets  
            for (i = 0; i < max_clients; i++)   
            {   
                //if position is empty  
                if( client_socket[i] == 0 )   
                {   
                    client_socket[i] = new_socket;   
                    pthread_create(&pt[i],NULL,mythread,&new_socket);
                    cout++;
                    escape = 0;
                    printf("Adding to list of sockets as %d\n" , i);   
                    break;   
                }   
            }   
        }      
    }      
    return 0;   
}
int main()
{

}