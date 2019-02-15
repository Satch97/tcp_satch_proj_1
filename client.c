#include <sys/socket.h>       /*  socket definitions        */
#include <sys/types.h>        /*  socket types              */
#include <arpa/inet.h>        /*  inet (3) funtions         */
#include <unistd.h>           /*  misc. UNIX functions      */

#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main() {
    int sock_desc;
    char *addr = "127.0.0.1";
    if((sock_desc = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Error : Socket Creation");
    }
    struct sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(2002);
    inet_aton (addr, &sockaddr.sin_addr);
}
