#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include "ThreadQueue.h"
//tcp server
using namespace std;

int main()
{
    int descriptor = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in sct;
    memset(&sct, 0, sizeof(sct));
    sct.sin_family = PF_INET;
    sct.sin_port = htons(1339);
    sct.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(descriptor, (struct sockaddr*) &sct, sizeof(sct)); //ѕрив€зываем сокет к номеру порта и ip
    listen(descriptor, 1); //—тавим сокет в состо€ние прослушивани€ с 1 устройства

    cout << "Waiting .." << endl;

    ThreadQueue taskQueue(2);
    while(1){
        const int sconnect = accept(descriptor, 0, 0);
        taskQueue.addThread([](int sock){
            cout << "Thread " << sock << endl;
            while(1)
            {
                char str_in[1000];
                size_t length_in;
                recv(sock, &length_in, sizeof(size_t), 0);
                int a = recv(sock, str_in, length_in, 0);
                str_in[a] = '\0';
                cout << "Received: " << str_in << endl;


                if (string(str_in) == "stop"){
                    cout << "Exit..." << sock << endl;
                    break;
                }

                cout << "Sending: " << str_in << endl;
                size_t length = strlen(str_in);
                send(sock, &length, sizeof(size_t), 0);
                send(sock, str_in, length, 0);
            }
            shutdown(sock, 2);
            close(sock);
        }, sconnect);

    }
    close(descriptor);
    return 0;
}
