#ifndef DRAW_H
#define DRAW_H

#include <stdio.h>

void draw_empty_memorygraph(int samples, int *currline, int *endline, double total, double curr);
void draw_empty_cpugraph(int samples, int *currline, int *endline, double curr);
void draw_empty_userschart(int *currline, int *endline);

#endif