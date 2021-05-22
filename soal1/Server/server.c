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

char akuns[200][1024] = {0}, tosend[5000], filea[100][50], fileb[100][50], filec[100][100], filed[100][50], filee[100][50], logined[200];
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
			strcpy(logined, checker);
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
    
    char aa[50], bb[50], cc[100], dd[50], ee[50];
    memset(aa, 0, 50);
    memset(bb, 0, 50);
    memset(cc, 0, 100);
    memset(dd, 0, 50);
    memset(ee, 0, 50);
    	
    	sscanf(fullz, "%s\t%s\t%s\n", aa, bb, cc);
		strcpy(filea[filecount], aa);
		strcpy(fileb[filecount], bb);
		strcpy(filec[filecount], cc);
		printf("%s\n", filec[filecount]);
		
		int dcounter = 0;
		    for(int i=0; i<strlen(cc); i++){
		    	if(cc[i]=='/'){
		    		memset(dd, 0, 50);
		    		dcounter=0;
		    		continue;
		    	}
		    	dd[dcounter]=cc[i];
		    	dcounter++;
		    }
			
			strcpy(filed[filecount], dd);
			
			int ecounter = 0;
		    for(int i=0; i<strlen(dd); i++){
		    	if(dd[i]=='.'){
		    		memset(ee, 0, 50);
		    		ecounter=0;
		    		continue;
		    	}
		    	ee[ecounter]=dd[i];
		    	ecounter++;
		    }
			
			strcpy(filee[filecount], ee);
			
	
    fclose(tsvmake); 
   
    FILE *filez1;
	filez1 = fopen("running.log", "a+");
	fprintf(filez1, "Tambah : %s (%s)\n", filed[filecount], logined);
	fclose(filez1);
			   
	filecount++;
    return;   
}

void takeFile(char msg[], char namez[]){
 	const char ch = '/';
  	char *ret;
  	char aa[100];
  	memset(aa, 0, 100);

  	ret = strrchr(namez, ch);
  	for(int j=1; j<strlen(ret); j++){
			aa[j-1]=ret[j];
	}
  	
  	char totalfile[100];
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
	puts("beforehere");
	
	for(int i=0; i<filecount; i++){
		if(strcmp(fullz, filed[i])==0){
			char pathz[200];
			strcpy(pathz, "FILES/");
			strcat(pathz, fullz);
			FILE *filez;
			char perline[1024];
			memset(tosend, 0, 5000);
			memset(perline, 0, 1024);
			puts("untilhere");
			filez = fopen(pathz, "a+");
			while(fscanf(filez, "%[^\n]\n", perline) != EOF)
			{
				strcat(tosend, perline);
				strcat(tosend, "\n");
			}
			fclose(filez);
			puts("enduntilhere");
			return true;
		}
	}
	puts("false");
	return false;
	
}

bool seeFile(){
	if (filecount==0){
		return false;
	}

	for(int i=0; i<filecount; i++){
		strcat(tosend, "\nNama: ");
		char abc[50];
		memset(abc, 0, 50);
		int counts=0;
		for(int j=0; j<strlen(filed[i]); j++){
			if(filed[i][j]=='.'){
				break;
			}
			abc[counts]=filed[i][j];
			counts++;
		}
		strcat(tosend, abc);
		
		strcat(tosend, "\nPublisher: ");
		strcat(tosend, filea[i]);
		
		strcat(tosend, "\nTahun publishing: ");
		strcat(tosend, fileb[i]);
		
		strcat(tosend, "\nEkstensi File : ");
		strcat(tosend, filee[i]);
		
		strcat(tosend, "\nFilepath : ");
		strcat(tosend, filec[i]);
		
		strcat(tosend, "\n");
	}
	return true;

}

bool deleteFile(char msg[]){
	char fullz[strlen(msg)];
	memset(fullz, 0, strlen(msg));
	for(int i=2; i<strlen(msg); i++){
		fullz[i-2]=msg[i];
	}
	
	for(int i=0; i<filecount; i++){
		if(strcmp(fullz, filed[i])==0){
			char newname[300] = {0}, oldname[300] = {0};
			sprintf(newname, "/home/solxius/Desktop/Sisop/Modul3/Server/FILES/old-%s", filed[i]);
			sprintf(oldname, "/home/solxius/Desktop/Sisop/Modul3/Server/FILES/%s", filed[i]);
			rename(oldname, newname);
			
			filecount--;
			for (int j=i; j<filecount; j++){
				strcpy(filea[j], filea[j+1]);
				strcpy(fileb[j], fileb[j+1]);
				strcpy(filec[j], filec[j+1]);
				strcpy(filed[j], filed[j+1]);
				strcpy(filee[j], filee[j+1]);
			}
			
			remove("files.tsv");
			FILE *tsvmake;
			tsvmake = fopen("files.tsv", "a+");
			for (int j=0; j<filecount; j++){
				fprintf(tsvmake, "%s\t%s\t%s\n", filea[j], fileb[j], filec[j]);
			}
			fclose(tsvmake);
			
			FILE *filez1;
			filez1 = fopen("running.log", "a+");
			fprintf(filez1, "Hapus : %s (%s)\n", filed[i], logined);
			fclose(filez1);
			
			return true;
		}
	}
	puts("false");
	return false;
	
}

bool findFile(char msg[]){
	bool checkz = false;
	char abc[50];
	
	char fullz[strlen(msg)];
	memset(fullz, 0, strlen(msg));
	for(int i=2; i<strlen(msg); i++){
		fullz[i-2]=msg[i];
	}
	
	for(int i=0; i<filecount; i++){
		memset(abc, 0, 50);
		int counts=0;
		for(int j=0; j<strlen(filed[i]); j++){
			if(filed[i][j]=='.'){
				break;
			}
			abc[counts]=filed[i][j];
			counts++;
		}
		
		if (strstr(abc, fullz) != NULL){
		
			strcat(tosend, "\nNama: ");
			strcat(tosend, abc);
			
			strcat(tosend, "\nPublisher: ");
			strcat(tosend, filea[i]);
			
			strcat(tosend, "\nTahun publishing: ");
			strcat(tosend, fileb[i]);
			
			strcat(tosend, "\nEkstensi File : ");
			strcat(tosend, filee[i]);
			
			strcat(tosend, "\nFilepath : ");
			strcat(tosend, filec[i]);
			
			strcat(tosend, "\n");
		
			checkz = true;
		}
	}
	
	return checkz;

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
    
    FILE *filez1;
    filez1 = fopen("running.log", "a+");
    fclose(filez1);
    
    
    FILE *tsvmake;
    char aa[50], bb[50], cc[100], dd[50], ee[50];
    tsvmake = fopen("files.tsv", "a+");
    	memset(aa, 0, 50);
    	memset(bb, 0, 50);
    	memset(cc, 0, 100);
    	memset(dd, 0, 50);
    	memset(ee, 0, 50);
    	while(fscanf(tsvmake, "%s\t%s\t%s\n", aa, bb, cc) != EOF) {
			strcpy(filea[filecount], aa);
		    strcpy(fileb[filecount], bb);
		    strcpy(filec[filecount], cc);
		    
		    int dcounter = 0;
		    for(int i=0; i<strlen(cc); i++){
		    	if(cc[i]=='/'){
		    		memset(dd, 0, 50);
		    		dcounter=0;
		    		continue;
		    	}
		    	dd[dcounter]=cc[i];
		    	dcounter++;
		    }
			
			strcpy(filed[filecount], dd);
			
			int ecounter = 0;
		    for(int i=0; i<strlen(dd); i++){
		    	if(dd[i]=='.'){
		    		memset(ee, 0, 50);
		    		ecounter=0;
		    		continue;
		    	}
		    	ee[ecounter]=dd[i];
		    	ecounter++;
		    }
			
			strcpy(filee[filecount], ee);
	
		    filecount++;
		    memset(aa, 0, 50);
			memset(bb, 0, 50);
			memset(cc, 0, 100);
			memset(dd, 0, 50);
			memset(ee, 0, 50);
		}

    fclose(tsvmake);
    
    for(int i=0; i<filecount; i++){
			printf("%d %s %s %s %s %s \n", i, filea[i], fileb[i], filec[i], filed[i], filee[i]);
	}
    
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
	char *message , client_message[5000], apple[100];
	
	while(someone);
	send(sock, "ye", 2, 0 );
	
	someone = true;
	memset(client_message, 0, 5000);
	memset(tosend, 0, 5000);
	
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
				send(sock, "right", 5, 0);
				recv(sock, client_message, 5000, 0);
				puts("sending message");
				send(sock, tosend, strlen(tosend), 0);
			}
			else{
				puts("sending wrong");
				send(sock, "wrong", strlen("wrong"), 0);
			}
		}
		else if(client_message[0]=='z'){
			if(deleteFile(client_message)){
				puts("sending right");
				send(sock, "right", 5, 0);
			}
			else{
				puts("sending wrong");
				send(sock, "wrong", strlen("wrong"), 0);
			}
		}
		else if(client_message[1]=='e'){
			if(seeFile(client_message)){
				send(sock, "right", 5, 0);
				recv(sock, client_message, 5000, 0);
				puts("sending logs");
				send(sock, tosend, strlen(tosend), 0);
			}
			else {
				puts("sending wrong");
				send(sock, "wrong", 5, 0);
			}
		}
		else if(client_message[0]=='f'){
			if(findFile(client_message)){
				puts("found");
				send(sock, "right", 5, 0);
				recv(sock, client_message, 5000, 0);
				puts("sending logs");
				send(sock, tosend, strlen(tosend), 0);
			}
			else {
				puts("sending wrong");
				send(sock, "wrong", 5, 0);
			}
		}
		memset(client_message, 0, 5000);
		memset(tosend, 0, 5000);
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
