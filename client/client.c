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

/* CREATE BOARD */
void create (int udp_sock, struct  sockaddr_in sin) {

	/* Declare variables */
	char board_name[100];		// Board name
	char server_resp[100];		// Server response
	int addr_len;

	memset(board_name, '\0', sizeof(board_name));
	memset(server_resp, '\0', sizeof(server_resp));

	/* Prompt user input */
	printf("Enter name of board to create: ");
	scanf("%s", board_name);

	/* Send board name to server */
	if(sendto(udp_sock,board_name,strlen(board_name),0,(struct sockaddr *)&sin, sizeof(struct sockaddr)) == -1) {
		perror("ERROR: client-sendto()\n");
		exit(1);
	} //end sendto check

	/* Receive server confirmation regarding creation of board and print results */
	if(recvfrom(udp_sock,server_resp,strlen(server_resp),0,(struct sockaddr *)&sin, &addr_len) == -1) {
		perror("ERROR: client-recvfrom()\n");
		exit(1);
	} //end recvfrom check

	if (strcmp(server_resp,"Yes") == 0) {
		printf("Success: Board %s has been created\n", board_name);
	} else {
		printf("Failure: Board %s has not been created\n", board_name);
	}

} // end CREATE BOARD



/* LEAVE MESSAGE */
void message (int udp_sock, struct sockaddr_in sin) {

	/* Declare variables */
	char board_name[100];		// Board name
	char message[4096];		// Message to be added
	char server_resp[100];		// Server response
	int addr_len;

	memset(board_name, '\0', sizeof(board_name));
	memset(message, '\0', sizeof(message));
	memset(server_resp, '\0', sizeof(server_resp));

	/* Prompt user input */
	printf("Enter name of board to add message: ");
	scanf("%s", board_name);

	/* Send board name to server */
	if(sendto(udp_sock,board_name,strlen(board_name),0,(struct sockaddr *)&sin, sizeof(struct sockaddr)) == -1) {
		perror("ERROR: client-sendto()\n");
		exit(1);
	} //end sendto check

	/* Prompt user input */
	printf("Enter message: ");
	scanf("%s", message);

	/* Send message to server */
	if(sendto(udp_sock,message,strlen(message),0,(struct sockaddr *)&sin, sizeof(struct sockaddr)) == -1) {
		perror("ERROR: client-sendto()\n");
		exit(1);
	} //end sendto check

	/* Receive server confirmation regarding addition of message and print results */
	if(recvfrom(udp_sock,server_resp,strlen(server_resp),0,(struct sockaddr *)&sin, &addr_len) == -1) {
		perror("ERROR: client-recvfrom()\n");
		exit(1);
	} //end recvfrom check

	if (strcmp(server_resp,"Yes") == 0) {
		printf("Success: Message '%s' has been added to Board %s\n", message, board_name);
	} else {
		printf("Failure: Message '%s' was not added to Board %s\n", message, board_name);
	}

} //end LEAVE MESSAGE



/* DELETE MESSAGE */
void delete (int udp_sock, struct sockaddr_in sin) {

	/* Declare variables */
	char board_name[100];           // Board name
	char message[4096];             // Message to be deleted
	//int message2;			// Message to be deleted
	char server_resp[100];          // Server response
	int addr_len;

	memset(board_name, '\0', sizeof(board_name));
	memset(message, '\0', sizeof(message));
	memset(server_resp, '\0', sizeof(server_resp));

	/* Prompt user input */
	printf("Enter name of board to delete message: ");
	scanf("%s", board_name);

	/* Send board name to server */
	if(sendto(udp_sock,board_name,strlen(board_name),0,(struct sockaddr *)&sin, sizeof(struct sockaddr)) == -1) {
		perror("ERROR: client-sendto()\n");
		exit(1);
	} //end sendto check

	/* Prompt user input */
	printf("Enter message to be deleted: ");
	scanf("%s", message);

	/* Send message to server */
	if(sendto(udp_sock,message,strlen(message),0,(struct sockaddr *)&sin, sizeof(struct sockaddr)) == -1) {
		perror("ERROR: client-sendto()\n");
		exit(1);
	} //end sendto check

	/* Receive server confirmation regarding deletion of message and print results */
	if(recvfrom(udp_sock,server_resp,strlen(server_resp),0,(struct sockaddr *)&sin, &addr_len) == -1) {
		perror("ERROR: client-recvfrom()\n");
		exit(1);
	} //end recvfrom check

	if (strcmp(server_resp,"Yes") == 0) {
		printf("Success: Message '%s' has been deleted from Board %s\n", message, board_name);
	} else {
		printf("Failure: Message '%s' was not deleted from Board %s\n", message, board_name);
	}

} //end DELETE MESSAGE



/* EDIT MESSAGE */
void edit (int udp_sock, struct sockaddr_in sin) {

	/* Declare variables */
	char board_name[100];           // Board name
	char message[4096];             // Message to be replaced
	char new_message[4096];		// New message
	char server_resp[100];          // Server response
	int addr_len;

	memset(board_name, '\0', sizeof(board_name));
	memset(message, '\0', sizeof(message));
	memset(new_message, '\0', sizeof(new_message));
	memset(server_resp, '\0', sizeof(server_resp));

	/* Prompt user input */
	printf("Enter name of board to replace message: ");
	scanf("%s", board_name);

	/* Send board name to server */
	if(sendto(udp_sock,board_name,strlen(board_name),0,(struct sockaddr *)&sin, sizeof(struct sockaddr)) == -1) {
		perror("ERROR: client-sendto()\n");
		exit(1);
	} //end sendto Check

	/* Prompt user input */
	printf("Enter message to be replaced: ");
	scanf("%s", message);

	/* Send message to be replaced to server */
	if(sendto(udp_sock,message,strlen(message),0,(struct sockaddr *)&sin, sizeof(struct sockaddr)) == -1) {
		perror("ERROR: client-sendto()\n");
		exit(1);
	} //end sendto check

	/* Prompt user input */
	printf("Enter new message: ");
	scanf("%s", new_message);

	/* Send new message to server */
	if(sendto(udp_sock,new_message,strlen(new_message),0,(struct sockaddr *)&sin, sizeof(struct sockaddr)) == -1) {
		perror("ERROR: client-sendto()\n");
		exit(1);
	} //end sendto check

	/* Receive server confirmation regarding replacement of message and print results */
	if(recvfrom(udp_sock,server_resp,strlen(server_resp),0,(struct sockaddr *)&sin, &addr_len) == -1) {
		perror("ERROR: client-recvfrom()\n");
		exit(1);
	} // end recvfrom check

	if (strcmp(server_resp,"Yes") == 0) {
		printf("Success: Message '%s' has been replaced in Board %s\n", message, board_name);
	} else {
		printf("Failure: Message '%s' was not replaced in Board %s\n", message, board_name);
	}

} //end EDIT MESSAGE



/* LIST BOARDS */
void list (int udp_sock, struct sockaddr_in sin) {

	/* Declare variables */
	char board_names[4096];		// List of boards
	int addr_len;

	memset(board_names, '\0', sizeof(board_names));

	/* Receive board listing from server and print results */
	if(recvfrom(udp_sock,board_names,strlen(board_names),0,(struct sockaddr *)&sin,&addr_len) == -1) {
		perror("ERROR: client-recvfrom()\n");
		exit(1);
	} //end recvfrom check

	if (strlen(board_names) == 0) {
		printf("No boards exist\n");
	} else {
		printf("%s\n", board_names);
	}

} //end LIST BOARDS



/* DESTROY BOARD */
void destroy (int udp_sock, struct sockaddr_in sin) {

	/* Declare variables */
	char board_name[100];           // Board name
	char server_resp[100];          // Server response
	int addr_len;

	memset(board_name, '\0', sizeof(board_name));
	memset(server_resp, '\0', sizeof(server_resp));

	/* Prompt user input */
	printf("Enter name of board to destroy: ");
	scanf("%s", board_name);

	/* Send board name to server */
	if(sendto(udp_sock,board_name,strlen(board_name),0,(struct sockaddr *)&sin, sizeof(struct sockaddr)) == -1) {
		perror("ERROR: client-sendto()\n");
		exit(1);
	} //end sendto check

	/* Receive server confirmation regarding deletion of board and print results */
	if(recvfrom(udp_sock,server_resp,strlen(server_resp),0,(struct sockaddr *)&sin, &addr_len) == -1) {
		perror("ERROR: client-recvfrom()\n");
		exit(1);
	} //end recvfrom check

	if (strcmp(server_resp,"Yes") == 0) {
		printf("Success: Board %s has been destroyed\n", board_name);
	} else {
		printf("Failure: Board %s has not been destroyed\n", board_name);
	}

} //end DESTROY BOARD



/* SHUTDOWN SERVER */
int shutdown_server (int udp_sock, struct sockaddr_in sin) {

	/* Declare variables */
	char password[100];		// Admin password
	char server_resp[100];		// Server response
	int addr_len;
	int shutdown_ind=0;

	memset(password, '\0', sizeof(password));
	memset(server_resp, '\0', sizeof(server_resp));

	/* Prompt user input */
	printf("Enter admin password to server: ");
	scanf("%s", password);

	/* Send password to server */
	if(sendto(udp_sock,password,strlen(password),0,(struct sockaddr *)&sin, sizeof(struct sockaddr)) == -1) {
		perror("ERROR: cleint-sendto()\n");
		exit(1);
	} //end sendto check

	/* Receive server confirmation regarding admin password */
	if(recvfrom(udp_sock,server_resp,strlen(server_resp),0,(struct sockaddr *)&sin, &addr_len) == -1) {
		perror("ERROR: client-recvfrom()\n");
		exit(1);
	} //end recvfrom check

	if (strcmp(server_resp, "Yes") == 0) {
		printf("Success: correct admin password\n");
		shutdown_ind=1;
	} else {
		printf("Failure: incorrect admin password\n");
		shutdown_ind=0;
	}

	return shutdown_ind;

} //end SHUTDOWN SERVER








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
	int	shutdown_ind;		// Shutdown indicator

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
			create(udp_sock,sin);
		} else if (strcmp(command,"LIS") == 0) {
			list(udp_sock,sin);
		} else if (strcmp(command,"MSG") == 0) {
			message(udp_sock,sin);
		} else if (strcmp(command,"DLT") == 0) {
			delete(udp_sock,sin);
		} else if (strcmp(command,"RDB") == 0) {
			//read(tcp_sock);
		} else if (strcmp(command,"EDT") == 0) {
			edit(udp_sock,sin);
		} else if (strcmp(command,"APN") == 0) {
			//append(tcp_sock);
		} else if (strcmp(command,"DWN") == 0) {
			//download(tcp_sock);
		} else if (strcmp(command,"DST") == 0) {
			destroy(udp_sock,sin);
		} else if (strcmp(command,"XIT") == 0) {
			close(udp_sock);
			close(tcp_sock);
			printf("Session has been closed\n");
			exit_loop = 0;
		} else if (strcmp(command,"SHT") == 0) {
			shutdown_ind = shutdown_server(udp_sock,sin);
			if (shutdown_ind) {
				close(udp_sock);
				close(tcp_sock);
				printf("Server is shutting down\n");
				exit_loop = 0;
			}
		} else {
			printf("Incorrect command\n");
		}

	} // end while loop

	return 0;
}
