// Computer Networks - CSE 30264
// Programming Assignment 4
// October 31, 2016
// Authors: Jorge Diaz & Jose Suarez
// NETIDs: jdiazort & jsuarezm
// Server




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
#include <sys/time.h>
#include <openssl/md5.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>



#define MAX_MD5LENGTH 50
#define MAX_FILENAME 100
#define MAX_PENDING 5
#define MAX_LINE 256


struct user {
	char name[100];
	char password[100];
};

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
void create(int udp_sock, struct  sockaddr_in sin, char * user_name) {

	printf("inside CREATE\n");
		
	/* Declare Variables  */
	char board_name[100];
	char server_resp[100];
	char board_head[200];	

	FILE *fp;

	int addr_len;

	memset(board_name, '\0', sizeof(board_name));
        memset(server_resp, '\0', sizeof(server_resp));
        memset(board_head, '\0', sizeof(board_head));

	
        /* Receive board_name of board to create from the client */
        if(recvfrom(udp_sock,board_name,sizeof(board_name),0,(struct sockaddr *)&sin, &addr_len) == -1) {
                perror("ERROR: client-recvfrom()\n");
                exit(1);
        } //end recvfrom check

	printf("board name = %s\n", board_name);

        strcpy(board_head, user_name);
        strcat(board_head, " ");
        strcat(board_head, board_name);
	
	printf("board_head = %s\n", board_head);
	
	size_t wf;
	if (fp = fopen(board_name,"ab+" )) {
		wf = fwrite(board_head, 1, sizeof(board_head), fp );
		fclose(fp);
		printf("Number of bytes written = %i\n", wf);	
	}

	strcpy(server_resp, "Yes" );

	if (sendto (udp_sock, server_resp, strlen(server_resp), 0, (struct sockaddr *)&sin, sizeof(struct sockaddr) ) == -1 ) {
                perror("ERROR: server-sendto()\n");

	}

	printf("End of create\n");


}


void message(int udp_sock, struct  sockaddr_in sin, char * user_name) {

	/* Declare cariables */
	char board_name[100];
	char message[4096];
	char server_resp[100];
        int addr_len;

	printf("inside message\n");	

        memset(board_name, '\0', sizeof(board_name));
        memset(message, '\0', sizeof(message));
        memset(server_resp, '\0', sizeof(server_resp));

        /* Receive board_name of board to create from the client */
        if(recvfrom(udp_sock,board_name,sizeof(board_name),0,(struct sockaddr *)&sin, &addr_len) == -1) {
               	perror("ERROR: client-recvfrom()\n");
                exit(1);
        } //end recvfrom check

        /* Receive message to post on board */
        if(recvfrom(udp_sock,message,sizeof(message),0,(struct sockaddr *)&sin, &addr_len) == -1) {
                perror("ERROR: client-recvfrom()\n");
                exit(1);
        } //end recvfrom check

	printf("board_name: %s\n", board_name);
	printf("message: %s\n", message);

	/*
 	bool found = false;
	for( size_t i=0; i<num_boards; i++) {
		if (boards[i] == board_name) {
			
		}
	}  

	if (found) {
		post message
		if success
			strcpy(server_resp, "Yes");
		else 
			strpy(server_resp, "No");
	}
	else {
		strpcy(server_resp, "No);
	}
	
	*/
	
	strcpy(server_resp,"No");

	if(sendto(udp_sock,board_name,strlen(board_name),0,(struct sockaddr *)&sin, sizeof(struct sockaddr)) == -1) {
                perror("ERROR: client-sendto()\n");
                exit(1);
        } //end sendto check

}


void delete (int udp_sock, struct  sockaddr_in sin, char * user_name) {
	
	/* Declare Variables */
	char board_name[100];		
	char message[4096];
        char server_resp[100];
        int addr_len;


        memset(board_name, '\0', sizeof(board_name));
        memset(message, '\0', sizeof(message));
        memset(server_resp, '\0', sizeof(server_resp));

	/* Receive board_name of boad to delete */
        if(recvfrom(udp_sock,board_name,strlen(board_name),0,(struct sockaddr *)&sin, &addr_len) == -1) {
                perror("ERROR: client-recvfrom()\n");
                exit(1);
        } //end recvfrom check
	
	/* Receive message to be deleted */
        if(recvfrom(udp_sock,message,strlen(message),0,(struct sockaddr *)&sin, &addr_len) == -1) {
                perror("ERROR: client-recvfrom()\n");
                exit(1);
        } //end recvfrom check
	
	/* PERFORM DELETION*/




	/* END DELETION */	

        strcpy(server_resp,"No");
        if(sendto(udp_sock,board_name,strlen(board_name),0,(struct sockaddr *)&sin, sizeof(struct sockaddr)) == -1) {
                perror("ERROR: client-sendto()\n");
                exit(1);
        } //end sendto check


}



void edit (int udp_sock, struct  sockaddr_in sin, char * user_name) {

        /* Declare Variables */
        char board_name[100];
        char message[4096];
	char new_message[4096];
        char server_resp[100];
        int addr_len;


        memset(board_name, '\0', sizeof(board_name));
        memset(message, '\0', sizeof(message));
        memset(server_resp, '\0', sizeof(server_resp));

        /* Receive board_name of boad to delete */
        if(recvfrom(udp_sock,board_name,strlen(board_name),0,(struct sockaddr *)&sin, &addr_len) == -1) {
                perror("ERROR: client-recvfrom()\n");
                exit(1);
        } //end recvfrom check

        /* Receive message to be edited */
        if(recvfrom(udp_sock,message,strlen(message),0,(struct sockaddr *)&sin, &addr_len) == -1) {
                perror("ERROR: client-recvfrom()\n");
                exit(1);
        } //end recvfrom check

	/* Receive edited message*/
        if(recvfrom(udp_sock,new_message,strlen(new_message),0,(struct sockaddr *)&sin, &addr_len) == -1) {
                perror("ERROR: client-recvfrom()\n");
                exit(1);
	}

        /* PERFORM REPLACEMENT */




        /* END DELETION */

        strcpy(server_resp,"No");
        if(sendto(udp_sock,server_resp,strlen(server_resp),0,(struct sockaddr *)&sin, sizeof(struct sockaddr)) == -1) {
                perror("ERROR: client-sendto()\n");
                exit(1);
        } //end sendto check


}


void list (int udp_sock, struct  sockaddr_in sin, char * user_name) {
	
        /* Declare Variables */
	char server_response[100];
	char board_names[4096];
        int addr_len;


        memset(board_names, '\0', sizeof(board_names));

	/* GET LIST OF ALL THE BOARDS*/



	/* */

	if(sendto(udp_sock,board_names,strlen(board_names),0,(struct sockaddr *)&sin, sizeof(struct sockaddr)) == -1) {
                perror("ERROR: client-sendto()\n");
                exit(1);
        } //end sendto chec
}

/*
void readBoard(int udp_sock, struct  so) {

        char board_name[100];
        char message[4096];
        char server_resp[100];

        memset(board_name, '\0', sizeof(board_name));
        memset(message, '\0', sizeof(message));
        memset(server_resp, '\0', sizeof(server_resp));

}
*/


int main (int argc, char * argv[]) {

	/* Declare variables */
	struct       sockaddr_in 	sin, 	client_addr;        // Address structure
	int          addr_len;               // Length

        int     udp_sock;               // UDP - File handle for socket
        int     tcp_sock;               // TCP - File handle for socket

	char         buf[10];                // Buffer for commands
	//   int          sock, new_sock;         // Sockets
	int          opt = 1;
	int          port_num;               // Port number

	struct user users[100];
	int total_users = 0;

        int     exit_loop=1;            // While loop variable
        int     shutdown_ind;           // Shutdown indicator




   	/* Check arguments in command line */
   	if (argc!=2) {
      		fprintf(stderr,"USAGE ERROR: %s <port number>\n",argv[0]);
      		exit(1);
   	} // end arguments check

	/* Assign command-line arguments and check for errors*/
   	/* Port number - argv1 */
  	port_num = atoi(argv[1]);

   	/* Build address data structure */
   	bzero((char *)&sin, sizeof(sin));
   	sin.sin_family = AF_INET;
        printf("HERE 1\n");
   	sin.sin_addr.s_addr = INADDR_ANY;
   	sin.sin_port = htons(port_num);

	/* --- Setting up the UDP server --- */
        if((udp_sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
                perror("ERROR: client-socket()\n");
                exit(1);
        } // end socket check

        if((bind(udp_sock,(struct sockaddr*)&sin, sizeof(sin))) < 0){
                perror("simplex-talk: bind");
                exit(1);
        }


	/* --- Setting up TCP server ---  */
   	/* Create a socket on the server side */
   	if ((tcp_sock = socket(PF_INET, SOCK_STREAM,0)) < 0) {
      		perror("ERROR: server-socket()\n");
      		exit(1);
   	} // end socket check

   	/* Set socket options */
   	if ((setsockopt(tcp_sock, SOL_SOCKET, SO_REUSEADDR, (char *)& opt, sizeof(int))) < 0) {
      		perror("ERROR: server-setsockopt()\n");
      		exit(1);
   	} //end socket options check



   	/* Bind the created socket to the specified address */
   	if ((bind(tcp_sock,(struct sockaddr *)&sin, sizeof(sin))) < 0) {
      		perror("ERROR: server-bind()\n");
      		exit(1);
   	} //end bind check

	while(1) {
		printf("Waiting for clien conection.\n");
                if ((listen(tcp_sock, MAX_PENDING)) < 0) {
                        perror("ERROR: server-listen()\n");
                        exit(1);
                } //end listen check
		printf("HERE 1\n");
                if ((tcp_sock = accept(tcp_sock, (struct sockaddr *)&sin, &addr_len)) < 0) {
                        perror("ERROR: server-accept()\n");
                        exit(1);
                } //end accept check

		printf("HERE 2 \n");
		char request_username[150];
		memset(request_username,'\0',sizeof(request_username));
		snprintf(request_username,sizeof(request_username),"Please enter your username: ");
		if (send(tcp_sock, request_username, sizeof(request_username),0) == -1) {
			perror("ERROR: server-send()\n");
		}
		printf("HERE 3 \n");
		char username[150];
		if (recv(tcp_sock, username, sizeof(username), 0) == -1 ) {
			perror("ERROR: server-recv()\n");
		}
		printf("USERNAME: %s\n", username);
		
		int user_exists = -1;		
		int i;
		for (i=0; i<100 && i<total_users; i++) {
			printf("HERE 4.1\n");
			if (strcmp(users[i].name,username) ) {
				printf("HERE 4.2\n");
				user_exists = 1;
			}
		}
	
		char request_password[150];
		memset(request_password, '\0', sizeof(request_password));
		snprintf(request_password, sizeof(request_password), "Please enter your password: ");
		if (send(tcp_sock, request_password, sizeof(request_password), 0) == -1) {
			perror("ERROR: server-send()");
		}

		char password[150];
		if (recv(tcp_sock, password, sizeof(password), 0) == -1 ) {
			perror("ERROR: server-recv()\n");
		}
		printf("PASSWORD: %s", password);
		int password_match = -1;
		if (user_exists != -1) {
			int j;
			for (j=0; j<100 && j<total_users; j++) {
            			if ( strcmp (users[j].password,password)) {
               				password_match = 1;
            			}
      			}
		} else {
			total_users++;
			struct user u;
			strcpy(u.name, username);
			strcpy(u.password, password);
			users[total_users] = u;
			printf("HERE 4\n");
			password_match = 1;
		}
		// Fail to authenticate user
		size_t ACK = 1;
		if (password_match == -1) {
			ACK = 0;
			printf("HERE 5\n");
			printf("User Authentication Failed.\n");
		} 
		if (send(tcp_sock, &ACK, sizeof(size_t), 0) == -1 ) {
			perror("ERROR: server-send()");
			exit(1);
		}
		int connected = 1;
		char protocol[10];
		int bytes_buf;
		
		addr_len = sizeof(client_addr);

		/* --------- WATING FOR USER OPERATION  --------- */
		while(connected) {
			memset(buf, '\0', sizeof(buf));
			printf("Waiting for operation from client.\n");
			if (bytes_buf = recvfrom(udp_sock, buf,sizeof(buf),0,(struct sockaddr *)&sin, &addr_len) == -1) {
               			perror("ERROR: client-recvfrom()\n");
                		exit(1);
        		} //end recvfrom check
			printf("Received: %s, %i bytes\n", buf, bytes_buf);


                        if (strcmp(buf,"CRT")==0){
				printf("Calling CREATE\n");
                        	create(udp_sock, sin, username);
                        }
                        else if (strcmp(buf,"MSG")==0){
                                message(udp_sock, sin, username);
                        }
                        else if (strcmp(buf,"LIS")==0){
                                //list(sock);
                        }
                        else if (strcmp(buf,"DEL")==0){
                                //makeDirectory(sock);
                        }
                        else if (strcmp(buf, "RDB")==0){
                                //removeDirectory(sock);
                        }
                        else if (strcmp(buf, "EDT")==0){
                                //changeDirectory(sock);
                        }
                        else if (strcmp(buf,"APN")==0){
                                //delete(sock);
                        }
			else if (strcmp(buf,"DWN")==0) {
				//dwonload
			}
			else if (strcmp(buf,"DST")==0) {
				// destoy
			}
			else if (strcmp(buf,"SHT")==0) {
				//shutdown
			}
                        else if (strcmp(buf,"XIT")==0){
                                connected = 0;
                                close(tcp_sock);
                                printf("Client has ended session..\n");
			}
			memset(buf, '\0', sizeof(buf));
			printf("Here 6\n");
		}	
		break;	

	}

}

