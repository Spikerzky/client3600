#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;


    int random, i;
    int ticket;
    int tickets[7] = {0};
    char buffer[256];
    char command[256];

    int index = 0;//for tickets[]. Will be treating it like a stack, removing a ticket
		//from the "top" each time then decrimenting

    srand(time(NULL));

///setup start
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
///setup end



    for(i=0;i<7;i++){

    random = rand() % 10;

    if(random != 9){
	///buy
	///will send string "buy" to server
	///and recieve a string containing the generated ticket number
	bzero(command,256);
	sprintf(command,"buy");
	puts(command);
	n = write(sockfd,command,strlen(buffer));
	if(n < 0)
		error("ERROR writing to socket");
	bzero(buffer,256);
	n = read(sockfd,buffer,255);
	if(n < 0)
		error("ERROR reading from socket");
	sscanf(buffer,"%d",&ticket);

	tickets[index] = ticket;
	index++;

	printf("%d\n", tickets[index-1]);
    }

    else{
	///cancel
	///will send string "cancel #####" to server, using number from ticket array
	///and recieve some string in return
	bzero(command, 256);
	sprintf(command,"cancel %d", tickets[index-1]);
	tickets[index-1] = 0;
	index--;
	puts(command);
	n = write(sockfd,command,strlen(buffer));
	if(n < 0)
		error("ERROR writing to socket");
	bzero(buffer,256);
	n = read(sockfd,buffer,255);
	if(n < 0)
		error("ERROR reading from socket");
    }
/*
    bzero(buffer,256);
    fgets(buffer,255,stdin);
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) 
         error("ERROR writing to socket");
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0) 
         error("ERROR reading from socket");
//    printf("%s\n",buffer);
    newTick = atoi(buffer);
    printf("%d\n", newTick);
*/
    };//end of for loop
    close(sockfd);

	printf("%d\n",tickets[0]);
	printf("%d\n",tickets[1]);
	printf("%d\n",tickets[2]);

    return 0;
};
