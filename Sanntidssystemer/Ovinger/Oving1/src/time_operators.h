#include "sys/times.h"
struct timespec timespec_add(struct timespec lhs, struct timespec rhs);

struct timespec timespec_normalized(time_t sec, long nsec);

struct timespec timespec_sub(struct timespec lhs, struct timespec rhs);


struct timespec timespec_add(struct timespec lhs, struct timespec rhs);

int timespec_cmp(struct timespec lhs, struct timespec rhs);


int cmpStrings(char* A, char* B);


void busy_wait(); 

long diffTime(struct tms A, struct tms B);