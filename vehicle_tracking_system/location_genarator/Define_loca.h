#include <stdio.h>
#include <iostream>
#include <pthread.h> 
#define     PORT_1    12346
#define     PORT_2    12347
#define     TRUE      1
#define     FALSE     0
void *Recv_from_ad (void *arg); //recieve request from VTS as a server 
void *Add_Sensor(void *arg); //create a connection from local_genarator to VTS as a client
int ConnectToVts(); //sensor send data to VTS
void *Write_Xml(void* arg);
class DataStruct
{
    public:
        char status[20];
        int ID ;
        int lagi, longi;
    public:
        char Name[20];
        int flag;
        char* SetName(char *arg);
        void SetID (int *arg);
        void SetCoordinate ();
        void SetStatus(char *arg);
        void Xuat();
        char* GetStatus();
};
