#ifndef CHILD_H
#define CHILD_H

#include <stdio.h>

void child_memory(int write_fd, int samples, int tdelay);
void child_cpu(int write_fd, int samples, int tdelay);
void child_users(int write_fd, int samples, int tdelay);

#endif