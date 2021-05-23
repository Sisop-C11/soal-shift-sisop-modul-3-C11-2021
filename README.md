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
```
https://user-images.githubusercontent.com/68369091/119258609-a899d400-bbf4-11eb-9baf-c992d04fe801.mp4

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
