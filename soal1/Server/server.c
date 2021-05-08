#include<stdio.h>
#include<string.h>	//strlen
#include<stdlib.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write
#include<pthread.h> //for threading , link with lpthread
#include <stdbool.h> 
#include <sys/stat.h>
#include <sys/types.h>

char akuns[200][1024] = {0}, tosend[10000];
int akuncount = 0, filecount = 0;
bool someone = false;

//the thread function
void *connection_handler(void *);

void registerAccount(char msg[]){
	for(int i=2; i<strlen(msg); i++){
		akuns[akuncount][i-2]=msg[i];
	}
	akuncount++;
	FILE *fp;
	fp = fopen ("akun.txt", "a");
	fprintf(fp, "%s\n", akuns[akuncount-1]);
	fclose(fp);
}

bool loginAccount(char msg[]){
	char checker[strlen(msg)-1];
	memset(checker, 0, strlen(msg)-1);
	for(int i=2; i<strlen(msg); i++){
		checker[i-2]=msg[i];
	}
	for(int j=0; j<akuncount; j++){
		if(strcmp(akuns[j], checker)==0){
			return true;
		}
	}
	return false;
}

void addFile(char msg[]){
	char fullz[strlen(msg)];
	memset(fullz, 0, strlen(msg));
	for(int i=2; i<strlen(msg); i++){
		fullz[i-2]=msg[i];
	}

	FILE *tsvmake;
    tsvmake = fopen("files.tsv", "a");
    fprintf(tsvmake, "%s", fullz);
    fclose(tsvmake); 
   
			   
	filecount++;
    return;   
}

void takeFile(char msg[], char namez[]){
 	const char ch = '/';
  	char *ret;
  	char aa[1024];
  	memset(aa, 0, 1024);

  	ret = strrchr(namez, ch);
  	for(int j=1; j<strlen(ret); j++){
			aa[j-1]=ret[j];
	}
  	
  	char totalfile[1024];
  	strcpy(totalfile, "/home/solxius/Desktop/Sisop/Modul3/Server/FILES/");
  	strcat(totalfile, aa);
  	
	FILE *fp;
	fp = fopen (totalfile, "w");
	fprintf(fp, "%s", msg);
	fclose(fp);
}

bool downloadFile(char msg[]){
	char fullz[strlen(msg)];
	memset(fullz, 0, strlen(msg));
	for(int i=2; i<strlen(msg); i++){
		fullz[i-2]=msg[i];
	}
	
	char aa[1024];
	int flag=0;
	FILE *tsvmake;
    tsvmake = fopen("files.tsv", "a+");
	while(fscanf(tsvmake, "%[^\n]\n", aa) != EOF){
			   
		if(strstr(aa, fullz)){
			flag = 1;
			break;
		}
	}
	fclose(tsvmake);
	
	if(flag==0){
		return false;
	}
	else{
		char pathz[200];
		strcpy(pathz, "/home/solxius/Desktop/Sisop/Modul3/Client/FILES/");
		strcat(pathz, aa);
		FILE *filez;
		char perline[1024];
		memset(tosend, 0, 5000);
		memset(perline, 0, 1024);
		filez = fopen(pathz, "a+");
		while(fscanf(filez, "%[^\n]\n", perline) != EOF)
		{
			strcat(tosend, perline);
			strcat(tosend, "\n");
		}
		fclose(filez);
		return true;
	}
	
}

void seeFile(){
	memset(tosend, 0, 5000);
	while(fscanf(tsvmake, "%[^\t]\t%[^\t]\t%[^\n]\n", aa, bb, cc) != EOF){
		
	}
}

int main(int argc , char *argv[])
{
	int socket_desc , client_sock , c , *new_sock;
	struct sockaddr_in server , client;
	
	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket");
	}
	puts("Socket created");
	
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( 8887 );
	
	//Bind
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
		//print the error message
		perror("bind failed. Error");
		return 1;
	}
	puts("bind done");
	
	//Listen
	listen(socket_desc , 10);
	
	FILE *filez;
    char perfile[1024];
    filez = fopen("akun.txt", "a+");
    if(filez == NULL) exit(0);
    while(fscanf(filez, "%s\n", perfile) != EOF)
    {
        strcpy(akuns[akuncount], perfile);
        akuncount++;
    }
    fclose(filez);
    
    
    FILE *tsvmake;
    char aa[500], bb[500], cc[500];
    tsvmake = fopen("files.tsv", "a+");
    if(tsvmake == NULL){
    	fprintf(tsvmake, "publisher\ttahun publikasi\tpath file");
    }

    fclose(tsvmake);
    
    int result = mkdir("/home/solxius/Desktop/Sisop/Modul3/Server/FILES", 0777);
	
	//Accept and incoming connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);
	
	while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
	{
		puts("Connection accepted");
		
		pthread_t sniffer_thread;
		new_sock = malloc(1);
		*new_sock = client_sock;
		
		if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
		{
			perror("could not create thread");
			return 1;
		}
		
		//Now join the thread , so that we dont terminate before the thread
		//pthread_join( sniffer_thread , NULL);
		puts("Handler assigned");
	}
	
	if (client_sock < 0)
	{
		perror("accept failed");
		return 1;
	}
	
	return 0;
}

/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc)
{
	//Get the socket descriptor
	int sock = *(int*)socket_desc;
	int read_size;
	char *message , client_message[1024], apple[100];
	
	if(someone){
		send(sock, "no", 2, 0 );
		puts("Client disconnected because currently in use");
		fflush(stdout);
		free(socket_desc);
		return 0;
	}
	send(sock, "ye", 2, 0 );
	
	someone = true;
	memset(client_message, 0, 1024);
	
	//Receive a message from client
	while( (read_size = recv(sock , client_message , 1024 , 0)) > 0 )
	{
		if(client_message[0]=='r'){
			registerAccount(client_message);
			send(sock, "registered", 10, 0 );
		}
		else if(client_message[0]=='l'){
			bool yo = loginAccount(client_message);
			if(yo){
				send(sock, "success", 10, 0 );
			}
			else{
				send(sock, "failure", 10, 0 );
			}
		}
		else if(client_message[0]=='a'){
			addFile(client_message);
			char namez[1024];
			strcpy(namez, client_message);
			memset(client_message, 0, 1024);
			send(sock, "adding file", 11, 0 );
			recv(sock, client_message, 5000, 0);
			takeFile(client_message, namez);
			send(sock, "file added", 10, 0 );
		}
		else if(client_message[0]=='d'){
			if(downloadFile(client_message)){
				send(sock, tosend, strlen(tosend), 0);
			}
			send(sock, "wrong", 5, 0);
		}
		else if(client_message[1]=='e'){
			if(downloadFile(client_message)){
				send(sock, tosend, strlen(tosend), 0);
			}
			send(sock, "wrong", 5, 0);
		}
		memset(client_message, 0, 1024);
	}
	
	if(read_size == 0){
		puts("Client disconnected");
		fflush(stdout);
	}
	else if(read_size == -1){
		perror("recv failed");
	}
		
	//Free the socket pointer
	someone=false;
	free(socket_desc);
	
	return 0;
}
