#include <sys/socket.h>       /*  socket definitions        */
#include <sys/types.h>        /*  socket types              */
#include <arpa/inet.h>        /*  inet (3) funtions         */
#include <unistd.h>           /*  misc. UNIX functions      */

#include <stdlib.h>
#include <stdio.h>

#define PORT        (2002)
#define Q_SIZE      (10)

int main() {
    struct sockaddr_in sockaddr, clientaddr;
    int sock_desc = -1; // Default to error val
    if ((sock_desc = socket(PF_INET, SOCK_STREAM, 0)) <= 0) {
        printf("SERVER: Error creating socket\n");
        exit(EXIT_FAILURE);
    }

    memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(PORT);
    sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(sock_desc, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) < 0) {
        printf("There has been an issue with binding");
        exit(EXIT_FAILURE);
    }

    if(listen(sock_desc, Q_SIZE) < 0) {
        printf("Error: listen error\n");
        exit(EXIT_FAILURE);
    }

    int conn_s, clientaddrlength;
    while(1) {
    }

    return 0;
}
