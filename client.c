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
    if((sock_desc = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Error : Socket Creation");
    }
}
