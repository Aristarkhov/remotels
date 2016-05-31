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
    struct sockaddr_in server_addr, addr;
    int bytes_read;
    int dlen=sizeof(addr);

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(3425);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind");
        exit(2);
    }

    DIR* d;
    struct dirent* dir;

    char folderName[MAX_LENGTH];
    char extension[MAX_LENGTH];
    bytes_read = recvfrom(sock, folderName, 1024, 0, NULL, NULL);
    printf("folderName\n");
    printf(folderName);
    bytes_read = recvfrom(sock, extension, 1024, 0, NULL, NULL);
    printf("extension\n");
    printf(extension);

    d=opendir(folderName);
    if (d){
        while ((dir=readdir(d)) != NULL)
        {

            if (checkExtension(dir->d_name, extension)){
                sendto(sock, dir->d_name, strlen(dir->d_name), 0, (struct sockaddr*) &addr, dlen);
                printf("%s\n", dir->d_name);
            }
            
        }
        closedir(d);
    }




    
    return 0;
}

