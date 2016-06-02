//SERVER

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdbool.h>
#include <dirent.h>
#include <string.h>
#define MAX_LENGTH 1024

bool checkExtension(const char* name, const char* extension){
    size_t extensionLength=strlen(extension);
    size_t filenameLength=strlen(name);
    return filenameLength>=extensionLength && strcmp(name+filenameLength-extensionLength, extension) == 0;
}

int main()
{
    int sock;
    struct sockaddr_in server_addr, from_addr;
    unsigned int faLen=sizeof(from_addr);
    int bytes_read;
    
    
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
    {
        perror("socket");
        return 1;
    }
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(3425);
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    
    // addr.sin_family=AF_INET;
    // addr.sin_port = htons(3426);
    // addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    if(bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind");
        return 2;
    }
    
    DIR* d;
    struct dirent* dir;
    
    char folderName[MAX_LENGTH];
    char extension[MAX_LENGTH];
    
    printf("Server started. Waiting for requests.\n");

    while (1){
        memset(folderName, 0, sizeof(folderName));
        memset(extension, 0, sizeof(extension));
        bytes_read = recvfrom(sock, folderName, MAX_LENGTH, 0, (struct sockaddr *)&from_addr, &faLen);
        bytes_read = recvfrom(sock, extension, MAX_LENGTH, 0, (struct sockaddr *)&from_addr, &faLen);
        
        printf("Received request. Foldername: %s, extension: %s \n", folderName, extension);
        
        d=opendir(folderName);
        if (d){
            while ((dir=readdir(d)) != NULL)
            {
                
                if (checkExtension(dir->d_name, extension)){
                    sendto(sock, dir->d_name, strlen(dir->d_name), 0, (struct sockaddr*) &from_addr, faLen);
                   // printf("%s\n", dir->d_name);
                }
                
            }
            closedir(d);
        }
        
    }

    
    
    
    return 0;
}

