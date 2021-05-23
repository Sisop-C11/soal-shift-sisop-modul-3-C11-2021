# soal-shift-sisop-modul-3-C11-2021
## Anggota
* James Raferty Lee 	(05111940000055)
* Mohammad Tauchid		(05111940000136)
* Kevin Davi Samuel		(05111940000157)

# Soal dan Penjelasan Jawaban
## Soal Nomor 1
Keverk adalah orang yang cukup ambisius dan terkenal di angkatannya. Sebelum dia menjadi ketua departemen di HMTC, dia pernah mengerjakan suatu proyek dimana keverk tersebut meminta untuk membuat server database buku. Proyek ini diminta agar dapat digunakan oleh pemilik aplikasi dan diharapkan bantuannya dari pengguna aplikasi ini. 

Di dalam proyek itu, Keverk diminta: 

**(a)** Pada saat client tersambung dengan server, terdapat dua pilihan pertama, yaitu register dan login. Jika memilih register, client akan diminta input id dan passwordnya untuk dikirimkan ke server. User juga dapat melakukan login. Login berhasil jika id dan password yang dikirim dari aplikasi client sesuai dengan list akun yang ada didalam aplikasi server. Sistem ini juga dapat menerima multi-connections. Koneksi terhitung ketika aplikasi client tersambung dengan server. Jika terdapat 2 koneksi atau lebih maka harus menunggu sampai client pertama keluar untuk bisa melakukan login dan mengakses aplikasinya. Keverk menginginkan lokasi penyimpanan id dan password pada file bernama akun.txt dengan format :

akun.txt
id:password
id2:password2


**(b)** Sistem memiliki sebuah database yang bernama files.tsv. Isi dari files.tsv ini adalah path file saat berada di server, publisher, dan tahun publikasi. Setiap penambahan dan penghapusan file pada folder file yang bernama  FILES pada server akan memengaruhi isi dari files.tsv. Folder FILES otomatis dibuat saat server dijalankan. 

**(c)** Tidak hanya itu, Keverk juga diminta membuat fitur agar client dapat menambah file baru ke dalam server. Direktori FILES memiliki struktur direktori di bawah ini : 

Direktori FILES 
```
File1.ekstensi
File2.ekstensi
```

Pertama client mengirimkan input ke server dengan struktur sebagai berikut :
		Contoh Command Client :
```		
add
```

			

		Output Client Console:
```		
Publisher:
Tahun Publikasi:
Filepath:
```

Kemudian, dari aplikasi client akan dimasukan data buku tersebut (perlu diingat bahwa Filepath ini merupakan path file yang akan dikirim ke server). Lalu client nanti akan melakukan pengiriman file ke aplikasi server dengan menggunakan socket. Ketika file diterima di server, maka row dari files.tsv akan bertambah sesuai dengan data terbaru yang ditambahkan.

**(d)** Dan client dapat mendownload file yang telah ada dalam folder FILES di server, sehingga sistem harus dapat mengirim file ke client. Server harus melihat dari files.tsv untuk melakukan pengecekan apakah file tersebut valid. Jika tidak valid, maka mengirimkan pesan error balik ke client. Jika berhasil, file akan dikirim dan akan diterima ke client di folder client tersebut. 

Contoh Command client
```
download TEMPfile.pdf
```


**(e)** Setelah itu, client juga dapat menghapus file yang tersimpan di server. Akan tetapi, Keverk takut file yang dibuang adalah file yang penting, maka file hanya akan diganti namanya menjadi ‘old-NamaFile.ekstensi’. Ketika file telah diubah namanya, maka row dari file tersebut di file.tsv akan terhapus.

Contoh Command Client:
```
delete TEMPfile.pdf
```


**(f)** Client dapat melihat semua isi files.tsv dengan memanggil suatu perintah yang bernama see. Output dari perintah tersebut keluar dengan format. 

Contoh Command Client :
```
see
```


Contoh Format Output pada Client:
```
Nama:
Publisher:
Tahun publishing:
Ekstensi File : 
Filepath : 

Nama:
Publisher:
Tahun publishing:
Ekstensi File : 
Filepath : 
```


**(g)** Aplikasi client juga dapat melakukan pencarian dengan memberikan suatu string. Hasilnya adalah semua nama file yang mengandung string tersebut. Format output seperti format output f.

Contoh Client Command:
```
find TEMP
```

**(h)** Dikarenakan Keverk waspada dengan pertambahan dan penghapusan file di server, maka Keverk membuat suatu log untuk server yang bernama running.log. Contoh isi dari log ini adalah

running.log
```
Tambah : File1.ektensi (id:pass)
Hapus : File2.ektensi (id:pass)
```



Akan tetapi, kevek saat itu masih belum paham dengan sistem server dan client. Bantulah Keverk untuk menyelesaikan proyek ini!

Note: 
Dilarang menggunakan system() dan execv(). Silahkan dikerjakan sepenuhnya dengan thread dan socket programming. 
Untuk download dan upload silahkan menggunakan file teks dengan ekstensi dan isi bebas (yang ada isinya bukan touch saja dan tidak kosong) dan requirement untuk benar adalah percobaan dengan minimum 5 data.


Struktur Direktori:
```
.
├── Client
│   ├── client.c
│   ├── File2.extensi
│   └── File1.extensi
└── Server
    ├── akun.txt
    ├── files.tsv
    ├── server.c
    ├── running.log
    └── FILES
            ├── File2.extensi
            └── File1.ekstensi
```

## Jawaban Soal Nomor 1
**(a)**
client:
```c
int sock, valread, choice;
	struct sockaddr_in server;
	char message[1000] , server_reply[5000] , username[1024], pass[1024], temp[1024];
	
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
```
server:
```c
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
	
	void *connection_handler(void *socket_desc)
	{	
	//Get the socket descriptor
	int sock = *(int*)socket_desc;
	int read_size;
	char *message , client_message[5000], apple[100];
	
```

- Di sini adalah kerangka di mana kita dapat meng-connect antara server dengan client.

server.c
```c
while(someone);
```

client.c
```c
puts("Please wait.\n");
recv(sock , server_reply , 1024 , 0);
```

- Ini untuk supaya jika ada yang sedang connect dengan server, disuruh menunggu sampai yang lain sudah disconnect.


![image](https://user-images.githubusercontent.com/68369091/119257547-e21c1080-bbef-11eb-99bc-6154755358e3.png)
![image](https://user-images.githubusercontent.com/68369091/119257576-07a91a00-bbf0-11eb-9915-da389b2ea244.png)

client.c
```c
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
        memset(server_reply, 0, 5000);
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
```

```c
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
```

- Dari client, untuk register, kita mengirimkan isi username dan password ke server dengan simbol r, untuk membilang ke server bahwa kita ingin register. Lalu, server memasukkan ke database dan akun.txt isi dari username dan password baru.
- Untuk login, client mengirim username dan password ke server dengan simbol l, untuk embilang ke server bahwa kita ingin login. Server akan membandingkan dengan database, dan jika benar, memberitahu ke client bahwa credentials benar, sehingga client pindah ke screen baru.


https://user-images.githubusercontent.com/68369091/119258208-ebf34300-bbf2-11eb-936a-8a0c4de1a066.mp4

https://user-images.githubusercontent.com/68369091/119258224-fc0b2280-bbf2-11eb-90de-cc0085687d95.mp4


```c
FILE *filez;
char perfile[1024];
filez = fopen("akun.txt", "a+");
if(filez == NULL) exit(0);
while(fscanf(filez, "%s\n", perfile) != EOF)
{
      strcpy(akuns[akuncount], perfile);
      akuncount++;
}
fclose(filez)
```
- Ini untuk menyimpan ke database username dan password yang sudah disimpan di akun.txt di sesi sebelumnya.

**(b)**
```c
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
    
    int result = mkdir("/home/solxius/Desktop/Sisop/Modul3/Server/FILES", 0777);
```
- Di sini, file.tsv dibuat. Jika sebelumnya ada, kita memasukkan isinya ke dalam array. 
- Lalu, dibuat direktori FILES jika belum ada.

https://user-images.githubusercontent.com/68369091/119258609-a899d400-bbf4-11eb-9baf-c992d04fe801.mp4

**(c)**
server.c
```c
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
```

client.c
```c
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
``` 
- Di dalam client, kita meminta publisher, tahun publikasi, dan pathfile, lalu kita kirim ke server. Di server, kita masukkan ke dalam file.tsv. Lalu, kita masukkan ke array publishernya, tahun publikasinya, dan pathfile. Lalu, kita process sehingga bisa mendapat nama file dan ekstensi, dan kita masukkan ke array juga.
- Selanjutnya, di client, akan meng-scan isi dari file yang ingin ditambahkan ke server. Hasil scan dikirim ke server, di mana mereka bisa membuat file baru dengan nama yang sama dan memasukkan isi hasil scan ke dalamnya, sehingga dapat meng-rekreasikan isi file.


https://user-images.githubusercontent.com/68369091/119264159-261d0e80-bc0c-11eb-9aff-fb8bc94a67a1.mp4


![image](https://user-images.githubusercontent.com/68369091/119264093-eeae6200-bc0b-11eb-982f-60cb9ea81726.png)


**(d)**
server.c
```c
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
```
client.c
```c
void takeFile(char msg[], char namez[]){
  	char totalfile[1024];
  	strcpy(totalfile, namez);
  	
	FILE *fp;
	fp = fopen (totalfile, "w");
	fprintf(fp, "%s", msg);
	fclose(fp);
	puts("File terdownload");
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
		    if( recv(sock, server_reply , 5000 , 0) < 0)
			{
				puts("recv failed");
			}
			char sumn[10];
			strcpy(sumn, server_reply);
			
			puts("b4here");
			if(strcmp(sumn, "wrong")==0){
				puts("Tidak ada file tersebut.\n");
				goto screen2;
			}
			else{
				send(sock, "go", 2, 0);
				if( recv(sock, server_reply , 5000 , 0) < 0)
				{
					puts("recv failed");
				}
				puts("here");
				takeFile(server_reply, temp);
			}
		    goto screen2;
    	}
```
- Untuk download, client mengirim nama file. Lalu, di server, akan dicek dengan array database jika ada tidak. Jika ada, kita meng-scan isi file dan mengirimnya ke client.
- Sama persis dengan 1c, namun kebalikan saja. Client mengambil hasil scan dan membuat ulang file tersebut.
- Jika tidak ada di server, kita memberitahu ke client bahwa tidak ada.

https://user-images.githubusercontent.com/68369091/119264233-7bf1b680-bc0c-11eb-9b63-c1e968992e6f.mp4

**(e)**
server.c
```c
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
```

client.c
```c
else if(strcmp(choice2, "delete")==0){
    		strcpy(sended, "z ");
    		printf("What file would you like to delete: ");
    		scanf("%s", temp);
    		strcat(sended, temp);
    		
    		if( send(sock, sended, strlen(sended), 0) < 0)
		    {
		        puts("Send failed");
		        return 1;
		    }
    		if( recv(sock, server_reply , 5000 , 0) < 0)
			{
				puts("recv failed");
			}
			
			if(strcmp(server_reply, "wrong")==0){
				puts("Tidak ada file tersebut.\n");
				goto screen2;
			}
			else{
				puts("File telah dihapus.\n");
				goto screen2;
			}
    	}
```

- Pertama, client mengirim nama file ke server yang ingin di-delete, dengan simbol z supaya server tahu ini untuk delete.
- Jika ada di database, server mengganti nama file di server dengan ditambah "old-" di depannya. Lalu menghapus entry nya dalam files.tsv. Untuk melakukan itu, kita mengganti di array sehingga terhapus indeks dengan file tersebut. Lalu, menghapus files.tsv dan membuat baru dengan database baru.



**(f)**
server.c
```c
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
```

client.c
```c
else if(strcmp(choice2, "see")==0){
    		if( send(sock, "see", 3, 0) < 0)
		    {
		        puts("Send failed");
		        return 1;
		    }
		    
		    if( recv(sock, server_reply , 5000 , 0) < 0)
			{
				puts("recv failed");
			}
			char sumn[10];
			strcpy(sumn, server_reply);
			
			if(strcmp(sumn, "wrong")==0){
				puts("Files.tsv kosong.\n");
				goto screen2;
			}
			else{
				send(sock, "go", 2, 0);
				if( recv(sock, server_reply , 5000 , 0) < 0)
				{
					puts("recv failed");
				}
				printf("%s\n", server_reply);
				goto screen2;
			}
    	}
```

- Untuk see, kita tinggal mengirim isi array database yang kita punya dan sudah kita process dari sebelumnya, dan mengirimnya ke client.

![image](https://user-images.githubusercontent.com/68369091/119264279-a9d6fb00-bc0c-11eb-8c49-3ab8cbe0875c.png)



**(g)**
server.c
```c
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
```

client.c
```c
else if(strcmp(choice2, "find")==0){
    		strcpy(sended, "f ");
    		printf("What would you like to find: ");
    		scanf("%s", temp);
    		strcat(sended, temp);
    		
    		if( send(sock, sended, strlen(sended), 0) < 0)
		    {
		        puts("Send failed");
		        return 1;
		    }
		    
		    if( recv(sock, server_reply , 5000 , 0) < 0)
			{
				puts("recv failed");
			}
			char sumn[10];
			strcpy(sumn, server_reply);
			
			if(strcmp(sumn, "wrong")==0){
				puts("Tidak ditemukan.\n");
				goto screen2;
			}
			else{
				send(sock, "go", 2, 0);
				if( recv(sock, server_reply , 5000 , 0) < 0)
				{
					puts("recv failed");
				}
				printf("%s\n", server_reply);
				goto screen2;
			}
    	}
```

- Pertama kita mengirim ke server terms yang ingin dicari.
- Di server, kita menggunakan strstr di dalam array database nama file, dan jika ditemukan yang mengandung terms, kita kirim semua yang mengandung terms nya sesuai format kembali ke client untuk ditunjukkan.
- Jika tidak ada, kita bilang tidak ada.

![image](https://user-images.githubusercontent.com/68369091/119264294-bf4c2500-bc0c-11eb-8775-6d70e9647c92.png)

![image](https://user-images.githubusercontent.com/68369091/119264299-c83cf680-bc0c-11eb-83ef-03c2426c36cb.png)


**(h)**
server.c
```c
FILE *filez1;
filez1 = fopen("running.log", "a+");
fprintf(filez1, "Tambah : %s (%s)\n", filed[filecount], logined);
fclose(filez1);

filez1 = fopen("running.log", "a+");
fprintf(filez1, "Hapus : %s (%s)\n", filed[i], logined);
fclose(filez1);
```
- Saat hapus atau tambah, tinggal append/(tambah jika belum ada). Server selalu menyimpan kredensial yang sedang menggunakan, dan tinggal dimasukkan.

![image](https://user-images.githubusercontent.com/68369091/119264318-dab73000-bc0c-11eb-8190-ec649a458b6c.png)



## Soal Nomor 3
Seorang mahasiswa bernama Alex sedang mengalami masa gabut. Di saat masa gabutnya, ia memikirkan untuk merapikan sejumlah file yang ada di laptopnya. Karena jumlah filenya terlalu banyak, Alex meminta saran ke Ayub. Ayub menyarankan untuk membuat sebuah program C agar file-file dapat dikategorikan. Program ini akan memindahkan file sesuai ekstensinya ke dalam folder sesuai ekstensinya yang folder hasilnya terdapat di working directory ketika program kategori tersebut dijalankan

**(a)** Program menerima opsi -f seperti contoh di atas, jadi pengguna bisa menambahkan argumen file yang bisa dikategorikan sebanyak yang diinginkan oleh pengguna. 
Output yang dikeluarkan adalah seperti ini :

File 1 : Berhasil Dikategorikan (jika berhasil)

File 2 : Sad, gagal :( (jika gagal)

File 3 : Berhasil Dikategorikan

**(b)** Program juga dapat menerima opsi -d untuk melakukan pengkategorian pada suatu directory. Namun pada opsi -d ini, user hanya bisa memasukkan input 1 directory saja, tidak seperti file yang bebas menginput file sebanyak mungkin. Contohnya adalah seperti ini:

$ ./soal3 -d /path/to/directory/

Perintah di atas akan mengkategorikan file di /path/to/directory, lalu hasilnya akan disimpan di working directory dimana program C tersebut berjalan (hasil kategori filenya bukan di /path/to/directory).
Output yang dikeluarkan adalah seperti ini :

Jika berhasil, print “Direktori sukses disimpan!”

Jika gagal, print “Yah, gagal disimpan :(“

**(c)** Selain menerima opsi-opsi di atas, program ini menerima opsi *, contohnya ada di bawah ini:

$ ./soal3 \*

Opsi ini akan mengkategorikan seluruh file yang ada di working directory ketika menjalankan program C tersebut.

**(d)** Semua file harus berada di dalam folder, jika terdapat file yang tidak memiliki ekstensi, file disimpan dalam folder “Unknown”. Jika file hidden, masuk folder “Hidden”.

**(e)** Setiap 1 file yang dikategorikan dioperasikan oleh 1 thread agar bisa berjalan secara paralel sehingga proses kategori bisa berjalan lebih cepat.

Namun karena Ayub adalah orang yang hanya bisa memberi ide saja, tidak mau bantuin buat bikin programnya, Ayub meminta bantuanmu untuk membuatkan programnya. Bantulah agar program dapat berjalan!

Catatan: 

* Kategori folder tidak dibuat secara manual, harus melalui program C

* Program ini tidak case sensitive. Contoh: JPG dan jpg adalah sama

* Jika ekstensi lebih dari satu (contoh “.tar.gz”) maka akan masuk ke folder dengan titik terdepan (contoh “tar.gz”)

* Dilarang juga menggunakan fork-exec dan system()

* Bagian b dan c berlaku rekursif

## Jawaban Nomor 3
**(a)**
Program ini mengecek banyak argumen sekaligus membuat thread dengan mengeset thread id dengan nilai jumlah_argumen-2

Jumlah argumen untuk penggunaan -f harus sesuai dengan note input, jika tidak maka akan menampilkan error message dan program ditutup dengan exit(1)

Thread akan dijalankan dengan routine, lalu program akan join setiap thread yang sudah dibuat dengan pthread_join
```
if (strcmp(argv[1], "-f") == 0) {
    if (argc <= 2) {
      printf("Argument salah\n");
      exit(1);
    }

    pthread_t tid[argc-2];
    for (int i = 2; i < argc; i++) {
      pthread_create(&tid[i-2], NULL, &routine, (void *)argv[i]);
    }
    for (int i = 2; i < argc; i++) {
      pthread_join(tid[i-2], NULL);
    }
    exit(0);
  }
```
Membuat kategori c

![image](https://user-images.githubusercontent.com/63457105/119264612-1c94a600-bc0e-11eb-8902-46604d173bff.png)


**(b)**
Program akan cek banyak argumen, dan jumlah input argumen harus sesuai dengan note input

Jika hanya fungsi if pertama yang terpenuhi, maka program akan membuat directory sesuai dengan argumen kedua menggunakan opendir dan memasukannya kedalam variabel dir, namun jika tidak terbuka maka akan menampilkan error message dan exit program dengan exit(1)
```
if (strcmp(argv[1], "-d") == 0) {
    if (argc != 3) {
      printf("Argument salah\n");
      exit(1);
    }
    DIR* dir = opendir(argv[2]);
    if (dir) {
      directory = argv[2];
    } else if (ENOENT == errno) {
      printf("Directory tidak ada\n");
      exit(1);
    }
    closedir(dir);
  }
```
hasil :

![image](https://user-images.githubusercontent.com/63457105/119264627-29b19500-bc0e-11eb-95aa-6944b8196f29.png)


**(c)**
Progrram akan cek banyak argumen dan jumlah argumen harus sesuai dengan note input
Jika hanya fungsi if pertama yang terpenuhi, maka program akan set cwd beserta sizenya dengan getcwd kedalam buffer baru yang akan dimasukkan ke variabel directory
```
 char *directory;
  if (strcmp(argv[1], "*") == 0) {
    if (argc != 2) {
      printf("Argument salah\n");
      exit(1);
    }
    char buff[1337];
    getcwd(buff, sizeof(buff));
    directory = buff;
  }
```
hasil :

![image](https://user-images.githubusercontent.com/63457105/119264633-30d8a300-bc0e-11eb-8a7a-ed6042f7792c.png)

Kendala : mengkategorikan file dengan ekstensi lebih dari satu, masalah dengan thread
