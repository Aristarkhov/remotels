//CLIENT

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>


int main()
{
    int sock;
    struct sockaddr_in addr;
    char buf[1024];
    int bytes_read;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(3425);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    char* folderName= NULL;
    size_t folderNameLength=0;
    size_t read;
    
    char* extension=NULL;
    size_t extensionLength=0;
    connect(sock, (struct sockaddr *)&addr, sizeof(addr));
    
    
    printf("Enter the path to destination folder:\n");
    read=getline(&folderName, &folderNameLength, stdin);
    if (read>0){
        folderName[read-1]=0;
    }
    
    printf("Enter desired extension:\n");
    read=getline(&extension, &extensionLength, stdin);
    if (read>0){
        extension[read-1]=0;
    }
    
    // sendto(sock, folderName, strlen(folderName), 0, (struct sockaddr*) &addr, sizeof(addr));
    // sendto(sock, extension, strlen(extension), 0, (struct sockaddr*) &addr, sizeof(addr));
    
    send(sock, folderName, strlen(folderName), 0);
    send(sock, extension, strlen(extension), 0);
    printf("List of files with extension %s for folder %s \n", extension, folderName);
    while(1){
        bytes_read = recvfrom(sock, buf, 1024, 0, NULL, NULL);
        buf[bytes_read] = '\0';
        printf("%s", buf);
        printf("\n");
    }
    
    free(folderName);
    free(extension);
    close(sock);
    return 0;
}

