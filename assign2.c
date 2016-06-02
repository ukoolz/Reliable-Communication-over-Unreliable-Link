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
#define MAXBUF  500000
//getrequest function

int Request(char *ipaddr, char *port){      //This function is created for HTTP GET Request.

	int sockfd;              //In this function socket creation, connection and HTTP GET header formatting takes place.
        char  path[100];   
	char getrequest[1024];
        const char *b=" Praneeth";
        int sockbufsize;
        char *reqli[1];
        char s[MAXBUF];
        unsigned int intlen = sizeof(int);
        char filename[100];
        int bytes_read;
       
        char *content;
       
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

    FILE *fp = fopen(filename, "w+");	//Opening a file.
       
    bzero(s, sizeof(s));
    
    while((bytes_read = recv(sockfd, s, MAXBUF, MSG_WAITALL)) > 0)
        {
            
            content = strstr(s, "\r\n\r\n");
         
            if(content != NULL)
            {
              content +=4;
              fwrite(content, strlen(content) + 1, 1, fp);
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

return sockfd;
}


//main function
int main(int argc, char **argv){   //Main function.

	int port, sockfd;
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

	printf("Please enter your choice \nGET\nPUT\n");
	printf("MyBrowser:>");
	
        scanf("%[^\n]",choice);	

if (strcmp(choice,"get\0")==0 || strcmp(choice,"GET\0")==0)
       {  
     
     // sockfd = Request(host, argv[2]);
      }

else  if ((strcmp(choice, "put")==0)|| (strcmp(choice, "PUT")==0))
   {
  // sockfd = putrequest(host, argv[2]);
   } 

else{
printf("This application doesnot support such requests\n");
}

close(sockfd);


return 0;

}
