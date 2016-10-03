#include <stdio.h>
int beagle_pio_configure(int pion,int dir){
  char str[256];
  FILE *f=fopen("/sys/class/gpio/export","w");
  if(f==NULL){
    printf("## cannot access to /sys/class/gpio/export, unable to configure\n");
    return -1;
  }
  fprintf(f,"%d\n",pion);
  fclose(f);
  sprintf(str,"/sys/class/gpio/gpio%d/direction",pion);
  f=fopen(str,"w");
  if(f==NULL){
    printf("## cannot access to %s, unable to configure directiob\n",str);
    return -2;
  }
  if(dir==0){
    fprintf(f,"in\n");
  } else {
    fprintf(f,"out\n");
  }
  fclose(f);
  return 0;
}
/**
   \brief set a value of an output pio
   \param pion the pio number
   \param dir 0 is input, 1 is output
   \return 0 if success
*/
int beagle_pio_set(int pion,int val){
  char str[256];
  FILE*f;
  sprintf(str,"/sys/class/gpio/gpio%d/value",pion);
  f=fopen(str,"w");
  if(f==NULL){
    printf("## cannot access to %s, unable to set pio %d\n",str,pion);
    return -2;
  }
  if(val)
    fprintf(f,"1\n");
  else
    fprintf(f,"0\n");

  fclose(f);
  return 0;
}

int beagle_pio_get(int pion,int *val){
  char str[256];
  FILE*f;
  sprintf(str,"/sys/class/gpio/gpio%d/value",pion);
  f=fopen(str,"r");
  if(f==NULL){
    printf("## cannot access to %s, unable to read pio %d\n",str,pion);
    return -2;
  }
  fscanf(f,"%d",val);
  fclose(f);
  return 0;
}
