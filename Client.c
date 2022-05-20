#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>

int ClientMain(){
	int s, status, c, e, r;
	struct addrinfo hints, *res;
	FILE *fisier;

	//socket client
	s=socket(AF_INET6, SOCK_STREAM, 0);//AF_INET6 selecteaza protocolul pe baza socket-ului folosit; SOCK_STREAM - socket de tip TCP
	if(s==-1)
	{
		printf("Socket necreat\n");
	} else{
		printf("\nSocket creat");
	}

	memset(&hints, 0, sizeof hints);//hints - indicator spre blocul de memorie
	hints.ai_family = AF_INET6;//selecteaza IPv6
	hints.ai_socktype = SOCK_STREAM;//socket de tip TCP
	status = getaddrinfo("www.afrinic.net", "80", &hints, &res);//returneaza adresa IPv6, opt-zero reprezinta portul pentru http
	
	if(status==0){
		printf("\nFunctioneaza");
	}else{
		printf("\nNu functioneaza");
	}
	//conect - conecteaza socket-ul la adresa specificata
	
	c=connect(s, res->ai_addr, res->ai_addrlen);
	if(c==-1){//returneaza -unu in caz de eroare
		printf("\nEroare");
	}else{
		printf("\nConexiunea functioneaza");
	}

	char *mesaj = "GET / HTTP/1.0\r\n\r\n";
	//send - trimite un mesaj serverului; zero - seteaza flag-urile in zero
	r=send(s, mesaj, strlen(mesaj), 0);

	if(r==-1){
		printf("\nMesaj netrimis");
	}
	else{
		printf("\nMesaj trimis\n");
	}

	char buff[2000];
	memset(buff, 0, 2000);

	fisier = fopen("fisier.html", "w"); //deschidem fiserul, w pentru scriere
	if(fisier==NULL){ //verifica existenta fisierului
		printf("Fiserul nu exista");
	}
	else{
		printf("Fisier deschis cu succes");
	}
	//verifica primirea datelor
 
	while(1)
	{
		r=recv(s, buff, 2000-1, 0);//primeste datele de la server
		if(r==-1){//verifica daca se primesc informatii de la server
			printf("\nEroare");
			break;
		}

		if(r==0){
			printf("\nConexiune incheiata");
			break;
		}
 	
		fprintf(fisier, buff);//scrie continutul buffer-ului in interiorul fisierului
	}

	e=close(s);
	
	if(e==-1){
		printf("\nEroare la inchiderea conexiunii");
	}
	else{
		printf("\nConexiunea a fost inchisa");
	}

	return 0;
}