#include <libmemcached/memcached.h>
#include <string.h>
#include <stdio.h>
#include "btfdata.h"
#define USAGE \
  printf("%s <memcacheserver url> <key>\n",argv[0])

int main(int argc,char**argv){
  char * url,*pnt;
  char*key;
  size_t value_length=0;
  memcached_return_t              mc_result = MEMCACHED_SUCCESS;
  memcached_st *mc_client;

  btfdata_t *btf;
  int port=0;
  if(argc!=3){
    USAGE;
    return -1;
  }
  url=argv[1];
  pnt=strchr(url,':');
  if(pnt==NULL){
      printf("## bard url format, expected is address:port\n");
      return -1;
  }
  key=argv[2];
  *pnt=0;
  port=atoi(pnt+1);
  
   
    mc_client = memcached(NULL, 0);
    if(mc_client == NULL) {
        printf("## cannot open memcache server %s:%d\n",url,port);
        return -2;
    }
     
    mc_result = memcached_behavior_set(mc_client, MEMCACHED_BEHAVIOR_BINARY_PROTOCOL, (uint64_t)1);
     mc_result = memcached_behavior_set(mc_client, MEMCACHED_BEHAVIOR_NO_BLOCK, (uint64_t)1);
   mc_result = memcached_behavior_set(mc_client, MEMCACHED_BEHAVIOR_NOREPLY, (uint64_t)1);
    mc_result = memcached_behavior_set(mc_client, MEMCACHED_BEHAVIOR_KETAMA, (uint64_t)1);
    mc_result = memcached_behavior_set(mc_client, MEMCACHED_BEHAVIOR_TCP_NODELAY,(uint64_t)1);
    mc_result = memcached_behavior_set(mc_client, MEMCACHED_BEHAVIOR_SERVER_FAILURE_LIMIT, (uint64_t)2);
    mc_result = memcached_behavior_set(mc_client, MEMCACHED_BEHAVIOR_REMOVE_FAILED_SERVERS, (uint64_t)1);
    mc_result = memcached_behavior_set(mc_client, MEMCACHED_BEHAVIOR_RETRY_TIMEOUT,(uint64_t)2);
    mc_result = memcached_behavior_set(mc_client, MEMCACHED_BEHAVIOR_CONNECT_TIMEOUT,(uint64_t)500);
    mc_result = memcached_behavior_set(mc_client, MEMCACHED_BEHAVIOR_TCP_KEEPALIVE,(uint64_t)1);
    if((mc_result = memcached_server_add(mc_client, url,port)) != MEMCACHED_SUCCESS){
        printf("## cannot add server \n");
        return -3;
    }
    btf = (btfdata_t *)memcached_get(mc_client,
                                key,
                                strlen(key),
                                &value_length,
                                0,
                                0);
    if(btf==NULL){
        printf("## no key found \"%s\"\n",key);
        return -3;
    }
    printf("acquisition %lu\n",btf->acquisition);
    return 0;
  
}
