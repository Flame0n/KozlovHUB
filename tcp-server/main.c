#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main()
{
    struct sockaddr_in sockAddr;
    int descptr;
    memset(&sockAddr,0,sizeof(sockAddr));
    sockAddr.sin_family=AF_INET;
    sockAddr.sin_port=htons(3425);
    sockAddr.sin_addr.s_addr=htonl(INADDR_LOOPBACK);




    while(1)
    {
        descptr=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
        if(descptr==-1)
        {
            printf("Cant create a socket/");
            return 0;
        }


        if(connect(descptr,(struct sockaddr*)&sockAddr,sizeof(sockAddr))<0)
        {
            printf("Cant create a connection/");
            close(descptr);
        }
        char msg[100];
        printf("Enter msg:");
        fgets(msg,99,stdin);
        send(descptr,msg,sizeof(msg),MSG_WAITALL);
        recv(descptr,msg,sizeof(msg),MSG_WAITALL);
        printf("%s",msg);
        close(descptr);

    }

    return 0;
}


