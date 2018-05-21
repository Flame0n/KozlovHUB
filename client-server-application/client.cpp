#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
//tcp client

using namespace std;

int main()
{
    int descriptor = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //ipv4, tcp - создаем точку соединения, возвращаем дескриптор
    struct sockaddr_in sct;
    memset(&sct, 0, sizeof(sct));
    sct.sin_family = PF_INET;
    sct.sin_port = htons(1339);
    inet_pton(PF_INET, "127.0.0.1", &sct.sin_addr);

    cout <<"Connecting..." << endl;
    if (connect(descriptor, (struct sockaddr*) &sct, sizeof(sct)) == -1){
        cout << "Connection error" << endl;
        return -1;
    }
    cout << "Connected.." << endl;
    while(1)
    {
        char str_out[100];
        cout << "Insert: ";
        cin >> str_out;
        cout << "Sending: " << str_out << endl;
        size_t length = strlen(str_out);
        send(descriptor, &length, sizeof(size_t), 0);
        send(descriptor, str_out, length, 0);
        if (string(str_out) == "stop")
            break;
        char str_in[1000];
        size_t length_in;
        recv(descriptor, &length_in, sizeof(size_t), 0);
        recv(descriptor, str_in, length_in, 0);
        str_in[length_in] = '\0';
        cout << "Received: " << str_in << endl;
    }

    shutdown(descriptor, 2);
    close(descriptor);
    return 0;
}
