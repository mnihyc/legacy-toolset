       
    #include <stdio.h>   
    #include <stdlib.h>   
    #include <errno.h>   
    #include <winsock2.h>    
    #include <io.h>   
    #include <signal.h>   
       
    #pragma comment(lib, "ws2_32.lib")   
       
    #define VERSION                "1.02"   
    #define TIMEOUT                300   
    #define MAXSIZE                20480   
    #define HOSTLEN                40   
    #define CONNECTNUM             5   
       
    // define 2 socket struct   
    struct transocket    
    {   
        SOCKET fd1;   
        SOCKET fd2;   
    };   
       
    // define function    
    void ver();   
    void usage(char *prog);   
    void transmitdata(LPVOID data);   
    void getctrlc(int j);   
    void closeallfd();   
    void makelog(char *buffer, int length);   
    void proxy(int port);   
    //void bind2bind(int port1, int port2);   
    void bind2conn(int port1, char *host, int port2);   
    //void conn2conn(char *host1, int port1, char *host2, int port2);   
    int testifisvalue(char *str);   
    int create_socket();   
    int create_server(int sockfd, int port);   
    int client_connect(int sockfd, char* server, int port);   
       
    // define GLOBAL variable here   
    extern int errno;   
    FILE *fp;   
    int method=0;   
    int  v=0; //0表示默认状态为不显示任何信息   
    //int connectnum=0;   
       
    //************************************************************************************   
    //    
    //  function main   
    //   
    //************************************************************************************   
    VOID main(int argc, char* argv[])   
    {   
        char **p;   
        char sConnectHost[HOSTLEN], sTransmitHost[HOSTLEN];   
        int iConnectPort=0, iTransmitPort=0;   
        char *logfile=NULL;   
       
        ver();   
        memset(sConnectHost, 0, HOSTLEN);   
        memset(sTransmitHost, 0, HOSTLEN);   
       
        p=argv;   
        while(*p)   
        {   
            if(stricmp(*p, "-log") == 0)   
            {   
                if(testifisvalue(*(p+1)))   
                {   
                    logfile = *(++p);   
                }       
                else   
                {   
                    printf("[-] ERROR: Must supply logfile name.\r\n");   
                    return;   
                }   
                p++;   
                continue;   
            }   
               
            p++;   
        }   
       
        if(logfile !=NULL)   
        {   
            fp=fopen(logfile,"a");   
            if(fp == NULL )    
            {   
                printf("[-] ERROR: open logfile");   
                return;   
            }   
       
            makelog("====== Start ======\r\n", 22);   
        }   
       
       
        // Win Start Winsock.   
        WSADATA wsadata;   
        WSAStartup(MAKEWORD(1, 1), &wsadata);   
       
        signal(SIGINT, &getctrlc);   
       
        if(argc > 2)   
        {   
            if(stricmp(argv[1], "-listen") == 0 && argc >= 4)   
            {   
                iConnectPort = atoi(argv[2]);   
                iTransmitPort = atoi(argv[3]);   
                method = 1;   
            }   
            else   
            if(stricmp(argv[1], "-tran") == 0 && argc >= 5)   
            {   
                iConnectPort = atoi(argv[2]);   
                strncpy(sTransmitHost, argv[3], HOSTLEN);   
                iTransmitPort = atoi(argv[4]);   
                if(argc  >5)   
                    if(stricmp(argv[5], "-v") == 0) v =1; //如果添加了-v，则显示输出信息   
                method = 2;   
            }   
            else   
            if(stricmp(argv[1], "-slave") == 0 && argc >= 6)   
            {   
                strncpy(sConnectHost, argv[2], HOSTLEN);   
                iConnectPort = atoi(argv[3]);   
                strncpy(sTransmitHost, argv[4], HOSTLEN);   
                iTransmitPort = atoi(argv[5]);   
                method = 3;   
            }   
        }   
       
        switch(method)   
        {   
        case 1:   
            //bind2bind(iConnectPort, iTransmitPort);   
            break;   
        case 2:   
            bind2conn(iConnectPort, sTransmitHost, iTransmitPort);   
            break;   
        case 3:   
            //conn2conn(sConnectHost, iConnectPort, sTransmitHost, iTransmitPort);   
            break;   
        default:   
            usage(argv[0]);   
            break;   
        }   
           
        if(method)   
        {   
            closeallfd();   
        }   
       
        WSACleanup();   
       
        return;   
    }   
       
       
    //************************************************************************************   
    //    
    //  print version message   
    //   
    //************************************************************************************   
    VOID ver()    
    {       
        if(v != 0 )   
        printf("======================== htran V%s =======================\r\n", VERSION);   
      //  printf("=========== Code by lion & bkbll, Welcome to [url]http://www.cnhonker.com[/url] ==========\r\n\n");   
    }   
       
    //************************************************************************************   
    //    
    //  print usage message   
    //   
    //************************************************************************************   
    VOID usage(char* prog)    
    {       
        printf("[Usage of Packet Transmit:]\r\n");   
        printf("  %s -<listen|tran|slave> <option> [-log logfile]\n\n", prog);   
        printf("[option:]\n");   
        printf("  -listen <ConnectPort> <TransmitPort>\n");   
        printf("  -tran  <ConnectPort> <TransmitHost> <TransmitPort>\n");   
        printf("  -slave  <ConnectHost> <ConnectPort> <TransmitHost> <TransmitPort>\n\n");   
        return;   
    }   
       
    //************************************************************************************   
    //    
    //  test if is value    
    //   
    //************************************************************************************   
    int testifisvalue(char *str)   
    {   
        if(str == NULL ) return(0);   
        if(str[0]=='-')    
            return 0;   
        return 1;   
    }   
       
    //************************************************************************************   
    //    
    //  LocalHost:ConnectPort transmit to TransmitHost:TransmitPort   
    //   
    //************************************************************************************   
    void bind2conn(int port1, char *host, int port2)   
    {   
        SOCKET sockfd,sockfd1,sockfd2;   
        struct sockaddr_in remote;   
        int size;   
        char buffer[1024];   
       
        HANDLE hThread=NULL;   
        transocket sock;   
        DWORD dwThreadID;   
       
        if (port1 > 65535 || port1 < 1)   
        {   
            if(v != 0 )printf("[-] ConnectPort invalid.\r\n");   
            return;   
        }   
       
        if (port2 > 65535 || port2 < 1)   
        {   
            if(v != 0 ) printf("[-] TransmitPort invalid.\r\n");   
            return;   
        }   
           
        memset(buffer,0,1024);   
       
        if((sockfd=create_socket()) == INVALID_SOCKET) return;   
       
        if(create_server(sockfd, port1) == 0)    
        {   
            closesocket(sockfd);   
            return;   
        }   
           
        size=sizeof(struct sockaddr);   
        while(1)   
        {   
            if(v != 0 ) printf("[+] Waiting for Client ......\r\n");       
            if((sockfd1=accept(sockfd,(struct sockaddr *)&remote,&size))<0)   
            {   
                if(v != 0 )printf("[-] Accept error.\r\n");   
                continue;   
            }   
       
            if(v != 0 )  printf("[+] Accept a Client from %s:%d ......\r\n",   
            inet_ntoa(remote.sin_addr), ntohs(remote.sin_port));   
              if((sockfd2=create_socket())==0)    
              {   
                  closesocket(sockfd1);   
                  continue;       
              }   
                if(v != 0 ) printf("[+] Make a Connection to %s:%d ......\r\n",host,port2);   
              fflush(stdout);   
       
            if(client_connect(sockfd2,host,port2)==0)   
              {   
                  closesocket(sockfd2);   
                  sprintf(buffer,"[SERVER]connection to %s:%d error\r\n", host, port2);   
                  send(sockfd1,buffer,strlen(buffer),0);   
                  memset(buffer, 0, 1024);   
                  closesocket(sockfd1);   
                  continue;   
              }   
                 
            if(v != 0 )printf("[+] Connect OK!\r\n");   
       
            sock.fd1 = sockfd1;   
            sock.fd2 = sockfd2;   
       
            hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)transmitdata, (LPVOID)&sock, 0, &dwThreadID);    
            if(hThread == NULL)    
            {   
                TerminateThread(hThread, 0);   
                return;   
            }   
       
            Sleep(1000);   
            if(v != 0 ) printf("[+] CreateThread OK!\r\n\n");   
        }   
    }   
       
       
    //************************************************************************************   
    //    
    //  Socket Transmit to Socket   
    //   
    //************************************************************************************   
    void transmitdata(LPVOID data)   
    {   
        SOCKET fd1, fd2;   
        transocket *sock;   
        struct timeval timeset;   
        fd_set readfd,writefd;   
        int result,i=0;   
        char read_in1[MAXSIZE],send_out1[MAXSIZE];   
        char read_in2[MAXSIZE],send_out2[MAXSIZE];   
        int read1=0,totalread1=0,send1=0;   
        int read2=0,totalread2=0,send2=0;   
        int sendcount1,sendcount2;   
        int maxfd;   
        struct sockaddr_in client1,client2;   
        int structsize1,structsize2;   
        char host1[20],host2[20];   
        int port1=0,port2=0;   
        char tmpbuf[100];   
       
        sock = (transocket *)data;   
        fd1 = sock->fd1;   
        fd2 = sock->fd2;   
       
        memset(host1,0,20);   
        memset(host2,0,20);   
        memset(tmpbuf,0,100);   
       
        structsize1=sizeof(struct sockaddr);   
        structsize2=sizeof(struct sockaddr);   
           
        if(getpeername(fd1,(struct sockaddr *)&client1,&structsize1)<0)   
        {   
            strcpy(host1, "fd1");   
        }   
        else   
        {       
    //        printf("[+]got, ip:%s, port:%d\r\n",inet_ntoa(client1.sin_addr),ntohs(client1.sin_port));   
            strcpy(host1, inet_ntoa(client1.sin_addr));   
            port1=ntohs(client1.sin_port);   
        }   
       
        if(getpeername(fd2,(struct sockaddr *)&client2,&structsize2)<0)   
        {   
            strcpy(host2,"fd2");   
        }   
        else   
        {       
    //        printf("[+]got, ip:%s, port:%d\r\n",inet_ntoa(client2.sin_addr),ntohs(client2.sin_port));   
            strcpy(host2, inet_ntoa(client2.sin_addr));   
            port2=ntohs(client2.sin_port);   
        }   
       
        if(v != 0 ) printf("[+] Start Transmit (%s:%d <-> %s:%d) ......\r\n\n", host1, port1, host2, port2);   
         
        maxfd=max(fd1,fd2)+1;   
        memset(read_in1,0,MAXSIZE);   
        memset(read_in2,0,MAXSIZE);   
        memset(send_out1,0,MAXSIZE);   
        memset(send_out2,0,MAXSIZE);   
         
        timeset.tv_sec=TIMEOUT;   
        timeset.tv_usec=0;   
       
        while(1)   
        {   
            FD_ZERO(&readfd);   
            FD_ZERO(&writefd);    
             
            FD_SET((UINT)fd1, &readfd);   
            FD_SET((UINT)fd1, &writefd);   
            FD_SET((UINT)fd2, &writefd);   
            FD_SET((UINT)fd2, &readfd);   
             
            result=select(maxfd,&readfd,&writefd,NULL,&timeset);   
            if((result<0) && (errno!=EINTR))   
            {   
                if(v != 0 )printf("[-] Select error.\r\n");   
                break;   
            }   
            else if(result==0)   
            {   
                if(v != 0 )printf("[-] Socket time out.\r\n");   
                break;   
            }   
               
            if(FD_ISSET(fd1, &readfd))   
            {   
                /* must < MAXSIZE-totalread1, otherwise send_out1 will flow */   
                if(totalread1<MAXSIZE)   
                 {   
                    read1=recv(fd1, read_in1, MAXSIZE-totalread1, 0);    
                    if((read1==SOCKET_ERROR) || (read1==0))   
                      {   
                        if(v != 0 ) printf("[-] Read fd1 data error,maybe close?\r\n");   
                          break;   
                      }   
                   
                    memcpy(send_out1+totalread1,read_in1,read1);   
                    sprintf(tmpbuf,"\r\nRecv %5d bytes from %s:%d\r\n", read1, host1, port1);   
                    if(v != 0 ) printf(" Recv %5d bytes %16s:%d\r\n", read1, host1, port1);   
                    makelog(tmpbuf,strlen(tmpbuf));   
                    makelog(read_in1,read1);   
                    totalread1+=read1;   
                    memset(read_in1,0,MAXSIZE);   
                }   
            }   
       
            if(FD_ISSET(fd2, &writefd))   
            {   
                int err=0;   
                sendcount1=0;   
                while(totalread1>0)   
                {   
                    send1=send(fd2, send_out1+sendcount1, totalread1, 0);   
                    if(send1==0)break;   
                    if((send1<0) && (errno!=EINTR))   
                    {   
                        if(v != 0 ) printf("[-] Send to fd2 unknow error.\r\n");   
                        err=1;   
                        break;   
                    }   
                       
                    if((send1<0) && (errno==ENOSPC)) break;   
                    sendcount1+=send1;   
                    totalread1-=send1;    
       
                    if(v != 0 )printf(" Send %5d bytes %16s:%d\r\n", send1, host2, port2);   
                }   
                      
                if(err==1) break;   
                if((totalread1>0) && (sendcount1>0))   
                {   
                    /* move not sended data to start addr */   
                    memcpy(send_out1,send_out1+sendcount1,totalread1);   
                    memset(send_out1+totalread1,0,MAXSIZE-totalread1);   
                }   
                else   
                memset(send_out1,0,MAXSIZE);   
            }    
               
            if(FD_ISSET(fd2, &readfd))   
            {   
                if(totalread2<MAXSIZE)   
                {   
                    read2=recv(fd2,read_in2,MAXSIZE-totalread2, 0);    
                    if(read2==0)break;   
                    if((read2<0) && (errno!=EINTR))   
                    {   
                        if(v != 0 ) printf("[-] Read fd2 data error,maybe close?\r\n\r\n");   
                        break;   
                    }   
       
                    memcpy(send_out2+totalread2,read_in2,read2);   
                    sprintf(tmpbuf, "\r\nRecv %5d bytes from %s:%d\r\n", read2, host2, port2);   
                    if(v != 0 ) printf(" Recv %5d bytes %16s:%d\r\n", read2, host2, port2);   
                    makelog(tmpbuf,strlen(tmpbuf));   
                   makelog(read_in2,read2);   
                   totalread2+=read2;   
                   memset(read_in2,0,MAXSIZE);   
                }   
          }   
       
            if(FD_ISSET(fd1, &writefd))   
            {   
                int err2=0;   
                  sendcount2=0;   
                  while(totalread2>0)   
                  {   
                       send2=send(fd1, send_out2+sendcount2, totalread2, 0);   
                      if(send2==0)break;   
                      if((send2<0) && (errno!=EINTR))   
                      {   
                        if(v != 0 )   printf("[-] Send to fd1 unknow error.\r\n");   
                        err2=1;   
                          break;   
                      }   
                      if((send2<0) && (errno==ENOSPC)) break;   
                      sendcount2+=send2;   
                      totalread2-=send2;    
                       
                        if(v != 0 )printf(" Send %5d bytes %16s:%d\r\n", send2, host1, port1);   
                  }   
                if(err2==1) break;   
                  if((totalread2>0) && (sendcount2 > 0))   
                {   
                    /* move not sended data to start addr */   
                    memcpy(send_out2, send_out2+sendcount2, totalread2);   
                    memset(send_out2+totalread2, 0, MAXSIZE-totalread2);   
                }   
                else   
                    memset(send_out2,0,MAXSIZE);   
            }    
       
            Sleep(5);   
        }    
         
        closesocket(fd1);   
        closesocket(fd2);   
    //    if(method == 3)   
    //        connectnum --;   
           
        if(v != 0 ) printf("\r\n[+] OK! I Closed The Two Socket.\r\n");     
    }   
       
    void getctrlc(int j)   
    {   
        if(v != 0 ) printf("\r\n[-] Received Ctrl+C\r\n");   
        closeallfd();   
        exit(0);   
    }   
       
    void closeallfd()   
    {   
        int i;   
       
        if(v != 0 )printf("[+] Let me exit ......\r\n");   
        fflush(stdout);   
       
        for(i=3; i<256; i++)   
        {   
            closesocket(i);       
        }   
       
        if(fp != NULL)    
        {   
            fprintf(fp,"\r\n====== Exit ======\r\n");   
            fclose(fp);   
        }   
       
            if(v != 0 )printf("[+] All Right!\r\n");   
    }   
       
    int create_socket()   
    {     
        int sockfd;   
         
        sockfd=socket(AF_INET,SOCK_STREAM,0);   
        if(sockfd<0)   
        {   
            if(v != 0 )printf("[-] Create socket error.\r\n");   
            return(0);   
        }   
           
        return(sockfd);       
    }   
       
    int create_server(int sockfd,int port)   
    {   
        struct sockaddr_in srvaddr;   
        int on=1;   
           
        memset(&srvaddr, 0, sizeof(struct sockaddr));   
       
        srvaddr.sin_port=htons(port);   
        srvaddr.sin_family=AF_INET;   
        srvaddr.sin_addr.s_addr=htonl(INADDR_ANY);   
         
        setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR, (char*)&on,sizeof(on));  //so I can rebind the port   
       
        if(bind(sockfd,(struct sockaddr *)&srvaddr,sizeof(struct sockaddr))<0)   
        {   
            if(v != 0 ) printf("[-] Socket bind error.\r\n");   
            return(0);   
        }   
       
        if(listen(sockfd,CONNECTNUM)<0)   
        {   
            if(v != 0 ) printf("[-] Socket Listen error.\r\n");   
            return(0);   
        }   
           
        return(1);   
    }   
       
    int client_connect(int sockfd,char* server,int port)   
    {   
      struct sockaddr_in cliaddr;   
      struct hostent *host;   
       
      if(!(host=gethostbyname(server)))   
      {   
            if(v != 0 ) printf("[-] Gethostbyname(%s) error:%s\n",server,strerror(errno));   
          return(0);   
      }         
         
      memset(&cliaddr, 0, sizeof(struct sockaddr));   
      cliaddr.sin_family=AF_INET;   
      cliaddr.sin_port=htons(port);   
      cliaddr.sin_addr=*((struct in_addr *)host->h_addr);   
         
      if(connect(sockfd,(struct sockaddr *)&cliaddr,sizeof(struct sockaddr))<0)   
      {   
            if(v != 0 ) printf("[-] Connect error.\r\n");   
          return(0);   
      }   
      return(1);   
    }   
       
    void makelog(char *buffer,int length)   
    {   
        if(fp !=NULL)   
        {   
    //        fprintf(fp, "%s", buffer);   
    //        printf("%s",buffer);   
            write(fileno(fp),buffer,length);   
    //        fflush(fp);   
        }   
    }  