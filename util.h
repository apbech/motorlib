#ifndef UTIL_H
#define UTIL_H

#include "../st_device.h"
#include "core_cm4.h"

extern volatile uint32_t * const cpu_clock;

static inline uint32_t get_clock() { return *cpu_clock; }
static inline uint8_t get_cpi_count() { return DWT->CPICNT; }
static inline uint8_t get_lsu_count() { return DWT->LSUCNT; }

void ms_delay(uint16_t ms);
void ns_delay(uint16_t ns);

extern char _estack;
extern uint32_t _Min_Stack_Size;
inline uint32_t get_stack_free() {
    char *start = &_estack - (uint32_t) &_Min_Stack_Size; 
    char *count = start;
    while(!*count++); // assume zero filled
    return (count - start);
}
inline uint32_t get_stack_used() {
    return (uint32_t) &_Min_Stack_Size - get_stack_free();
}

extern char _end;
extern uint32_t _Min_Heap_Size;
inline uint32_t get_heap_free() {
    char *start = &_end + (uint32_t) &_Min_Heap_Size; 
    char *count = start;
    while(!*count--); // assume zero filled
    return (start - count);
}
inline uint32_t get_heap_used() {
    return (uint32_t) &_Min_Heap_Size - get_heap_free();
}

#define while_timeout_ms(condition, ms) while((condition) && ((get_clock() - t_start) < ms*CPU_FREQUENCY_HZ/1000))

#ifdef __cplusplus
class FrequencyLimiter {
 public:
    FrequencyLimiter(float rate_seconds) {
        t_diff_ = CPU_FREQUENCY_HZ/rate_seconds;
        last_time_ = get_clock();
    }
    // returns true once for each time it is allowed to run
    bool run() {
        uint32_t time = get_clock();
        if (time - last_time_ > t_diff_) {
            last_time_ = time; //todo needs to account for time after t_diff
            return true;
        }
        return false;
    }
 private:
    uint32_t t_diff_, last_time_;
};

template <typename T, unsigned B>
inline T signextend(const T x)
{
  struct {T x:B;} s;
  return s.x = x;
}
#endif

#endif