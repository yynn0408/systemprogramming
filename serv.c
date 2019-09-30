#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>

#define SERV_TCP_PORT 11693
#define SERV_ADDR "165.246.38.152"

void main(){
   int s1,s2, x, y;
   struct sockaddr_in serv_addr, cli_addr;
   char buf[50];
   socklen_t  xx;

   printf("Hi, I am the server\n");

   bzero((char *)&serv_addr, sizeof(serv_addr));
   serv_addr.sin_family=PF_INET;
   serv_addr.sin_addr.s_addr=inet_addr(SERV_ADDR);
   serv_addr.sin_port=htons(SERV_TCP_PORT);


   if ((s1=socket(PF_INET, SOCK_STREAM, 0))<0){
      printf("socket creation error\n");
      exit(1);
   }
   printf("socket opened successfully. socket num is %d\n", s1);


   x =bind(s1, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
   if (x < 0){
      printf("binding failed\n");
      exit(1);
   }
   printf("binding passed\n");
   listen(s1, 5);
   xx = sizeof(cli_addr);
   s2 = accept(s1, (struct sockaddr *)&cli_addr, &xx);
   printf("we passed accept. new socket num is %d\n", s2);


   printf("now reading from client\n");

   int i,child;
   printf("enter a string to send\n");

   child=fork();

   if(child==0){ // 자식은 보내기
        for(i=0;i<10;i++){
                fgets(buf,50,stdin);
                buf[strlen(buf)-1]=0;
                write(s2, buf, strlen(buf));
                if(strcmp(buf,"bye")==0){
                        kill(getppid(), 15);
                        break;
                }
        }
   }
   else{  // 부모는 받기
        for(i=0;i<10;i++){
                y=read(s2,buf,50);
                buf[y]=0;
                printf("from cli: %s\n", buf);

                if(strcmp(buf,"bye")==0){
                        kill(child, 15);
                        break;
                }
        }

   }
   close(s2);   // disconnect the connection
   close(s1);   // close the original socket :
}
       