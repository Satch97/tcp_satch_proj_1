#include <sys/socket.h>       /*  socket definitions        */
#include <sys/types.h>        /*  socket types              */
#include <arpa/inet.h>        /*  inet (3) funtions         */
#include <unistd.h>           /*  misc. UNIX functions      */

#include <stdlib.h>
#include <stdio.h>

int main() {
    struct sockaddr_in sockaddr;
    int sock_desc = -1; // Default to error val
    if ((sock_desc = socket(PF_INET, SOCK_STREAM, 0)) <= 0){
        printf("SERVER: Error creating socket\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}
