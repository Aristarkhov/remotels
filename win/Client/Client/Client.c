/*
Simple udp client
Silver Moon (m00n.silv3r@gmail.com)
*/
#include<stdio.h>
#include<winsock2.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define SERVER "127.0.0.1"  //ip address of udp server
#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to listen for incoming data
#define AMAX_LENGTH 100

char* getNameAndExtension() {
	const char* mask = "*.";
	char namebuf[AMAX_LENGTH] = { 0 };
	printf("Enter the desired path with the disk letter and double backslashes as folder delimiters:\n");
	fgets(namebuf, AMAX_LENGTH, stdin);
	char extbuf[AMAX_LENGTH] = { 0 };
	printf("Enter the desired extension:\n");
	fgets(extbuf, AMAX_LENGTH, stdin);


	char* resultPath = malloc((strlen(namebuf) - 1 + strlen(extbuf) - 1 + 5)*sizeof(char));
	strncpy(resultPath, namebuf, strlen(namebuf) - 1);
	strncpy(resultPath + (strlen(namebuf) - 1), mask, strlen(mask));
	strncpy(resultPath + (strlen(namebuf) - 1) + strlen(mask), extbuf, strlen(extbuf) - 1);
	resultPath[strlen(namebuf) - 1 + strlen(extbuf) - 1 + strlen(mask)] = 0;
	return resultPath;
}


int main(void)
{
	struct sockaddr_in si_other;
	int s, slen = sizeof(si_other);
	char buf[BUFLEN];
	char message[BUFLEN];
	WSADATA wsa;

	//Initialise winsock
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	printf("Initialised.\n");

	//create socket
	if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
	{
		printf("socket() failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	//setup address structure
	memset((char *)&si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(PORT);
	si_other.sin_addr.S_un.S_addr = inet_addr(SERVER);

	//start communication

	char* path = getNameAndExtension();
	if (sendto(s, path, strlen(path), 0, (struct sockaddr *) &si_other, slen) == SOCKET_ERROR)
	{
		printf("sendto() failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	printf("Response:\n");
	while (1)
	{
		//receive a reply and print it
		//clear the buffer by filling null, it might have previously received data
		memset(buf, '\0', BUFLEN);
		//try to receive some data, this is a blocking call
		if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == SOCKET_ERROR)
		{
			printf("recvfrom() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}
		puts(buf);
	}

	closesocket(s);
	WSACleanup();

	return 0;
}
