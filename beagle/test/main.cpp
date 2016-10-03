#include <stdio.h>
#include "beagle_pio.h"
#include <stdlib.h>
int main(int argc,char**argv){
  int pio,val;
  if(argc<2){
    printf("Usage %s <pio> [data]\n",argv[0]);
    return -1;
  }
  pio=atoi(argv[1]);
  if(argc==2){
    if(beagle_pio_configure(pio,0)!=0){
      printf("## error configuring read\n");
      return -4;
    }
    if(beagle_pio_get(pio,&val)!=0){
      printf("## error reading\n");
      return -5;
    }
    printf("[%d] val=%d\n",pio,val);
  } else {
    if(beagle_pio_configure(pio,1)!=0){
      printf("## error configuring write\n");
      return -4;
    }
    val =atoi(argv[2]);
     if(beagle_pio_set(pio,val)!=0){
      printf("## error writing\n");
      return -5;
    }
  }
  return 0;
}
