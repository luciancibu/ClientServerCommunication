#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include "client.c"

int main(){
	int sockfd, rez, new_fd, len, bytes_sent, dimensiune;
	char buffer[100], *msg = "Comanda neimplementata";
	struct addrinfo hints, *res, sockaddr_storage, their_addr;
	socklen_t addr_size;
	FILE *fp;

	//seteaza parametri socket-ului
	memset(&hints, 0, sizeof (hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	getaddrinfo(NULL, "22128", &hints, &res);//atribuire port de conectare
	sockfd=socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	
	//leaga socket-ul de port
	if(sockfd==-1)
	{
		printf("Socket necreat\n");
		exit(1);
	}
	
	printf("\nSocket creat");	
	rez=bind(sockfd, res->ai_addr, res->ai_addrlen);

	if(rez==-1){
		printf("Eroare bind");
		exit(1);
	}
	
	printf("\nMerge bind");

	rez=listen(sockfd, 1);//asculta cererile de conectare, nr de clienti in asteptare
	if(rez==-1){
		printf("Eroare listen");
		exit(1);	
	}
	
	printf("\nMerge listen\n");


	addr_size = sizeof their_addr;
	new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);//accepta conexiuni

	if(new_fd==-1){
		printf("\nEroare accept");
		perror("accept: ");
		exit(1);	
	}
	printf("\nClient conectat");

	do{
		dimensiune=recv(new_fd, buffer, sizeof(buffer), 0);

		if(dimensiune==0){
			perror("Client deconectat\n");
			break;
		}
		if(dimensiune==-1){
			printf("\nEroare recv");
			perror("receive: ");
			break;
		}

		//verifica daca comanda primita este unu-cinci#
	
		if(dimensiune==3 && buffer[0]=='1' && buffer[1]=='5' && buffer[2]=='#'){

			ClientMain();
			fp = fopen("fisier.html", "r");//deschide fisierul

			while(!feof(fp)){ //citeste linie cu linie pana ajunge la finalul fisierului
				fscanf(fp, "%s", msg);
				bytes_sent = send(new_fd, msg, len, 0);//trimite datele clientului IPv4
		
				if(bytes_sent==-1){
					perror("\nEroare trimitere mesaj");
					break;
				}
			}
		printf("\nMesaje trimise");
		}else{
			len = strlen(msg);
			bytes_sent = send(new_fd, msg, len, 0);
			
			if(bytes_sent==-1){
				perror("\nComanda neimplementata");//afiseaza acest mesaj in cazul in care comanda nu este unu-cinci#
				break;
			}
			printf("\nMesaj trimis");	
		}
	}while(dimensiune>0);
	close(new_fd);
	close(sockfd);
}