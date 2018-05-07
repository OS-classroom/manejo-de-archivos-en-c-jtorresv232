//includes
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/stat.h>
#include <semaphore.h>

//declaracion de metodos

void MyKill(int s, int ipid);
void Mypwd();
void MyTime();
void MyEcho(const char *text);
void MyGrep(char* cadena,char* path);
int MyCp(const char *origin, const char *destination);
void MyClear();


//implementacion de metodos

void MyKill(int s, int kpid){
	pid_t pid=kpid;
	int i = kill(pid);
	kill(pid,s);
	printf("La señal %d ha sido enviada #proeso: %d\n",i,kpid );
}

void Mypwd(){
	char path[1024];
	if(getcwd(path, sizeof(path))!= NULL){
		printf("%s\n",path );
	}
}

void MyTime(){
	time_t mytime;
	struct tm * timeinfo;
	time(&mytime);
	timeinfo=localtime(&mytime);
	printf("%s\n",asctime(timeinfo) );
}

void MyEcho(const char *text){
	printf("%s\n",text );
}

void MyGrep(char* cadena, char* path){
	char* linea=""; // aquí guardaremos cada una de las lineas del archivo para comparar

	size_t len=0;
	ssize_t r;

	//abrimos el archivo con la ruta especificada en los parametros
	FILE *fp;
	fp= fopen(path,"r"); //solo lectura
	if(fp==NULL){ //verificamos que se haya abierto correctamente el archivo
		printf("Ha ocurrido un error abriendo el archivo, verifique la ruta indicada");
	}
	while((r=getline(&linea,&len,fp))!= -1){
		//se utiliza la funcion getline de stdio para leer el archivo por lineas
		//verificamos que la linea leida contenga la palabra con strstr que busca la primera ocurrencia
		if(strstr(linea,cadena)!= NULL){
			printf("%s\n",linea );
		}
	}
}

int MyCp(const char *origin, const char *destination){
	int fd_dest, fd_origin;
	char buffer[4096];
	ssize_t r; //size leido
	int s_errnum;

	//tratamos de abrir los archivos de origen y destination
	fd_origin=open(origin, O_RDONLY);
	if(fd_origin<0){ //verificamos errores
		//fue error por lo tanto retornamos error
		printf("error abriendo el archivo de origen\n" );
		return -1;
	}
	//archivo de destination
	fd_dest=open(destination,O_WRONLY | O_CREAT);
	if(fd_dest<0){
		printf("destino");
		goto out_error;
	}

	while(r = read(fd_origin, buffer, sizeof buffer), r>0){
		//mientras el archivo no acabe
		char *out_ptr=buffer;
		ssize_t w; // size escrito
		do{
			w=write(fd_dest, out_ptr,r); //escribimos en el archivo destination
			if (w>=0) {
				r-=w; //leido - escrito
				out_ptr += w;
			}else if(errno != EINTR){
				goto out_error;
			}
		}while(r>0); //fin do while

		if (r==0) {
			if (close(fd_dest)<0) {
				fd_dest=-1;
				goto out_error; //no se pudo escribir
			}

			close(fd_origin); //cerramos archivo origen

			return 0; //retornamos 0 en caso de exito
		}

		out_error:
			s_errnum=errno;
			close(fd_origin);
			if(fd_dest>=0){
				close(fd_dest);
			}

			errno=s_errnum;
			return -1;
	}
}

void MyClear(){
	const char *CLEARS="\e[1;1H\e[2J";
	write(STDOUT_FILENO,CLEARS,12);
}
