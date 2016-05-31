//CLIENT

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

bool checkExtension(const char* name, const char* extension){
	size_t extensionLength=strlen(extension);
	size_t filenameLength=strlen(name);
	return filenameLength>=extensionLength && strcmp(name+filenameLength-extensionLength, extension) == 0;	
}

int main()
{
    int sock;
    struct sockaddr_in addr;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(3425);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
	

	DIR* d;
	struct dirent* dir;
	
	char* folderName= NULL;
	size_t folderNameLength=0;
	size_t read;
	
	char* extension=NULL;
	size_t extensionLength=0;
	
	printf("Enter the path to destination folder:\n");
	read=getline(&folderName, &folderNameLength, stdin);
	if (read>0){
		folderName[read-1]=0;
		//printf("The folderName is: %s, read %i symbols \n", folderName, read-1);
	}
	
	printf("Enter desired extension:\n");
	read=getline(&extension, &extensionLength, stdin);
	if (read>0){
		extension[read-1]=0;
		//printf("Extension is: %s, read %i symbols \n", extension, read-1);
	}
	
	connect(sock, (struct sockaddr *)&addr, sizeof(addr));
	
	d=opendir(folderName);
	if (d){
		while ((dir=readdir(d)) != NULL)
		{

			if (checkExtension(dir->d_name, extension)){
				send(sock, dir->d_name, strlen(dir->d_name), 0);
			}
			
		}
		closedir(d);
	}

	free(folderName);
	free(extension);
    close(sock);
    return 0;
}

