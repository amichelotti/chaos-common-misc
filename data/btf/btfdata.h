#ifndef __BTFDATA__
#define __BTFDATA__
#include <stdint.h>
typedef struct _btf_data {
  uint64_t acquisition;
  uint64_t trigger;
  uint32_t qdc965hi[16];
  uint32_t qdc965lo[16];
  uint32_t qdc9792[32];
  uint32_t scaler[32];
  // correctors
  double current[4]; //CHHTB001, CHHTB002,CVVTB001, CVVTB002 
} btfdata_t;
#endif
