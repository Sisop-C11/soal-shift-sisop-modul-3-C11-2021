/*
	C ECHO client example using sockets
*/
#include <stdio.h>	//printf
#include <string.h>	//strlen
#include <sys/socket.h>	//socket
#include <arpa/inet.h>	//inet_addr
#include <unistd.h>
#include <stdbool.h> 

void takeFile(char msg[], char namez[]){
  	char totalfile[1024];
  	strcpy(totalfile, "/home/solxius/Desktop/Sisop/Modul2/Client/");
  	strcat(totalfile, namez);
  	
	FILE *fp;
	fp = fopen (totalfile, "w");
	fprintf(fp, "%s", msg);
	fclose(fp);
}

int main(int argc , char *argv[])
{
	int sock, valread, choice;
	struct sockaddr_in server;
	char message[1000] , server_reply[1024] , username[1024], pass[1024], temp[1024];
	
	//Create socket
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1)
	{
		printf("Could not create socket");
	}
	puts("Socket created");
	
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons( 8887 );

	//Connect to remote server
	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		perror("connect failed. Error");
		return 1;
	}
	
	recv(sock , server_reply , 1024 , 0);
	if(strcmp(server_reply, "no")==0){
		puts("Currently in use\n");
		return 0;
	}
	
	puts("Connected\n");
	
	screen1:
    printf("1. Login\n2. Register\nEnter your choice (1/2) : ");
    scanf("%d", &choice);
    if(choice == 1)
    {
    	strcpy(username, "l ");
        printf("Username : ");
        getchar();
        scanf("%s", temp);
        strcat(username, temp);
        printf("Password : ");
        getchar();
        scanf("%s", pass);
        strcat(username, ":");
        strcat(username, pass);
        send(sock, username, strlen(username), 0);
        memset(server_reply, 0, 1024);
        if( recv(sock, server_reply, 1024 , 0) < 0)
		{
			puts("recv failed");
		}
        if(strcmp(server_reply, "success")==0)
        {
            printf("login success\n");
            send(sock, "sukses", 6, 0);
            goto screen2;
        }
        else if(strcmp(server_reply, "failure")==0)
        {
            printf("login failed\n");
            goto screen1;
        }
    }
    else if(choice == 2)
    {
        strcpy(username, "r ");
        printf("Username : ");
        getchar();
        scanf("%s", temp);
        strcat(username, temp);
        printf("Password : ");
        getchar();
        scanf("%s", pass);
        strcat(username, ":");
        strcat(username, pass);
        if( send(sock , username , strlen(username) , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
        memset(server_reply, 0, 1024);
        if( recv(sock , server_reply , 1024 , 0) < 0)
		{
			puts("recv failed");
		}
        printf("%s\n", server_reply);
        goto screen1;
    }
    else
    {
        printf("invalid input\n");
        goto screen1;
    }
    
    if(1==0){
    	screen2:;
    	char sended[1024], temp[1024], choice2[15];
    	memset(sended, 0, 1024);
    	printf("WELCOME TO SERVER BUKU JAMED\nWatchu wanna do?\n-add\n-download\n-see\nChoice: ");
    	scanf("%s", choice2);
    	if(strcmp(choice2, "add")==0){
    		strcpy(sended, "a \n");
    		printf("Publisher: ");
    		scanf("%s", temp);
    		strcat(sended, temp);
    		strcat(sended, "\t");
    		printf("Tahun publikasi: ");
    		scanf("%s", temp);
    		strcat(sended, temp);
    		strcat(sended, "\t");
    		printf("Filepath: ");
    		scanf("%s", temp);
    		strcat(sended, temp);
    		if( send(sock, sended, strlen(sended), 0) < 0)
		    {
		        puts("Send failed");
		        return 1;
		    }
		    if( recv(sock , server_reply , 1024 , 0) < 0)
			{
				puts("recv failed");
			}
		    printf("%s\n", server_reply);
		    
		    FILE *filez;
			char perline[1024], perfile[5000];
			memset(perfile, 0, 5000);
			memset(perline, 0, 1024);
			filez = fopen(temp, "a+");
			while(fscanf(filez, "%[^\n]\n", perline) != EOF)
			{
				strcat(perfile, perline);
				strcat(perfile, "\n");
			}
			fclose(filez);
			
			if( send(sock, perfile, strlen(perfile), 0) < 0)
		    {
		        puts("Send failed");
		        return 1;
		    }
		    if( recv(sock , server_reply , 1024 , 0) < 0)
			{
				puts("recv failed");
			}
		    printf("%s\n", server_reply);
		    
		    goto screen2;
    	}
    	
    	else if(strcmp(choice2, "download")==0){
    		strcpy(sended, "d ");
    		printf("What file would you like to download: ");
    		scanf("%s", temp);
    		strcat(sended, temp);
    		
    		if( send(sock, sended, strlen(sended), 0) < 0)
		    {
		        puts("Send failed");
		        return 1;
		    }
		    if( recv(sock, server_reply , 1024 , 0) < 0)
			{
				puts("recv failed");
			}
			if(strcmp(server_reply, "wrong")==0){
				puts("Tidak ada file tersebut.");
			}
			else{
				takeFile(server_reply, temp);
			}
		    goto screen2;
    	}
    	else if(strcmp(choice2, "see")==0){
    		if( send(sock, "see", 3, 0) < 0)
		    {
		        puts("Send failed");
		        return 1;
		    }
    	}
    }
	
	close(sock);
	return 0;
}
