#ifndef UPDATE_H
#define UPDATE_H

#include <stdio.h>

// ---------------- Updates ----------------
void update_memory_graph(int i, int mem_endline, int graph_offset, int curr_mem_line, double total_ram_gb, double curr_ram_gb);
void update_cpu_graph(int i, int cpu_endline, int graph_offset, int curr_cpu_line, double curr_cpu_usage);
void update_userschart(int users_endline, const char *users_str, int *currline, int len);

#endif