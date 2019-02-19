#include <sys/socket.h>       /*  socket definitions        */
#include <sys/types.h>        /*  socket types              */
#include <arpa/inet.h>        /*  inet (3) funtions         */
#include <unistd.h>           /*  misc. UNIX functions      */

#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

ssize_t ReadFile(int sock_desc, void *buffer, size_t maxlen) {
    int rec, num;
    char c, *lbuff = buffer;
    for(num = 0; num < maxlen; num++) {
        if((rec = read(sock_desc, &c, 1)) == 1) {
            *lbuff++ = c;
        }
        else if (rec == 0) {
            if (num == 0) return 0; // connection closed
            else break;
        }
        else {
            return -1;
        }
    }
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
        x = getchar();
        if (x == 's') return 1;
        if (x == 'f') return 2;
        if (x == 'q') exit(0);
    }

}

int main() {
    if (argc != 3) {
        printf("client.exe <server IP> <server port>");
        exit(EXIT_FAILURE);
    }
    int sock_desc;
    char *addr = "127.0.0.1";
    char buffer[MAX_LINE];
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

    if (action == 1) {
        memset(&buffer, 0, sizeof(buffer));
        strcpy(buffer, "CAP\n");
        WriteLine(sock_desc, buffer, strlen(buffer));

        memset(&buffer, 0, sizeof(buffer));
        printf("Enter string : ");
        int c, len = 0;
        fflush(stdout);
        c = getchar();
        while (c == '\n' || c == '\r') c = getchar(); // remove junk from stdin
        while (c != '\n' && c != EOF && c != '\r' && len < MAX_LINE - 1) {
            buffer[len++] = c;
            c = getchar();
        }
        buffer[len] = '\n'; // append newline
        WriteLine(sock_desc, buffer, strlen(buffer));

        memset(&buffer, 0, sizeof(buffer));
        ReadLine(sock_desc, buffer, MAX_LINE -1);
        int strlen = strtol(buffer, NULL, 10);
        printf("Number of bytes to receive is %d\n", strlen);

        memset(&buffer, 0, sizeof(buffer));
        ReadLine(sock_desc, buffer, MAX_LINE -1);
        printf("Capitalized string : %s\n", buffer);
    }
    else if (action == 2) {
        memset(&buffer, 0, sizeof(buffer));
        strcpy(buffer, "FILE\n");
        WriteLine(sock_desc, buffer, strlen(buffer));

        memset(&buffer, 0, sizeof(buffer));
        printf("Enter string : ");
        scanf("%s", buffer);
        char filename[500];
        strcpy(filename, buffer); // todo : might use malloc here
        strcat(buffer, "\n"); // add newline at end
        WriteLine(sock_desc, buffer, strlen(buffer));

        memset(&buffer, 0, sizeof(buffer));
        ReadLine(sock_desc, buffer, MAX_LINE -1);
        int mystrlen = strtol(buffer, NULL, 10);
        strcpy(filename,"testimg.jpg"); // testing file name
        FILE * fp;

        if (fp = fopen(filename,"wb")) {
            while(mystrlen > MAX_LINE) {
                ReadFile(sock_desc, buffer, MAX_LINE);
                fwrite(buffer, 1, MAX_LINE, fp);
                mystrlen -= MAX_LINE;
            }
            if (mystrlen > 0) {
                memset(&buffer, 0, sizeof(buffer));
                ReadFile(sock_desc, buffer, mystrlen);
                fwrite(buffer, 1, mystrlen, fp);
            }
            fflush(fp);
            fclose(fp);
        }
        else {
            printf("Cannot open file for writing\n");
        }
    }


    return 0;
}
