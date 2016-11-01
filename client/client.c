// Computer Networks - CSE 30264
// Programming Assignment 4
// October 31, 2016
// Authors: Jorge Diaz & Jose Suarez
// NETIDs: jdiazort & jsuarezm
// Client

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <openssl/md5.h>

int main (int argc, char * argv[]) {

	/* Declare variables */
	FILE 	*fn;			// File handle for file
	struct	hostent *host;		// Host
	struct	sockaddr_in sin;	// Address struct used for communication
	char	*hp;			// Host
	int	udp_sock;		// UDP - File handle for socket
	int	tcp_sock;		// TCP - File handle for socket
	int	addr_len;		// Address length
	int	port_num;		// Port number

	char	username[150];		// Username
	char	server_username[150];	// Username prompt
	char	password[150];		// Password
	char	server_password[150];	// Password prompt
	size_t	user_ack=0;		// User ack from server

	int	exit_loop=1;		// While loop variable

	/* Check arguments in command line */
	if (argc!=3) {
		fprintf(stderr,"USAGE ERROR: %s <server ip> <port number>\n",argv[0]);
		exit(1);
	} // end arguments check

	/* Assign command-line arguments and check for errors */
	/* Host - argv1 */
	/* Translate host name into peer's IP address */
	host = gethostbyname(argv[1]);
	if (!host) {
		printf("ERROR: host name/unknown host\n");
		exit(1);
	} // end hostname check

	/* Port number - argv2 */
	port_num = atoi(argv[2]);

	/* Build address data structure */
	bzero((char *)&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	bcopy(host->h_addr, (char *)&sin.sin_addr, host->h_length);
	sin.sin_port = htons(port_num);

	/* -------- UDP -------- */
	/* UDP - Open socket and check for errors */
	if((udp_sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("ERROR: client-socket()\n");
		exit(1);
	} // end socket check

	/* -------- TCP -------- */
	/* TCP - Open socket and check for errors */
	if((tcp_sock = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
		perror("ERROR: client-socket()\n");
		exit(1);
	} // end socket check

	/* TCP - Connect client and server */
	if (connect(tcp_sock, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
		perror("ERROR: client-connect()\n");
		close(tcp_sock);
		exit(1);
	} // end connect check
	
	/* -------- USERNAME -------- */
	/* Receive request for username from server */
	if(recv(tcp_sock, server_username, sizeof(server_username), 0) == -1) {
		perror("ERROR: client-recv()\n");
		exit(1);
	} // end recv check

	/* Prompt user for username and send it to the server */
	printf("%s", server_username);
	scanf("%s", &username);
	
	if(send(tcp_sock, username, sizeof(username), 0) == -1) {
		perror("ERROR: client-send()\n");
		exit(1);
	} // end send check

	/* -------- PASSWORD -------- */
	/* Receive request for password from server */
	if(recv(tcp_sock, server_password, sizeof(server_password), 0) == -1) {
		perror("ERROR: client-recv()\n");
		exit(1);
	} // end recv check

	/* Prompt user for password and send it to the server */
	printf("%s", server_password);
	scanf("%s", &password);

	if(send(tcp_sock, password, sizeof(password), 0) == -1) {
		perror("ERROR: client-send()\n");
		exit(1);
	} // end send check

	/* -------- SERVER ACKNOWLEDGEMENT -------- */
	if(recv(tcp_sock, &user_ack, sizeof(int), 0) == -1) {
		perror("ERROR: client-recv()\n");
		exit(1);
	} // end recv check

	if(user_ack == 0) {
		printf("ERROR: User authentication/registration failed\n");
		exit(1);
	}

	/* -------- PROMPT USER FOR OPERATION -------- */
	while (exit_loop) {
		char command[10];
		printf("Enter command: CRT, LIS, MSG, DLT, RDB, EDT, APN, DWN, DST, XIT, SHT\n");
		scanf("%s", &command);

		if (strcmp(command,"RDB")==0 || strcmp(command,"APN")==0 || strcmp(command,"DWN")==0) {
			if(send(tcp_sock,command,sizeof(command), 0) == -1) {
				perror("ERROR: client-send()\n");
				exit(1);
			} // end send check
		} else {
			if(sendto(udp_sock,command,strlen(command),0,(struct sockaddr *)&sin, sizeof(struct sockaddr)) == -1) {
				perror("ERROR: client-sendto()\n");
				exit(1);
			} // end send check
		} // end IF

		if (strcmp(command,"CRT") == 0) {
			//create(udp_sock)
		} else if (strcmp(command,"LIS") == 0) {
			//list(udp_sock);
		} else if (strcmp(command,"MSG") == 0) {
			//message(udp_sock);
		} else if (strcmp(command,"DLT") == 0) {
			//delete(udp_sock);
		} else if (strcmp(command,"RDB") == 0) {
			//read(tcp_sock);
		} else if (strcmp(command,"EDT") == 0) {
			//edit(udp_sock);
		} else if (strcmp(command,"APN") == 0) {
			//append(tcp_sock);
		} else if (strcmp(command,"DWN") == 0) {
			//download(tcp_sock);
		} else if (strcmp(command,"DST") == 0) {
			//destroy(udp_sock);
		} else if (strcmp(command,"XIT") == 0) {
			//exit(udp_sock);
		} else if (strcmp(command,"SHT") == 0) {
			//shutdown(udp_sock);
		} else {
			printf("Incorrect command\n");
		}

	} // end while loop
}
