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
#define MAX_MD5LENGTH 50
#define MAX_FILENAME 100
#define MAX_PENDING 5
#define MAX_LINE 256


struct user {
	char name[100];
	char password[100];
};




int main (int argc, char * argv[]) {

   /* Declare variables */
   struct       sockaddr_in sin;        // Address structure
   int          addr_len;               // Length
   char         buf[10];                // Buffer for commands
//   int          sock, new_sock;         // Sockets
   int          opt = 1;
   int          port_num;               // Port number


	struct user users[100];
	int total_users = 0;


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




	/* --- Setting up TCP server ---  */
		
	
	int tcp_sock;
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
	
	
		while(1) {
			printf("Waiting for client operation \n");
		}		
	}

}

