#include <stdio.h>
#include <iostream>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <pthread.h> 
#include "Define_loca.h"
#include <stdlib.h>
#include "tinyxml.h"
#include "unistd.h"
using namespace std;

                    /* Send data from sensor to VTS for user */
                    
void *Add_Sensor(void *arg)
{
    DataStruct* temp  = (DataStruct*)(arg);
    int a;
    //char name[]= "SS1";
    char status_open[] = "OPEN";
    char status_exit[] = "exit";
    char message[20] = "OKE";
    char buff[20];
    cout << "welcome " <<temp->Name<<"\n";
    int sock;
    sock = ConnectToVts();
    //DataStruct DataSensor;
    while(temp->flag == 0)
    {
        temp->SetStatus(status_open);
        temp->SetID(&a);
        //temp.SetName(temp.Name);
        temp->SetCoordinate();
        write(sock,temp,sizeof(DataStruct));
        read(sock,buff,sizeof(buff));
        if(strncmp(buff,"OKE",sizeof("OKE")-1)!= 0) // nhan error
        {
            temp->flag ==1;
            break;
        }
       // temp = *(Name_SenSor*)(arg);
    }
    if(temp->flag == 1)
    {
        cout << "SS2 send exit" <<"\n";
        temp->SetStatus(status_exit);
        temp->SetID(&a);
        //temp.SetName(temp.Name);
        temp->SetCoordinate();
        write(sock,temp,sizeof(DataStruct));
    }
    close(sock);
}

            /*Define function for class DataStruct*/

void DataStruct::SetCoordinate()
{
    longi = rand();
    lagi = rand();
}   
void DataStruct::SetID (int *arg)
{
    ID = rand()%100+1;
}
char* DataStruct::SetName(char *arg)
{
    strcpy(Name,arg);
    return Name;
}
void DataStruct::SetStatus(char*arg)
{
    strcpy(status,arg);
}

                    /*create a thread for sensor*/

// pthread_t newthread (pthread_t *arg1)
// {
//     pthread_create(arg1,NULL,Add_user,NULL);
//     return (*arg1);
// }



                    /*recieve request from VTS*/

void *Recv_from_ad (void *arg)
{
    // Name_SenSor sensor1; 
    // Name_SenSor sensor2;
    // Name_SenSor sensor3;
    DataStruct sensor1;
    DataStruct sensor2;
    DataStruct sensor3;
    SenSor_Data_Struct Data_SenSors;
    Data_SenSors.sensor1 = sensor1;
    Data_SenSors.sensor2 = sensor2;
    Data_SenSors.sensor3 = sensor3;
    sensor1.flag = 1;
    strcpy(sensor1.Name,"SenSor1");
    //sensor1.Name = SenSor1;
    sensor2.flag = 1;
    strcpy(sensor2.Name,"SenSor2");
    sensor3.flag = 1;
    strcpy(sensor3.Name,"SenSor3");
    pthread_t sensor1_thread;
    pthread_t sensor2_thread;
    pthread_t sensor3_thread;
    pthread_t CreateXml;    
    char buffer[100];
    sockaddr_in server_addr;
    int opt =1; 
    int listenfd ; // socket for listenning
    int newsocket   ; //socket for communication between server and client
    int addr_lenght = sizeof(server_addr);
    memset(&buffer,0,sizeof(buffer));
    memset(&server_addr,0,sizeof(server_addr));
    listenfd = socket(AF_INET, SOCK_STREAM, 0);// create a socket 

    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                   &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    };

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT_1);
    
    if(bind(listenfd,(sockaddr*)&server_addr, sizeof(server_addr)) <0)
    {
        close(listenfd);
        exit(2);
    } // bind IP address for server
    //NOTE: in server you must bind the Ip add for it, in client you just connect to server by server add
    listen(listenfd,3); // waiting for connection maximum 3 geue
    newsocket = accept(listenfd,(sockaddr*) &server_addr,(socklen_t*)&addr_lenght);
    cout << "da ket noi" << "\n";
    pthread_create(&CreateXml,NULL,Write_Xml,&Data_SenSors);
    while(1)
    {
        read(newsocket,&buffer,sizeof(buffer));
        if (!strncmp(buffer,"open1", sizeof("open1") - 1) && sensor1.flag!=0)
        {
            sensor1.flag = FALSE;
            pthread_create(&sensor1_thread,NULL,Add_Sensor,&sensor1);
        }
        if (!strncmp(buffer,"close1", sizeof("close1") - 1))
        {
            sensor1.flag = TRUE;
            pthread_join(sensor1_thread,NULL);
        }

        if (!strncmp(buffer,"open2", sizeof("open2") - 1)&& sensor2.flag!=0)
        {
            sensor2.flag = FALSE;
            pthread_create(&sensor2_thread,NULL,Add_Sensor,&sensor2);
        }

        if (!strncmp(buffer,"close2", sizeof("close2") - 1)&&sensor3.flag!=0)
        {
            sensor2.flag = TRUE;
            pthread_join(sensor2_thread,NULL);

        }
        bzero(buffer, sizeof(buffer));

        //pthread_join (sensor1_thread,NULL);
    }
}
void *Write_Xml(void* arg)
{
    SenSor_Data_Struct* temp = (SenSor_Data_Struct*) arg;
    //Tạo đối tượng quản lý tài liệu XML
	TiXmlDocument doc;

	//Tạo chỉ thị của tài liệu XML bao gồm version, endcoding sau đó thêm dec vào tài liệu
	TiXmlDeclaration *dec = new TiXmlDeclaration("1.0", "utf-8", "");
	//Thêm dec vào tài liệu
	doc.LinkEndChild(dec);

	//Tạo comment và thêm comment vào tài liệu
	TiXmlComment *cmt = new TiXmlComment("Demo read, write, edit XML document using TinyXML library");
	doc.LinkEndChild(cmt);

	//Tạo node root và thêm root vào tài liệu
	TiXmlElement* root = new TiXmlElement("Authors");
	doc.LinkEndChild(root);
   
    while(1)
    {
        //Tạo Author1
	    TiXmlElement* author1 = new TiXmlElement(temp->sensor1.Name);
	    //Set id cho author1
	    author1->SetAttribute("Id", temp->sensor1.ID);
	    //Thêm author1 vào root
	    root->LinkEndChild(author1);


	    //Tạo Author2
	    TiXmlElement* author2 = new TiXmlElement(temp->sensor2.Name);
	    //Set id cho author2
	    author2->SetAttribute("id",temp->sensor2.ID);
	    //Thêm author2 vào root
	    root->LinkEndChild(author2);

        //Tao Author3
        TiXmlElement* author3 = new TiXmlElement(temp->sensor3.Name);
        //Set ID for author2 
        author3->SetAttribute("id",temp->sensor3.ID);
        //add author3 to Root
        root->LinkEndChild(author3);

	    //Tạo node Name 
	    TiXmlElement* author1_name = new TiXmlElement(temp->sensor1.Name);
	    author1->LinkEndChild(author1_name);
	    TiXmlText* name_content_1 = new TiXmlText(temp->sensor1.Name);
	    author1_name->LinkEndChild(name_content_1);

        TiXmlElement* author1_Status = new TiXmlElement(temp->sensor1.status);
	    author1->LinkEndChild(author1_Status);
	    TiXmlText* Status_content_1 = new TiXmlText(temp->sensor1.status);
	    author1_Status->LinkEndChild(Status_content_1);

        TiXmlElement* author1_lag = new TiXmlElement("Lag");
        author1_lag->SetAttribute("Lag",temp->sensor1.lagi);
        author1->LinkEndChild(author1_lag);

        TiXmlElement* author1_log = new TiXmlElement("Log");
        author1_lag->SetAttribute("Log",temp->sensor1.longi);
        author1->LinkEndChild(author1_log);
        sleep(6);
    }
    
	doc.SaveFile("Authors_Write.xml");
	return 0;    
}




                        /*create connection from location_generator to VTS*/

int ConnectToVts()
{
    int sock = 0; //creat a socket
    sockaddr_in serv_addr; // creat server_addr
    memset(&serv_addr,0,sizeof(serv_addr));
    sock = socket(AF_INET,SOCK_STREAM, 0);
    // bind the add to socket
    if(inet_pton(AF_INET, "192.168.122.1", &serv_addr.sin_addr) <= 0)  
    { 
        cout<<"\nInvalid address/ Address not supported \n"; 
        return 0; 
    } 
    serv_addr.sin_family = AF_INET; // truong dia chi, client khoi tao dia chi cho cua server de connect toi
    serv_addr.sin_port = htons(PORT_2);

    // connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        cout<<"\nConnection Failed \n"; 
        return 0; 
    }
    return sock;   
}