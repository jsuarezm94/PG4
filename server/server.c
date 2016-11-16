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

void deleteAll() {

        int ret;
        DIR *dir_stream;
        char list_buf[100];
        struct dirent *dir_read;
        ssize_t read;

        dir_stream = opendir("./");
        if (dir_stream != NULL) {
                while ( dir_read = readdir(dir_stream) ) {
                        ret = unlink(dir_read->d_name);
                }
        }

}

void sendFile(int tcp_sock, char * file_name) {

        int file_size;
        int original_size;
        FILE *fp;
        char file_line[20000];
        int len, sent_len;



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

}




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
		wf = fwrite(board_head, 1, strlen(board_head), fp );
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
	int board_exists;
	FILE *fp;
	
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

	printf("user name %s\n", user_name);
	printf("board_name: %s\n", board_name);
	printf("message: %s\n", message);


	if ( (board_exists = access(board_name, F_OK)) != -1 ) {
		size_t wf;

                if (fp = fopen(board_name,"ab+" )) {
                        wf = fwrite("\n", 1, sizeof("\n")-1, fp );
                        fclose(fp);
                        printf("Number of bytes written = %i\n", wf);
                }
                else {
                        board_exists = -1;
                }

        	if (fp = fopen(board_name,"ab+" )) {
                	wf = fwrite(user_name, 1, strlen(user_name), fp );
                	fclose(fp);
               		printf("Number of bytes written = %i\n", wf);
        	}		
		else {
			board_exists = -1;
		}

                if (fp = fopen(board_name,"ab+" )) {
                        wf = fwrite("\n", 1, sizeof("\n")-1, fp );
                        fclose(fp);
                        printf("Number of bytes written = %i\n", wf);
                }
		else {
			board_exists = -1;
		}

                if (fp = fopen(board_name,"ab+" )) {
                        wf = fwrite(message, 1, strlen(message), fp );
                        fclose(fp);
                        printf("Number of bytes written = %i\n", wf);
                }
                else {
                        board_exists = -1;
                }

	} 


	if (board_exists != -1) {	
		strcpy(server_resp,"Yes");
	}
	else {
		strcpy(server_resp, "No");
	}

	if(sendto(udp_sock,server_resp,strlen(server_resp),0,(struct sockaddr *)&sin, sizeof(struct sockaddr)) == -1) {
                perror("ERROR: client-sendto()\n");
                exit(1);
        } //end sendto check

}


void deleteMessage (int udp_sock, struct  sockaddr_in sin, char * user_name) {

	printf("Inside delete\n");
	
	/* Declare Variables */
	char board_name[100];		
	char message[4096];
        char server_resp[100];
        int addr_len;
	int board_exists;

	FILE *fp;
	char * line = NULL;
	char prev_line[200];
	size_t len = 0;
	ssize_t read;

	char * line_msg = NULL;
	size_t len_msg=0;	

	int found_message;
	FILE* out_file = fopen("temp.txt", "ab+");
	


        memset(board_name, '\0', sizeof(board_name));
        memset(message, '\0', sizeof(message));
        memset(server_resp, '\0', sizeof(server_resp));

	/* Receive board_name of boad to delete */
        if(recvfrom(udp_sock,board_name,sizeof(board_name),0,(struct sockaddr *)&sin, &addr_len) == -1) {
                perror("ERROR: client-recvfrom()\n");
                exit(1);
        } //end recvfrom check
	
	/* Receive message to be deleted */
        if(recvfrom(udp_sock,message,sizeof(message),0,(struct sockaddr *)&sin, &addr_len) == -1) {
                perror("ERROR: client-recvfrom()\n");
                exit(1);
        } //end recvfrom check

        strcat(user_name,"\n");

        printf("board_name: %s\n", board_name);
        printf("message: %s\n", message);
	size_t wf;
	printf("Before deletion\n");
	/* PERFORM DELETION*/
	if ( (board_exists = access(board_name, F_OK)) != -1 ) {
		printf("Inside access\n");
	        if ( fp = fopen(board_name, "rw")) {
			printf("Before while\n");
			while (read = getline(&line, &len, fp) != -1) {
				printf("Inside while\n");
				printf("LINE = %s\n", line);
				printf("MESSAGE = %s\n", message);
				printf("USERNAME = %s\n", user_name);
				printf("PREV_LINE = %s\n", prev_line);
				if ( (strcmp(user_name,line)==0)) {
					if (read = getline(&line_msg, &len_msg, fp) != -1 ) {
						if (strcmp(message,line)==0) {
				     			printf("FOUND!!!\n");
                                        		found_message = 1;	
						} 
						else {
                                        		printf("printing line = %s\n", line);
                                                        printf("printing line_msg = %s\n", line_msg);
                                       			wf = fwrite(line, 1, sizeof(line)-1, out_file);
                                                        wf = fwrite(line_msg, 1, sizeof(line_msg)-1, out_file);
                                        		fclose(out_file);
                                        		scanf("%s",server_resp);
                                        		out_file = fopen("temp.txt", "ab+");		
						}
					}
				}
				else  {
					printf("printing line = %s\n", line);
					wf = fwrite(line, sizeof(char), sizeof(line)-1, out_file );	
        	                        /*memset(prev_line, '\0', sizeof(prev_line));*/
	                                /*snprintf(prev_line, sizeof(prev_line),"%s", line);*/
					fclose(out_file);
					scanf("%s",server_resp);
					out_file = fopen("temp.txt", "ab+");	
				}
			}
        	}
	}
	fclose(fp);
	fclose(out_file);
	printf("found message = %i\n", found_message);
	if (found_message == 1) {
		rename("temp.txt", board_name);
		strcpy(server_resp, "Yes");
	        //unlink("temp.txt");
	}
	else {
		strcpy(server_resp, "No");
	}

	/* END DELETION */	
	printf("Before sending\n");	

        strcpy(server_resp,"Yes");
        if(sendto(udp_sock,server_resp,strlen(server_resp),0,(struct sockaddr *)&sin, sizeof(struct sockaddr)) == -1) {
                perror("ERROR: client-sendto()\n");
                exit(1);
        } //end sendto check
	printf("After sending respinse");

}



void editMessage(int udp_sock, struct  sockaddr_in sin, char * user_name) {

        printf("Inside insert\n");

        /* Declare Variables */
        char board_name[100];
        char message[4096];
	char new_message[4096];
        char server_resp[100];
        int addr_len;
        int board_exists;

        FILE *fp;
        char * line = NULL;
        char * prev_line = "";
        size_t len = 0;
        ssize_t read;

        int found_message;
        FILE* out_file = fopen("temp.txt", "w+");

        memset(board_name, '\0', sizeof(board_name));
        memset(message, '\0', sizeof(message));
        memset(server_resp, '\0', sizeof(server_resp));

        /* Receive board_name of boad to delete */
        if(recvfrom(udp_sock,board_name,sizeof(board_name),0,(struct sockaddr *)&sin, &addr_len) == -1) {
                perror("ERROR: client-recvfrom()\n");
                exit(1);
        } //end recvfrom check

        /* Receive message to be deleted */
        if(recvfrom(udp_sock,message,sizeof(message),0,(struct sockaddr *)&sin, &addr_len) == -1) {
                perror("ERROR: client-recvfrom()\n");
                exit(1);
        } //end recvfrom check

        /* Receive message that will replace old message */
        if(recvfrom(udp_sock,new_message,sizeof(new_message),0,(struct sockaddr *)&sin, &addr_len) == -1) {
                perror("ERROR: client-recvfrom()\n");
                exit(1);
        } //end recvfrom check

        strcat(user_name,"\n");
        printf("board_name: %s\n", board_name);
        printf("message: %s\n", message);
        printf("new message: %s\n", new_message);

        size_t wf;

        printf("Before edit\n");
        /* PERFORM EDIT*/
    	if ( (board_exists = access(board_name, F_OK)) != -1 ) {
                printf("Inside access\n");
                if ( fp = fopen(board_name, "rw")) {
                        printf("Before while\n");
                        while (read = getline(&line, &len, fp) != -1) {
                                printf("Inside while\n");
                                printf("LINE = %s\n", line);
                                printf("MESSAGE = %s\n", message);
                                printf("USERNAME = %s\n", user_name);
                                printf("PREV_LINE = %s\n", prev_line);
                                if ( (strcmp(user_name,line)==0)) {
					printf("Writing username\n");
					wf = fwrite(line, 1, sizeof(line)-1, out_file );
                                        fclose(out_file);
                                        scanf("%s",server_resp);
                                        out_file = fopen("temp.txt", "ab+");
                                        if (read = getline(&line, &len, fp) != -1 ) {
						printf("3 printing line = %s\n", line);
						strcat(message,"\n");
                                                if (strcmp(message,line)==0) {
                                                        printf("FOUND!!!\n");
                                                        found_message = 1;

                                                        printf("printing line = %s\n", new_message);
                                                        wf = fwrite(new_message, 1, sizeof(line)-1, out_file );
                                                        fclose(out_file);
                                                        scanf("%s",server_resp);
                                                        out_file = fopen("temp.txt", "ab+");
                                                } 
                                                else {  
                                                        printf("2 printing line = %s\n", line);
                                                        wf = fwrite(line, 1, sizeof(line)-1, out_file );
                                                        fclose(out_file);
                                                        scanf("%s",server_resp);
                                                        out_file = fopen("temp.txt", "ab+");
                                                }
                                        }
                                }
                                else  { 
                                        printf("1 printing line = %s\n", line);
                                        wf = fwrite(line, 1, sizeof(line)-1, out_file );
                                        /*memset(prev_line, '\0', sizeof(prev_line));*/
                                        /*snprintf(prev_line, sizeof(prev_line),"%s", line);*/
                                        fclose(out_file);
                                        scanf("%s",server_resp);
                                        out_file = fopen("temp.txt", "ab+");    
                                }
                        }
                }
        }

/*    if ( (board_exists = access(board_name, F_OK)) != -1 ) {
                printf("Inside access\n");
                if ( fp = fopen(board_name, "r")) {
                        printf("Before while\n");
                        while (read = getline(&line, &len, fp) != -1) {
                                printf("Inside while\n");
                                if ( (strcmp(message,line)) &&  strcmp (user_name,prev_line) ){
                                        printf("Found message\n");
                                        found_message = 1;
                                        break;
					fprintf(out_file, "%s", new_message);
                                }
                                else  { 
                                        fprintf(out_file, "%s", line);
                                }
                                prev_line = line;
                        }
                }
        }
*/
        fclose(fp);
        fclose(out_file);
        if (found_message == 1) {
                rename("temp.txt", board_name);
                strcpy(server_resp, "Yes");
        }
        else {  
                strcpy(server_resp, "No");
        }



}


void list (int udp_sock, struct  sockaddr_in sin, char * user_name) {
	printf("Inside lsit\n");	
        /* Declare Variables */
	char server_responze[100];
	char board_name[4096];
        int addr_len;
	int dir_size = 0;
	DIR *dir_stream;
	char list_buf[100];
	struct dirent *dir_read;
	ssize_t read;

        memset(board_name, '\0', sizeof(board_name));

	printf("before opendir\n");
	/* GET LIST OF ALL THE BOARDS*/
	dir_stream = opendir("./");
	if (dir_stream != NULL) {
		printf("reading directory size\n");
		/* Calculating the size of the directory */
                while ( dir_read = readdir(dir_stream) ) {
                        strcpy (list_buf, dir_read->d_name);
                        dir_size ++;
                        memset(list_buf, '\0', sizeof(list_buf));
                }
                closedir(dir_stream);
		printf ("dir_size = %i\n", dir_size);
                /* Sending directory size to client */
	        if(sendto(udp_sock, &dir_size, sizeof(dir_size),0,(struct sockaddr *)&sin, sizeof(struct sockaddr)) == -1) {
                	perror("ERROR: client-sendto()\n");
        	        exit(1);
	        } //end sendto check
		printf("before reading directory and sending\n");
                /* Send directory listing to cleint */
                dir_stream = opendir("./");
                while ( dir_read = readdir(dir_stream) ) {
                        strcpy ( list_buf, dir_read->d_name );

                        if(sendto(udp_sock,list_buf,strlen(list_buf),0,(struct sockaddr *)&sin, sizeof(struct sockaddr)) == -1) {
                                perror("ERROR: client-sendto()\n");
                                exit(1);
                        } //end sendto check         memset(list_buf, '\0', sizeof(list_buf));         


                        memset(list_buf, '\0', sizeof(list_buf));
                }
		printf("after sending\n");

	}


}

void readBoard(int tcp_sock) {

	printf("Inside read board\n");	

        char board_name[100];
        char line[1000];
        char server_resp[100];
	int board_exists;
	int board_name_len;

        int board_size_2;


	FILE *fp;
	int len;
        printf("HERE 2\n");
        memset(board_name, '\0', sizeof(board_name));
        printf("HERE 3\n");
        memset(server_resp, '\0', sizeof(server_resp));
        printf("HERE 4\n");
	memset(line,'\0', sizeof(line));


	printf("HERE 5\n");
	recv(tcp_sock,&board_name_len,sizeof(board_name_len),0);
        board_name_len = ntohl(board_name_len);
	printf("File length = %i\n",board_name_len);
	
	while(strlen(board_name)<board_name_len) {
		recv(tcp_sock,board_name,sizeof(board_name),0);
	}

	printf("Board name = %s\n", board_name);

	//board_size_2 = fileSize(board_name);
	//board_size_2 = htonl(board_size_2);
	//send(tcp_sock,&board_size_2,sizeof(int32_t),0);	

	if ( (board_exists = access(board_name, F_OK)) != -1 ) {
		printf("Inside if\n");
		board_size_2 = fileSize(board_name);
		if (board_size_2 == 0) {
			printf("file size = 0\n");
			board_size_2 = -1;
        	        send(tcp_sock,&board_size_2,sizeof(int32_t),0);
		}
		else {
			printf("board size = %i\n", board_size_2);
			board_size_2 = htonl(board_size_2);
                        printf("board size = %i\n", board_size_2);
        	        send(tcp_sock,&board_size_2,sizeof(int32_t),0);
			fp = fopen(board_name, "r");	
			while ( (len=fread(line,sizeof(char),sizeof(line),fp)) > 0) {
				send(tcp_sock,line,len,0);
				memset(line,'\0',sizeof(line));
			}
			fclose(fp);
		}
	}	

}


void append(int tcp_sock, char * user_name){

	printf("Inside append\n");
	/* Declare variables */
        char board_name[100];           // Board name
        int board_name_len;                  // Length of board name
        char file_name[100];            // File name
        int file_name_len;                   // Length of file name
        int server_response;            // Server confirmation
        int file_size,original_size;    // File size
        FILE *fp;
        char file_line[20000];
        int len, sent_len;

	int data_received=0;
	int total_data_received=0;
	
	int board_exists=0;
	int file_exists=0;
	char append_file_name[200];

	int status = 0;
	
        recv(tcp_sock,&board_name_len,sizeof(board_name_len),0);
        //printf("board name length = %i\n",board_name_len);

        while(strlen(board_name)<board_name_len) {
                recv(tcp_sock,board_name,sizeof(board_name),0);
        }
	//printf("board name = %s\n", board_name);

        recv(tcp_sock,&file_name_len,sizeof(file_name_len),0);
        printf("File name length = %i\n",file_name_len);

        while(strlen(file_name)<file_name_len) {
                recv(tcp_sock,file_name,sizeof(file_name),0);
        }
	//printf("File name = %s\n", file_name);

	if (  !((board_exists = access(board_name, F_OK)) != -1) ) {
		status = -1;
		send(tcp_sock, &status, sizeof(int32_t), 0);
		return;
	}
	if ( file_exists = access(file_name, F_OK) != -1 ) {
		status = -2;	
		send(tcp_sock, &status, sizeof(int32_t), 0);
		return;
	}

	printf("status = 1 \n");
        	status = 1;
	        send(tcp_sock, &status, sizeof(int32_t), 0);


                printf("Inside if\n");
		strcat(append_file_name,board_name);
		strcat(append_file_name,"-");
		strcat(append_file_name,file_name );
		
		recv(tcp_sock,&file_size,sizeof(file_size), 0);
		
		if (file_size == -1 ) {
			return;
		}

                size_t wf;
                fp = fopen(file_name,"ab+" ); 

        	while (total_data_received < file_size) {
	                if (data_received = recv(tcp_sock,file_line,sizeof(file_line),0) == -1) {
                        	total_data_received += data_received;
	                        wf = fwrite(file_line, 1, strlen(file_line), fp );
	                        printf("Number of bytes written = %i\n", wf);
        		} //end if
	        } //end WHILE
		fclose(fp);
		if (fp = fopen(board_name,"ab+" )) {
                        wf = fwrite(user_name, 1, strlen(user_name), fp );
                        printf("Number of bytes written = %i\n", wf);
			wf = fwrite("\nUser appended: ", 1, sizeof("\nUser appended: ")-1,fp);	
                        printf("Number of bytes written = %i\n", wf);
			wf = fwrite(append_file_name, 1, strlen(append_file_name), fp );
                        printf("Number of bytes written = %i\n", wf);
                        fclose(fp);
                }
	
}


void download(int tcp_sock){

        /* Declare variables */
        char board_name[100];           // Board name
        int board_name_len;                  // Length of board name
        char file_name[100];            // File name
        int file_name_len;                   // Length of file name
        char server_resp[10];            // Server confirmation
        int file_size;                  // File size
        FILE *fp;
        char file_line[20000];
        int  recv_len, recv_bytes, write_size;

        char *line = NULL;
	size_t len;
	int board_exists=0;

        char board_attachment[200];	
        memset(board_attachment, '\0', sizeof(board_attachment));
        strcpy(board_attachment,"User appended: " );

	/* Receive boardname length and board name */
        recv(tcp_sock,&board_name_len,sizeof(board_name_len),0);
        printf("File length = %i\n",board_name_len);

        while(strlen(board_name)<board_name_len) {
                recv(tcp_sock,board_name,sizeof(board_name),0);
        }	
	

        /* Recieve file_name length and file name  */
        recv(tcp_sock,&file_name_len,sizeof(file_name_len),0);
        printf("File length = %i\n",file_name_len);

        while(strlen(file_name)<file_name_len) {
                recv(tcp_sock,file_name,sizeof(file_name),0);
        }       
        
	if ( (board_exists = access(board_name, F_OK)) != -1) {
                if ( fp = fopen(board_name, "r")) {
                	while (getline(&line, &len, fp) != -1) {
                        	char temp[strlen(board_attachment)];
                                char filename_temp[100];
                                strncpy(temp,line,strlen(board_attachment));
                                if (strcmp (board_attachment, temp)) {
                                	strncpy(file_name,line+strlen(board_attachment), strlen(line)-strlen(board_attachment));
					if (strcmp(file_name,filename_temp)) {
						sendFile(tcp_sock, file_name);
						break;
					}                                        

				}
			}
		} 
	}
        

}


void destroy(int udp_sock, struct  sockaddr_in sin, char * user_name) {


        /* Declare variables */
        char board_name[100];           // Board name
        char server_resp[100];          // Server response
        int addr_len;

        FILE *fp;
        char * line = NULL;
        char * prev_line = "";
        size_t len = 0;

	char board_username[100];
	char board_attachment[200];
	int board_exists=0;


        memset(board_name, '\0', sizeof(board_name));
        memset(server_resp, '\0', sizeof(server_resp));
	memset(board_attachment, '\0', sizeof(board_attachment));
	
        strcpy(board_attachment,"User appended: " );



	strcpy(server_resp, "No");
        /* Receive server confirmation regarding creation of board and print results */
        if(recvfrom(udp_sock,board_name,sizeof(board_name),0,(struct sockaddr *)&sin, &addr_len) == -1) {
                perror("ERROR: client-recvfrom()\n");
                exit(1);
        } //end recvfrom check	

	/* Delete board and files appended to it */ 
	if ( (board_exists = access(board_name, F_OK)) != -1) {
                if ( fp = fopen(board_name, "r")) {
			if (getline(&line, &len, fp) != -1){  
				strncpy(line, line, strlen(user_name) );
				strcpy(board_username, line);

				if (strcmp(board_username, user_name)) {
					strcpy(server_resp, "Yes");
					while (getline(&line, &len, fp) != -1) {
						char temp[strlen(board_attachment)];
						char file_name[100];
						strncpy(temp,line,strlen(board_attachment));
						if (strcmp (board_attachment, temp)) {
							strncpy(file_name,line+strlen(board_attachment), strlen(line)-strlen(board_attachment));
							remove(file_name);
						}
					unlink(board_name);
					}
				} 
			}
		} 
	}

        if (sendto (udp_sock, server_resp, strlen(server_resp), 0, (struct sockaddr *)&sin, sizeof(struct sockaddr) ) == -1 ) {
                perror("ERROR: server-sendto()\n");
        }

}


void shutdownServer (int udp_sock, struct  sockaddr_in sin, char * admin_password) {

        /* Declare variables */
        char password[100];             // Admin password
        char server_resp[100];          // Server response
        int addr_len;
        int shutdown_ind=0;
	
        memset(password, '\0', sizeof(password));
        memset(server_resp, '\0', sizeof(server_resp));


        /* Receive board_name of board to create from the client */
        if(recvfrom(udp_sock,password,sizeof(password),0,(struct sockaddr *)&sin, &addr_len) == -1) {
                perror("ERROR: client-recvfrom()\n");
                exit(1);
        } //end recvfrom check

	if (strcmp(password,admin_password)) {
		strcpy(server_resp, "Yes" );
		deleteAll();
	}
	else {
        	strcpy(server_resp, "Yes" );
	}

        if (sendto (udp_sock, server_resp, strlen(server_resp), 0, (struct sockaddr *)&sin, sizeof(struct sockaddr) ) == -1 ) {
                perror("ERROR: server-sendto()\n");
        }

	
}


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


   	if ((listen(tcp_sock, MAX_PENDING)) < 0) {
                perror("ERROR: server-listen()\n");
		exit(1);
	} //end listen check
	printf("HERE 1\n");
	if ((tcp_sock = accept(tcp_sock, (struct sockaddr *)&sin, &addr_len)) < 0) {
		perror("ERROR: server-accept()\n");
		exit(1);
	} //end accept check



	while(1) {
		printf("Waiting for clien conection.\n");
        /*        if ((listen(tcp_sock, MAX_PENDING)) < 0) {
                        perror("ERROR: server-listen()\n");
                        exit(1);
                } //end listen check
		printf("HERE 1\n");
                if ((tcp_sock = accept(tcp_sock, (struct sockaddr *)&sin, &addr_len)) < 0) {
                        perror("ERROR: server-accept()\n");
                        exit(1);
                } //end accept check
*/
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
                                list(udp_sock,  sin, username);
                        }
                        else if (strcmp(buf,"DLT")==0){
                                deleteMessage(udp_sock, sin, username);
                        }
			else if (strcmp(buf,"EDT")==0) {
				editMessage(udp_sock, sin, username);
			}
                        else if (strcmp(buf, "RDB")==0){
                                readBoard(tcp_sock);
                        }
                        else if (strcmp(buf, "APN")==0){
                                append(tcp_sock, username);
                        }
                        else if (strcmp(buf,"DWN")==0){
                                download(tcp_sock);
                        }
			else if (strcmp(buf,"DST")==0) {
				destroy(udp_sock, sin, username);
			}
			else if (strcmp(buf,"SHT")==0) {
				shutdownServer(udp_sock, sin, username);
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

