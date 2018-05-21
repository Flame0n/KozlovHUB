#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int main()
{
    char msg[100];
    struct sockaddr_in sockAddr;
    int descptr;
    descptr=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(descptr==-1)
    {
        printf("Cant create a socket/");
        return 0;
    }
    memset(&sockAddr,0,sizeof(sockAddr));
    sockAddr.sin_family=AF_INET;
    sockAddr.sin_port=htons(3425);
    sockAddr.sin_addr.s_addr=htonl(INADDR_ANY);

    if( bind(descptr,(struct sockaddr*)&sockAddr,sizeof(sockAddr))<0)
    {
        printf("Cant bind.");
        close(descptr);
        return 0;
    }

    if (listen(descptr, 10) == -1)
    {
        printf("Cant listen.");
        close(descptr);
        return 0;
	}
	int count=0;
    while(count<10)
    {
        count++;
        int connect = accept(descptr, 0, 0);

		if (connect < 0) {
			perror("Accept error.");
			close(descptr);
			return 0;
		}

		read(connect,msg,sizeof(msg));
		printf(msg);
		msg[strlen(msg)-1]='\0';
		strcat(msg,".EDITED\n");
        write(connect,msg,sizeof(msg));
		close(connect);
    }

    close(descptr);
    return 0;
}


