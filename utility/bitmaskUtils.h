#ifndef BITMASKUTILS_H
#define BITMASKUTILS_H
namespace common {
    namespace utility {
#define UPMASK(bitmask,powerofTwo) \
bitmask |= powerofTwo 

#define DOWNMASK(bitmask,powerofTwo) \
bitmask=bitmask & (~powerofTwo) 

#define CHECKMASK(bitmask,powerofTwo) \
((bitmask & powerofTwo) != 0)

#define RAISEBIT(bitmask,bitToRaise) \
bitmask |= (1 << bitToRaise) 


#define DOWNBIT(bitmask,bitToLower) \
{ int mask = ~(1 << bitToLower); \
  bitmask=bitmask & mask; }

#define CHECKBIT(bitmask,bitToCheck)\
((bitmask & (1 << bitToCheck)) != 0)
    }
}
#endif