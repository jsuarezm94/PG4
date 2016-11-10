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

int32_t fileSize(char * file_name) {

	FILE *fp;
	int32_t file_size = -1;
	if(fp = fopen(file_name, "r")) {
		fseek(fp,0,SEEK_END);
		file_size = ftell(fp);
		fseek(fp,0,SEEK_SET);
		fclose(fp);
	}
	return file_size;

} //end FILESIZE

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



/* READ BOARD */
void readBoard (int tcp_sock) {

	/* Declare variables */
	char board_name[100];		// Board name
	int board_len;			// Length of board name
	int server_board_size;		// Server response - board size
	int server_data_received;	// Current data received
	int total_data_received;	// Total data received
	char server_board[4096];	// Board contents

	/* Prompt user input */
	printf("Enter board name to read: ");
	scanf("%s", board_name);

	/* Send length of filename and filename */
	board_len = strlen(board_name);
	board_len = htonl(board_len);
	if (send(tcp_sock,&board_len,sizeof(board_len),0) == -1) {
		perror("ERROR: client-send()\n");
		exit(1);
	} //end send check
	if (send(tcp_sock,board_name,sizeof(board_name),0) == -1) {
		perror("ERROR: client-send()\n");
		exit(1);
	} //end send check

	/* Receive server confirmation in terms of file size */
	if (recv(tcp_sock,&server_board_size,sizeof(server_board_size),0) == -1) {
		perror("ERROR: client-recv()\n");
		exit(1);
	} // end recv check

	/* Interpret server response and output appropriate message to user */
	if (server_board_size < 0) {
		printf("Board '%s' does not exist\n", board_name);
		return;
	}

	/* If board exists, enter loop to receive board contents until total data received == server_board_size */
	total_data_received = 0;
	while (total_data_received < server_board_size) {
		if (server_data_received = recv(tcp_sock,server_board,sizeof(server_board),0) == -1) {
			total_data_received += server_data_received;
			printf("%s\n",server_board);
		} //end if
	} //end WHILE

} // end READ BOARD



/* APPEND FILE */
void append(int tcp_sock) {

	/* Declare variables */
	char board_name[100];           // Board name
	int board_len;                  // Length of board name
	char file_name[100];		// File name
	int file_len;			// Length of file name
	int server_response;		// Server confirmation
	int file_size,original_size;	// File size
	FILE *fp;
	char file_line[20000];
	int len, sent_len;

	/* Prompt user input */
	printf("Enter board name to read: ");
	scanf("%s",board_name);

	/* Send length of board name and board name */
	board_len = strlen(board_name);
	board_len = htonl(board_len);
	if (send(tcp_sock,&board_len,sizeof(board_len),0) == -1) {
		perror("ERROR: client-send()\n");
		exit(1);
	} //end send check
	if (send(tcp_sock,board_name,sizeof(board_name),0) == -1) {
		perror("ERROR: client-send()\n");
		exit(1);
	} //end send check

	/* Prompt user input */
	printf("Enter name of file to append: ");
	scanf("%s", file_name);

	/* Send length of filename and filename */
	file_len = strlen(file_name);
	file_len = htonl(file_len);
	if (send(tcp_sock,&file_len,sizeof(file_len),0) == -1) {
		perror("ERROR: client-send()\n");
		exit(1);
	} //end send check
	if (send(tcp_sock,file_name,sizeof(file_name),0) == -1) {
		perror("ERROR: client-send()\n");
		exit(1);
	} //end send check

	/* Recieve server confirmation */
	if (recv(tcp_sock,&server_response, sizeof(server_response),0) == -1) {
		perror("ERROR: client-recv()\n");
		exit(1);
	} //end recv check

	/* Interpret server confirmation and output appropriate message */
	/* Response = 1 -> board exists and file can be created */
	/* Response = -1 -> board does not exist */
	/* Response = -2 -> board exists, and attachment already exists on board */
	if (server_response == -1) {
		printf("Board does not exist\n");
		return;
	} else if (server_response == -2) {
		printf("Attachment already exists on board\n");
		return;
	} else {}

	/* Calculate file size and send it to the server */
	file_size = fileSize(file_name);
	original_size = file_size;
	file_size = htonl(file_size);

	if (send(tcp_sock,&file_size,sizeof(int32_t),0) == -1) {
		perror("ERROR: client-send()\n");
		exit(1);
	} //end send check

	if (original_size == -1) {
		printf("File does not exist locally\n");
		return;
	}

	/* Open file */
	fp = fopen(file_name, "r");
	memset(file_line, '\0', sizeof(file_line));

	/* Read file and send to server line by line */
	len = 0;
	sent_len = 0;
	while ( len=fread(file_line,sizeof(char),sizeof(file_line),fp) > 0) {
		sent_len = send(tcp_sock,file_line,len,0);
		memset(file_line, '\0', sizeof(file_line));
	} //end reading file
	fclose(fp);

} // end APPEND FILE



/* DOWNLOAD FILE */
void download (int tcp_sock) {

	/* Declare variables */
	char board_name[100];		// Board name
	int board_len;			// Length of board name
	char file_name[100];		// File name
	int file_len;			// Length of file name
	int server_response;		// Server confirmation
	int file_size;			// File size
	FILE *fp;
	char file_line[20000];
	int len, recv_len, recv_bytes, write_size;

	/* Prompt user input */
	printf("Enter board name to download from: ");
	scanf("%s",board_name);

	/* Send length of board name and board name */
	board_len = strlen(board_name);
	board_len = htonl(board_len);
	if (send(tcp_sock,&board_len,sizeof(board_len),0) == -1) {
		perror("ERROR: client-send()\n");
		exit(1);
	} //end send check
	if (send(tcp_sock,board_name,sizeof(board_name),0) == -1) {
		perror("ERROR: client-send()\n");
		exit(1);
	} //end send check

	/* Prompt user input */
	printf("Enter name of file to append: ");
	scanf("%s", file_name);

	/* Send length of filename and filename */
	file_len = strlen(file_name);
	file_len = htonl(file_len);
	if (send(tcp_sock,&file_len,sizeof(file_len),0) == -1) {
		perror("ERROR: client-send()\n");
		exit(1);
	} //end send check
	if (send(tcp_sock,file_name,sizeof(file_name),0) == -1) {
		perror("ERROR: client-send()\n");
		exit(1);
	} //end send check

	/* Recieve server confirmation */
	if (recv(tcp_sock,&server_response, sizeof(server_response),0) == -1) {
		perror("ERROR: client-recv()\n");
		exit(1);
	} //end recv check

	/* Interpret server confirmation and output appropriate message */
	/* Response = 1 -> board exists and file can be created */
	/* Response = -1 -> board does not exist */
	/* Response = -2 -> board exists, and file is not appended on board*/
	if (server_response == -1) {
		printf("Board does not exist\n");
		return;
	} else if (server_response == -2) {
		printf("Attachment already exists on board\n");
		return;
	} else {}

	file_size = server_response;
	
	/* Open file to write */
	fp = fopen(file_name, "w");
	memset(file_line, '\0', sizeof(file_line));
	
	len=0;
	recv_len=0;
	while (recv_len = recv(tcp_sock,file_line,sizeof(file_line),0) > 0) {
		recv_bytes += recv_len;
		write_size = fwrite(file_line, sizeof(char), recv_len, fp);
		memset(file_line, '\0', sizeof(file_line));
		// if (recv_bytes == file_size) break;
	} //end while
	fclose(fp);

} //end DOWNLOAD FILE




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
	scanf("%s", username);
	
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
	scanf("%s", password);

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
		scanf("%s", command);
/*
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
*/

		if(sendto(udp_sock,command,strlen(command),0,(struct sockaddr *)&sin, sizeof(struct sockaddr)) == -1) {
			perror("ERROR: client-sendto()\n");
			exit(1);
		} //end sendto check

		if (strcmp(command,"CRT") == 0) {
			create(udp_sock,sin);
		} else if (strcmp(command,"LIS") == 0) {
			list(udp_sock,sin);
		} else if (strcmp(command,"MSG") == 0) {
			message(udp_sock,sin);
		} else if (strcmp(command,"DLT") == 0) {
			delete(udp_sock,sin);
		} else if (strcmp(command,"RDB") == 0) {
			readBoard(tcp_sock);
		} else if (strcmp(command,"EDT") == 0) {
			edit(udp_sock,sin);
		} else if (strcmp(command,"APN") == 0) {
			append(tcp_sock);
		} else if (strcmp(command,"DWN") == 0) {
			download(tcp_sock);
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
