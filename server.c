#include <sys/socket.h>       /*  socket definitions        */
#include <sys/types.h>        /*  socket types              */
#include <arpa/inet.h>        /*  inet (3) funtions         */
#include <unistd.h>           /*  misc. UNIX functions      */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PORT        (2002)
#define Q_SIZE      (10)
#define MAX_LINE    (65535)

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
    char buffer[MAX_LINE], tempbuffer[MAX_LINE];
    while(1) {
        if((conn_s = accept(sock_desc, (struct sockaddr *) &clientaddr, (socklen_t *) &clientaddrlength)) < 0) {
            printf("Error: accept\n");
            exit(EXIT_FAILURE);
        }
        printf("Connection made to clientaddr %d\n", clientaddr.sin_addr.s_addr);

        ReadLine(conn_s, buffer, MAX_LINE - 1);

        char* cap = "CAP\n";
        char* file = "FILE\n";
        if(strcmp(buffer, cap) == 0) {
            memset(&buffer, 0, sizeof(buffer));
            ReadLine(conn_s, buffer, MAX_LINE - 1);
            for(char* ch=buffer; *ch != '\0' ; ch++) *ch=toupper(*ch);

            int wordlen = strlen(buffer);
            memset(&tempbuffer, 0, sizeof(tempbuffer));
            sprintf(tempbuffer, "%d\n", wordlen );
            WriteLine(conn_s, tempbuffer, strlen(tempbuffer));

            WriteLine(conn_s, buffer, strlen(buffer));
            memset(&buffer, 0, sizeof(buffer));
        }
        else if (strcmp(buffer,file) == 0) {
            memset(&buffer, 0, sizeof(buffer));
            ReadLine(conn_s, buffer, MAX_LINE - 1);
            printf("received : %s with length %d\n", buffer, strlen(buffer));
            if (buffer[strlen(buffer) - 1] == '\n') buffer[strlen(buffer) - 1] = '\0';

            FILE *fp;
            memset(&tempbuffer, 0, sizeof(tempbuffer));
            if(fp = fopen (buffer, "rb")) {
                fseek(fp, 0, SEEK_END);
                int numbytes = ftell(fp);
                printf("Number of bytes to read is %d", numbytes);
                fseek(fp, 0, SEEK_SET);
                while(numbytes  > MAX_LINE) {
            } else {
                strcpy(buffer, "NOT FOUND");
                sprintf(tempbuffer, "%d\n", strlen(buffer) );
                strcat(buffer, "\n");
                WriteLine(conn_s, tempbuffer, strlen(tempbuffer));
                WriteLine(conn_s, buffer, strlen(buffer));
            }

        }
        close(conn_s);
    }

    return 0;
}
