#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include "functions.h"
#include "parser.h"

void printError(char* e);

int method(char *str);

int main(){
  char str[200];
  char **items; //args de entrada
  int num, splano; // num de instrucciones y indicador segundo plano
  int SIZE=5000;
  char expresion[SIZE];
  do{
    printf("myPrompt//>>");
    //scanf("%s\n",str ); //obtenemos la entrada
    fgets(str, SIZE, stdin);
    num=separaItems(str, &items, &splano);
    int execute=1;
    if(splano==1){ //si se ejecuta en segundo plano entra por acá
      //aquí no se usara wait para esperar el hijo
      execute=0;
      int h=fork(); //creamos el hijo
      if(h==0){//si es el hijo
        execute=1;
        printf("se ejecuta en segundo plano");
      }
    }
    if(execute==1){
      //si se encuentra ejecutando, este codigo no lo realiza el padre
      //el padre crea hijos que ejecutan los procesos
      //Aquí verificamos que comando se ha corrido
      if(strcmp(str,"myps")==0){
        execl("./ownGetDents.o", "/proc/", NULL);
        printf("\n");
      }else if (strcmp(str,"mygrep")==0) {
        if(num==3){//verificamos que hayan los argumentos necesarios
          MyGrep(items[1],items[2]);
          printf("\n");

        }else{
          printError(str);
        }
      }else if (strcmp(str,"psinfo")==0) {

          if(fork()==0){//creamos un hijo que ejecute la practica pasada
            execv("./psinfo.o",items);
          }

      }else if(strcmp(str,"mytime")==0){
        if(num == 1){
          MyTime();
        }else{
          printError(str);
        }
      }else if (strcmp(str,"myclear")==0) {
        if(num==1){
          MyClear();
        }else{
          printError(str);
        }
      }else if (strcmp(str,"myecho")==0) {
        if(num>1){
          char msj[1024]="";
          for(int a=1;a<num;a++){
            strcat(msj,items[a]);
            strcat(msj, " ");
          }
          MyEcho(msj);
          }else{
            printError(str);
          }
        }else if (strcmp(str,"mycopy")==0) {
          if(num==3){
            char origin[1024];
            strcpy(origin,items[1]);
            char destination[1024];
            strcpy(destination,items[2]);
            MyCp(origin, destination);
            printf("Copied file from: %s to %s\n",origin,destination );
          }else{
            printError(str);
          }
        }else if (strcmp(str,"mykill")==0) {
          if(num==3){
            int p1 = method(items[1]);
            int p2 = method(items[2]);
            MyKill(p1,p2);
          }else{
            printError(str);
          }
        }else if (strcmp(str,"mypwd")==0) {
          if(num==1){
            Mypwd();
          }else{
            printError(str);
          }
        }

    }
    liberaItems(items);
    if(splano==1 && execute==1){
      return 0;
    }
  }while(strcmp(str,"myexit")!=0);

  return 0;
}

void printError(char* err){
  printf("Error en el uso del comando, %s",err);
}

int method(char *str){
  int r;
  int p;

  r=0;
  p=1;
  while(('-'==(*str)) || ((*str)=='+')){
    if(*str=='-'){
      p=p*-1;
      str++;
    }
  }
  while((*str >= '0')&&(*str <='9')){
    r=(r*10)+ ((*str)-'0');
    str++;
  }
  return (r*p);
}
