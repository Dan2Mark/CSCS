#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#pragma comment ( lib, "ws2_32.lib" )
#include <stdio.h>
#include <winsock2.h>
#include <iostream>
#include <exception>
#include <string>
#include <locale.h>
#include <Windows.h>
#include <sstream>
#include "Pars.h"
#include <cstdio>
#include "bitmap/bitmap_image.hpp"
#define PORT 7777    // порт сервера

using namespace std;


Parser* pb;

DWORD WINAPI print(LPVOID lpParam) {
    while (true){
    pb->print();
    pb->clock_();
    Sleep (100);
}
    return 0;
}

int main(int argc, char* argv[])
{   bool end_prog = false;
    char buff[1024], buff_[1024];
    // подключение библиотеки 
    if (WSAStartup(0x202, (WSADATA*)&buff[0]))
    {
        printf("WSAStartup error: %d\n", WSAGetLastError());
        return -1;
    };

    // создание сокета
    SOCKET Socket;
    Socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (Socket == INVALID_SOCKET)
    {
        printf("Socket() error: %d\n", WSAGetLastError());
        WSACleanup();
        return -1;
    };
    
    // связывание сокета с локальным адресом 
    sockaddr_in local_addr;
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = INADDR_ANY;
    local_addr.sin_port = htons(PORT);

    if (bind(Socket, (sockaddr*)&local_addr, sizeof(local_addr)))
    {
        printf("bind error: %d\n", WSAGetLastError());
        system("pause");
        closesocket(Socket);
        WSACleanup();
        return -1;
    };

    Parser b;
    pb = &b;
    DWORD dwThreadId, dwThrdParam = 1;
    HANDLE hThread = CreateThread(NULL, 0, print, &dwThrdParam, 0, &dwThreadId);
    // обработка пакетов, присланных клиентами
    while (!end_prog)
    { 
        sockaddr_in client_addr;
        int client_addr_size = sizeof(client_addr);
        int bsize = recvfrom(Socket, &buff[0], sizeof(buff) - 1, 0, (sockaddr*)&client_addr, &client_addr_size);
        if (bsize == SOCKET_ERROR) {
            printf("recvfrom() error: %d\n", WSAGetLastError());
        };
        // Определяем IP-адрес клиента и прочие атрибуты
        HOSTENT* hst; 
        hst = gethostbyaddr((char*)&client_addr.sin_addr, 4, AF_INET);
       // printf("+%s [%s:%d] new DATAGRAM!\n", (hst) ? hst->h_name : "Unknown host", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // добавление завершающего нуля
        buff[bsize-1] = ' ';
        buff[bsize] = 0;

        string err;
        b.do_cmd(buff, &end_prog, &err);
        int bsize_;
        if (err != "--------------------------------------\n") {
            strcpy(buff_, err.c_str());
        }
        else strcpy(buff_, "Команда успешно выполнена\n---------------------------------------\n");
        bsize_ = strlen(buff_);
        // посылка датаграммы клиенту
        sendto(Socket, &buff_[0], bsize_, 0, (sockaddr*)&client_addr, sizeof(client_addr));
    }

    CloseHandle(hThread);
    return 0;
}