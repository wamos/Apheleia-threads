#include <inttypes.h>


// Based on references online: http://www.mcs.anl.gov/~kazutomo/rdtsc.h 
// return number of cycles, our cycle time = 1.67 ns 
static inline uint64_t rdtsc(void) {
    uint32_t lo, hi;
    __asm__ __volatile__("xor %%eax, %%eax;" "cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));
    return (((uint64_t)hi << 32) | lo);
}
