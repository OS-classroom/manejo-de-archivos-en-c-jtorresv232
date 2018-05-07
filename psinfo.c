#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct proceso{
char name[40];
char state[40];
char id[40];
};

typedef struct proceso proceso_d;

//declaracion de metodos
proceso_d* crearProceso(char nombre[40], char estado[40], char pid[40]);
void imprimirProceso(proceso_d* proceso);
void psinfo(char pid[40]);
bool inside(char var[40],char *vars[2]);
FILE *openfile(char path[40]);
void listar(char pid[40]);
void hacerCadena(char cadena[], char pid[40], char ids[50]);
void escribirCadena(char cadena[], proceso_d* proceso);

int main(int argc, char *argv[]){

char *arguments[2]={"-l","-r"};
char cadena[1000]="----REPORTE DE PROCESOS----\n\n";
char pids[50]="";
int index=1;

while(argv[index]!=0){

if(strcmp(argv[index],"-l")==0){
    index++;
    while(argv[index]!=0 && !inside(argv[index],arguments)){
        listar(argv[index]);
        index++;
        continue;
    }
      
    continue;

}else if(strcmp(argv[index],"-r")==0){
    index++;
    while(argv[index]!=0 && !inside(argv[index],arguments)){
        //printf("hola estoy en el while");
        hacerCadena(cadena,argv[index],pids);
        index++;
        continue;
    }
    //creamos nuevo archivo
    
    char path[20]= "/home/";
    strcat(path,getenv("USER"));
    strcat(path,"/");
    strcat(path,"psinfo-report");
    strcat(path,pids);
    strcat(path, ".info");
    FILE *myfile;
    printf("%s\n",path);
    myfile=fopen(path,"w");
    //fflush(stdout);
    //fflush(stdin);
    printf("%s\n",getenv("HOME"));
    //fwrite(cadena,1,sizeof(cadena),myfile);
    fprintf(myfile,"%s", cadena);
    fclose(myfile);
    printf("Se ha creado el archivo %s\n\n",path);
    continue;
}else if(argv[index]!=0 && index == 1){
while(argv[index]!=0 && !inside(argv[index],arguments)){
psinfo(argv[index]);
index++;
continue;}// fin while
}//fin elseif
else{printf("ERROR 400, command not found"
"\nel comando %s no existe, por favor ingrese un comando existente"
"\n\nutilice -l para listar los procesos"
"\n\nutilice -r para generar un reporte de los procesos en la carpeta home\n\n",argv[index]);
exit(0);}
}// fin while principal

}//fin main

void hacerCadena(char cadena[],char pid[40], char ids[50]){

    //conseguir informacion para el archivo
    char filename[20]= "/proc/";
    strcat(filename,pid);
    strcat(filename, "/status");
    FILE *fp = openfile(filename);
    char info[40];
    char value[40];
    char nombre[40];
    char estado[40];
    char ID[40];
    char texto[]="";
    //procesamos el archivo donde se encuentran los datos
    while(fscanf(fp,"%s%*c %[^\n]",info,value)!=EOF){
        if(strcmp(info,"Name:")==0){strcpy(nombre,value);}
        if(strcmp(info,"State:")==0){strcpy(estado,value);}
        if(strcmp(info,"Pid:")==0){strcpy(ID,value);}
    }
    fclose(fp);
    proceso_d *proceso=crearProceso(nombre,estado,ID);
    
    //agregamos el texto a nuestra cadena del reporte
    escribirCadena(texto,proceso);
    free(proceso);
    strcat(cadena,texto);
    strcat(ids,"-");
    strcat(ids,ID);
    
   
}


//constructor clase proceso_d con parametros
proceso_d* crearProceso(char nombre[40], char estado[40], char pid[40]){
    proceso_d* proceso = malloc(sizeof(proceso_d));
    strcpy(proceso->name,nombre);
    strcpy(proceso->state, estado);
    strcpy(proceso->id, pid);

    return proceso;
}

void listar(char pid[40]){
    char filename[20]= "/proc/";
    strcat(filename,pid);
    strcat(filename, "/status");
    FILE *fp = openfile(filename);
    char info[40];
    char value[40];
    char nombre[40];
    char estado[40];
    char ID[40];
    //procesamos el archivo
    while(fscanf(fp,"%s%*c %[^\n]",info,value)!=EOF){
        if(strcmp(info,"Name:")==0){strcpy(nombre,value);}
        if(strcmp(info,"State:")==0){strcpy(estado,value);}
        if(strcmp(info,"Pid:")==0){strcpy(ID,value);}
    }
    fclose(fp);
    proceso_d *proceso=crearProceso(nombre,estado,ID);
    imprimirProceso(proceso);
    free(proceso);
}

void imprimirProceso(proceso_d* proceso){
    printf("Nombre: %s\n"
    "ID: %s\n"
    "Estado: %s\n\n ... \n\n",proceso->name,proceso->id,proceso->state);
}

void escribirCadena(char cadena[],proceso_d* proceso){
   
    strcat(cadena, "Nombre: ");
    strcat(cadena, proceso->name);
    strcat(cadena, "\n");
    strcat(cadena, "ID: ");
    strcat(cadena, proceso->id);
    strcat(cadena, "\n");
    strcat(cadena, "Estado: ");
    strcat(cadena, proceso->state);
    strcat(cadena, "\n\n ... \n\n");
}


void psinfo(char pid[40]){
//conseguir el nombre del archivo
char filename[20]= "/proc/";
strcat(filename,pid);
strcat(filename, "/status");

//abrir archivo
FILE *fp;
char info[40];
char value[40];
char name[40];
char estado[40];
char tmemoria[20];
char ttext[20];
char tdata[20];
char tstack[20];
char cambiosv[20];
char cambiosnv[20];
fp = fopen(filename,"r");
if(fp==NULL){
    printf("ERROR 404, por favor verifique que el proceso exista\n"
	
	"puede verificar los procesos existentes con el comando ps"
	"\nverifique que haya ingresado comandos validos (-l  -r)");
    exit(1);    
}

//procesar archivo
while(fscanf(fp,"%s%*c %[^\n]",info,value)!=EOF){
    if(strcmp(info,"VmSize:")==0){strcpy(tmemoria,value);}
    if(strcmp(info,"VmStk:")==0){strcpy(tstack,value);}
    if(strcmp(info,"VmExe:")==0){strcpy(ttext,value);}
    if(strcmp(info,"VmData:")==0){strcpy(tdata,value);}
    if(strcmp(info,"Name:")==0){strcpy(name,value);}
    if(strcmp(info,"State:")==0){strcpy(estado,value);}
    if(strcmp(info,"voluntary_ctxt_switches:")==0){strcpy(cambiosv,value);}
    if(strcmp(info,"nonvoluntary_ctxt_switches:")==0){strcpy(cambiosnv,value);}
}

printf("1. Nombre del proceso:  %s\n"
"2. Estado del proceso: %s\n"
"3. Tamaño total de la imagen de memoria: %s\n"
"   a. Tamaño sección TEXT: %s\n"
"   b. Tamaño seccion DATA: %s\n"
"   c. Tamaño de la sección STACK: %s\n"
"4. Cambios realizados (voluntarios - no voluntarios): %s - %s\n\n ... \n\n",
name,estado,tmemoria,ttext,tdata,tstack,cambiosv,cambiosnv);

fclose(fp);
}


bool inside(char *var, char *vars[]){
    for(int i = 0; i < 2; i++){
        if(strcmp(var,vars[i])==0){return true;}
    }
return false;
}


FILE *openfile(char path[40]){
    FILE *fp;
    fp=fopen(path,"r");
    if(fp==NULL){printf("ERROR 404, por favor verifique que el proceso exista\n"
	"puede verificar los procesos existentes con el comando ps");
    exit(0);}
    return fp;
    
}
