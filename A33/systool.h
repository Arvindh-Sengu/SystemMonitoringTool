#ifndef SYSTOOL_H
#define SYSTOOL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>
#include <signal.h>
#include <utmp.h>
#include <sys/types.h>


int is_integer(const char *str);
int ceil_fn(double value);
ssize_t read_full(int fd, void *buf, size_t count);


double get_total_memory(void);
double get_used_memory_gb(void);
void parse_args(int argc, char *argv[], int *cpuflag, int *memoryflag, int *usersflag, int *samples, int *tdelay);

#endif