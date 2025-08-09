#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define FILENAME "testfile.txt"

struct tm *get_cur_time()
{

    time_t raw_time;

    time(&raw_time);

    return localtime(&raw_time);
}

int write_fileln(char *message, FILE *fd)
{
    if (fprintf(fd, "%s\n", message) < 0)
    {
        return -1;
    }

    fflush(fd);

    return 0;
}

int recv_message(int sockfd, char *buffer, struct sockaddr_in *peer_addr)
{
    if (sockfd == 0 || buffer == NULL || peer_addr == NULL)
    {
        return -1;
    }
    
    socklen_t peer_len = sizeof(struct sockaddr_in);

    if (recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *) peer_addr, &peer_len) < 0)
    {
        return -1;
    }

    
    return 0;
}

int clear_buf(char *buffer)
{
    if (buffer == NULL)
        return -1;
    
    memset(buffer, 0, BUFFER_SIZE);
    
    return 0;
}

/* Args: 1: Port, */
int main(int argc, char **argv)
{
    /* ----------------------------------- SETTING UP UDP SERVER -------------------------------------------*/
    if (argc != 2)
        return -1;
    
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket() failed");
        return -1;
    }

    struct sockaddr_in peer_addr;

    struct sockaddr_in my_addr = {.sin_family = AF_INET, 
                                   .sin_addr.s_addr = INADDR_ANY,
                                    .sin_port = htons(atoi(argv[1]))};
    
    char buffer[BUFFER_SIZE];

    if (bind(sockfd, (struct sockaddr *) &my_addr, sizeof(my_addr)) < 0)
    {
        perror("bind failed");
        return -1;
    }


    /* ------------------------------------- SETTING UP FILE --------------------------------------------------*/

    FILE *fd;
    
    if ((fd = fopen(FILENAME, "w")) == NULL)
    {
        perror("Opening file failed");
        return -1;
    }


    /* ------------------------------------- MAIN LOOP ---------------------------------------------------------*/
    while (1)
    {
        if ((recv_message(sockfd, buffer, &peer_addr)) < 0)
        {
            perror("recv_message failed");
            close(sockfd);
            return -1;
        }
    
    

        printf("recieved a packet from %s:%d -- Message: %s\n Time: %s", inet_ntoa(peer_addr.sin_addr), ntohs(peer_addr.sin_port), buffer, asctime(get_cur_time()));


        write_fileln(buffer, fd);
        clear_buf(buffer);
        
    }
    close(sockfd);
    fclose(fd);

    return 0;
}
