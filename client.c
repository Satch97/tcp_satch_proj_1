#include <sys/socket.h>       /*  socket definitions        */
#include <sys/types.h>        /*  socket types              */
#include <arpa/inet.h>        /*  inet (3) funtions         */
#include <unistd.h>           /*  misc. UNIX functions      */

#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

ssize_t ReadLine(int sock_desc, void *buffer, size_t maxlen) {
    int rec, num;
    char c, *lbuff = buffer;
    for(num = 1; num < maxlen; num++) {
        if((rec = read(sock_desc, &c, 1)) == 1) {
            if(c == '\r') continue;
            *lbuff++ = c;
            if(c == '\n') break;
        }
        else if (rec == 0) {
            if (num == 1) return 0; // connection closed
            else break;
        }
        else {
            return -1;
        }
    }
    *lbuff = '\0';
    return num; // number of bytes received inclusive of NULL
}

ssize_t WriteLine(int sock_desc, const void *vptr, size_t len) {
    int nwritten , nrem = len;
    const char *buffer;
    buffer =vptr;

    while(nrem >0) {
        if ((nwritten = write(sock_desc, buffer,nrem)) <= 0) return -1;
        nrem = nrem - nwritten;
        buffer = buffer + nwritten;
    }
    return len;
}

int GetInstr() {
    char x= '\0';
    while(1) {
        printf("Enter \'s\' to send a string \nEnter 'f' to send file\nEnter q to exit\nEnter : ");
    }
}

int main() {
    int sock_desc;
    char *addr = "127.0.0.1";
    char buffer[65535];
    if((sock_desc = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Error : Socket Creation");
    }
    struct sockaddr_in sockaddr;
    memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(2002);

    if (inet_aton (addr, &sockaddr.sin_addr) <= 0)
    {
        printf("Error converting address to 32 bit int\n");
    }

    if (connect (sock_desc, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) < 0 )
    {
        printf("Connection failed\n");
    }

    int action = GetInstr();

    memset(&buffer, 0, sizeof(buffer));
    strcpy(buffer, "CCC\n");
    WriteLine(sock_desc, buffer, strlen(buffer));

    memset(&buffer, 0, sizeof(buffer));
    strcpy(buffer, "string to capitalize\n");
    WriteLine(sock_desc, buffer, strlen(buffer));

    memset(&buffer, 0, sizeof(buffer));
    ReadLine(sock_desc, buffer, 54435);
    printf("Capitalized string : %s\n", buffer);

    return 0;
}
