#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>

#define _GNU_SOURCE
#define h_err(msg)\
      do {perror(msg);exit(EXIT_FAILURE);}while(0)


struct linux_dirent{
  long d_ino;
  off_t d_off;
  unsigned short d_reclen;
  char d_name[];
};

#define BUF_SIZE 1024*1024*5

int main(int argc, char *argv[]){
  int fd, r;
  char buf[BUF_SIZE];
  struct linux_dirent *d;
  int bpos;
  //Abrimos el archivo proc donde se encuentran los datos
  fd=open("/proc/", O_RDONLY | O_DIRECTORY);

  if(fd==-1){//verificamos que se haya abierto
    h_err("open");
  }

  for( ; ; ) {
    //leemos con llamada al systema getdents
    r=syscall(SYS_getdents,fd,buf,BUF_SIZE);
    if(r==-1){//verificamos que no sea errores
      h_err("getdents");
    }
    if (r==0) {//exito
      break;
    }

    for(bpos=0;bpos<r;){
      d=(struct linux_dirent *) (buf+bpos);
      if(d->d_ino!=0) printf("%s\n", (char *) d->d_name);
      bpos += d->d_reclen;
    }
  }

  exit(EXIT_SUCCESS);


}
