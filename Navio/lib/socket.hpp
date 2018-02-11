#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <unistd.h>

class Socket
{

public:
    Socket(char * ip,char * port)
    {
        std::cout<< ip << "   " << port << std::endl;
        sockfd = socket(AF_INET,SOCK_DGRAM,0);
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = inet_addr(ip);
        servaddr.sin_port = htons(atoi(port));
    }

    Socket()
    {
        sockfd = socket(AF_INET,SOCK_DGRAM,0);
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        servaddr.sin_port = htons(7000);
    }

    void output(int Hz, float gx, float gy, float gz, float ax, float ay, float az,float mx, float my, float mz)
    {
        sprintf(sendline,"%10f %10f %10f %10f %10f %10f %10f %10f %10f %dHz\n",
		gx,gy,gz,ax,ay,az,mx,my,mz,Hz);
        sendto(sockfd, sendline, strlen(sendline), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
    }

private:
    int sockfd;
    struct sockaddr_in servaddr = {0};
    char sendline[256];

};
