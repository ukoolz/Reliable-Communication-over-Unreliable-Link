/*
This C code is a HTTP Client which is created to download a file from a server,
saves the file to the local machine and then uploads it to the http address specified by the user.

Http Methods used are: GET and PUT.

*/
#include <stdio.h>
#include <ctype.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#define MAXBUF  5000000

int compare(char *c);

//getrequest function

int Request(char *ipaddr, char *port){      //This function is created for HTTP GET Request.

	int sockfd;              //In this function socket creation, connection and HTTP GET header formatting takes place.
        char  path[100];   
	char getrequest[1024];
        const char *b=" Umesh";
        int sockbufsize;
        char *reqli[1];
        char s[MAXBUF];
        char server_r[MAXBUF];
        unsigned int intlen = sizeof(int);
        char filename[100];
        char filename1[100];
        char *tok;
        int bytes_read;
        char *show=(char*)calloc(50,sizeof(char));
       
        char *content;
	int pid;       
        struct addrinfo hints, *res, *rp;           //using addressinfo struct
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;           //Can handle IP address family of either IPV4 or IPV6
	hints.ai_socktype = SOCK_STREAM;

        if ( getaddrinfo(ipaddr, port, &hints, &res) != 0 ){
	fprintf(stderr, "Either of hostname or IP address is not valid\n");
	exit(0);
	}

	 for (rp = res; rp != NULL; rp = rp->ai_next) {
              sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
               if (sockfd == -1)
                   continue;

               if ( connect(sockfd, res->ai_addr, res->ai_addrlen) != 0 ){
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		exit(0);
		} else {break;}                  /* Success */
               
          close(sockfd);
           }

           if (rp == NULL) {               /* No address succeeded */
               fprintf(stderr, "Could not connect\n");
               exit(EXIT_FAILURE);
           }        

        if (getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF,
		       &sockbufsize, &intlen) < 0) {
		perror("getsockopt");
		return -1;
	}

	int optval = 1;
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
        
        printf("Please enter the filename to be downloaded preceeding with slash '/' ex: /test.txt\n");
        scanf("%s",path);

        sprintf(getrequest, "GET %s HTTP/1.1\r\nHOST: %s\r\nIam:%s\r\n\r\n", path, ipaddr,b);

	      write(sockfd, getrequest, strlen(getrequest));

        printf("Please enter the path where you want to save the file in your local desktop along with the filename of your choice ex:/home/Desktop/test.txt \n");
        scanf("%s",filename);
        strcpy(filename1,filename);
       
        
           
 	tok = strstr(filename1,".");
	
	
		printf("%s",getrequest);
	if(tok[1]=='h'){
		strcpy(show,"firefox ");
	}
	else if(tok[1]=='p'){
		strcpy(show,"evince ");
	}
	else if(tok[1]=='j'){
		strcpy(show,"eog ");
	}
	else
		strcpy(show,"gedit ");
		strcat(show,filename1);
		printf("%s",show);
		
		
		
		
		

    FILE *fp = fopen(filename, "w+");	//Opening a file.
       
    bzero(s, sizeof(s));
    
    while((bytes_read = recv(sockfd, s, MAXBUF, MSG_WAITALL)) > 0)
        {
            
            content = strstr(s, "\r\n\r\n");
           
         
            if(content != NULL)
            {
              content +=4;
              fwrite(content, 1, bytes_read, fp);
            }
            if(strncmp(s,"HTTP/1.1",8)==0)
           { 
            reqli[0] = strtok (s, "\r\n\r\n");
            printf("Your GET request has been completed with the following response %s \n", reqli[0]);
            }
 
             bzero(s, sizeof(s));
            
        }
         
    if(bytes_read < 0)
      {
    perror("Error receiving data");
       }
       
      fclose(fp);
      pid=fork();
      if(pid == 0)
      {

		system(show);
       }
return sockfd;
}

//put request function
int putrequest(char *ipaddr, char *port){    //This function is created for HTTP PUT Request. HTTP PUT request formatting takes place.

       int sockfd; 
       const char *ct = "text/plain";
       char putsrequest[MAXBUF];
       char filename[100];
       char filepath[100];
       char entirefilepath[256];
       struct addrinfo hints, *res, *rp ;
       memset(&hints, 0, sizeof hints);
       hints.ai_family = AF_UNSPEC;
       hints.ai_socktype = SOCK_STREAM;
       int i=0,f=0,n, totbytes=0;
       const char *c=" Praneeth";
       char a[MAXBUF];
       char *content[2];
       char s[MAXBUF];
       int bytes_read;

  printf("Please enter the path where the file is located\n");
  scanf("%s",filepath);
 
  strcat(entirefilepath,filepath);
  strcat(entirefilepath,"/");

  printf("Please enter the filename\n");
  scanf("%s",filename);

  strcat(entirefilepath,filename);
  printf("%s\n",entirefilepath);


    FILE *fp = fopen(entirefilepath, "r");   //opening the file from the local desktop to read.

    while((a[i++] = fgetc(fp))!= EOF)
           ;                                                //Fetching the contents of the file.

     a[i] ='\0';
     f=strlen(a);

snprintf(putsrequest,MAXBUF,"PUT %s HTTP/1.1\r\nHost: %s\r\nIam:%s\r\nContent-Type:%s\r\nContent-Length: %d\r\n\r\n %s\r\n",filename,ipaddr,c,ct,f,a);

	if ( getaddrinfo(ipaddr, port, &hints, &res) != 0 ){
		fprintf(stderr, "Either of hostname or IP address is not valid\n");
		exit(0);
		}


	 for (rp = res; rp != NULL; rp = rp->ai_next) {
              sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
               if (sockfd == -1)
                   continue;

               if ( connect(sockfd, res->ai_addr, res->ai_addrlen) != 0 ){
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		exit(0);
		} else {break;}                  /* Success */

               close(sockfd);
           }

           if (rp == NULL) {               /* No address succeeded */
               fprintf(stderr, "Could not connect\n");
               exit(EXIT_FAILURE);
           }        


     	int optval = 1;

        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
 
if ( (n = write(sockfd, putsrequest, strlen(putsrequest))) > 0) {
		totbytes += n;
		printf("wrote %d bytes, total %d bytes\n", n, totbytes);
		
	}
else {
		perror("write error");
		return 1;
	}

        pclose(fp);
       bzero(s, sizeof(s));
       bytes_read = recv(sockfd, s, sizeof(s), 0);
   
   if (bytes_read<0)    // receive error
        fprintf(stderr,("received() error\n"));
    else if (bytes_read==0)    // receive socket closed
        fprintf(stderr,"Server disconnected upexpectedly.\n");
    else    // message received
    {
       content[0]=strtok(s,"\r\n\r\n");
       printf("Your PUT request has been completed with the following response %s \n", content[0]);
    }
return sockfd;
}

int compare(char *c){
  if(c[0]=='G' && c[1]=='E' && c[2]=='T' )
    return 1;
  if(c[0]=='P' && c[1]=='U' && c[2]=='T' )
    return 2;
  if(c[0]=='E' && c[1]=='X' && c[2]=='I' && c[4]=='T')
    return 3;
  else 
  return 4;
}

//main function
int main(int argc, char **argv){   //Main function.

	int port, sockfd;
  char *command,*cmnd_cpy;
	char *host, *ptr;
        char choice[10];
      
        if ( argc != 3) {                  //validating the input arguments.
                fprintf(stderr, "Usage: hostname port \n");
                exit(0);
        }

        port = atoi(argv[2]);
	      host = argv[1];
    
	if ( (ptr = strstr(host, "http://")) != NULL || (ptr = strstr(host, "https://")) != NULL )

         {                                               //validating the hostnames that user enters
		host = host + 7;
                
	}


        if ( port > 65536 || port < 0 ){  //validating the ports
                fprintf(stderr, "Invalid port number\n");
                exit(0);
        }

	printf("MyBrowser:");
	printf("Please enter your choice \nGET\nPUT\n");

   while(1){
    printf("MyBrowser>");
    command=(char*)calloc(1000,sizeof(char));
    cmnd_cpy=(char*)calloc(1000,sizeof(char));
    fgets(command,100,stdin);
    strcpy(cmnd_cpy,command);
    switch(compare(strtok(cmnd_cpy," ")))
    {
      case 1: sockfd = Request(host, argv[2]);
              memset(cmnd_cpy,0,100);
        break;
      case 2: sockfd = putrequest(host, argv[2]);
              memset(cmnd_cpy,0,100);
        break;
      case 3: exit(1);
        break;
      case 4: memset(command,0,100);
      continue;
    }
  }

close(sockfd);



return 0;

}
