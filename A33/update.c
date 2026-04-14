#include "update.h"
///_|> descry: this function updates the memory graph by printing the memory usage at each sample point.
///_|> i: The current sample index, type integer
///_|> end: the last line to print memory data on, type integer
///_|> offset: the offset for printing the graph, type integer
///_|> line: the current line for printing, type integer
///_|> total: the total amount of RAM in GB, type double
///_|> curr: the current used RAM in GB, type double ptr 
///_|> returning: this function does not return anything
void update_memory_graph(int i, int end, int offset, int line, double total, double curr) {
    printf("\x1b[%d;1H>>> Memory   %.2f GB ", line, curr);
    int y = end - ((curr / total) * 12);
    if (y < 1) y = end-1;
    printf("\x1b[%d;%dH#", y, offset + i);
}

///_|> descry: this function updates the CPU graph by printing the CPU usage at each sample point.
///_|> i: the current sample index, type integer
///_|> end: the last line to print CPU data on, type integer
///_|> offset: the offset for printing the graph, type integer
///_|> line: the current line for printing, type integer
///_|> curr: the current CPU usage in percentage, type double ptr
///_|> returning: this function does not return anything
void update_cpu_graph(int i, int end, int offset, int line, double curr) {
    printf("\x1b[%d;1H>>> CPU   %.2f%% ", line, curr);
    int y = end - ((curr / 100.0) * 10);
    if (y < 1) y = 1;
    printf("\x1b[%d;%dH:", y, offset + i);
}

///_|> descry: this functions updates the users chart 
///_|> end: the last line of users on the chart, type integer
///_|> str: buffer of user data to be updated, type const char ptr
///_|> currline: the last line of the program, type integer ptr
///_/> len: length of string in bytes, type integer
///_|> returning: this functions returns nothing
void update_userschart(int end, const char *str, int *currline, int len) {
    printf("\x1b[%d;1H", end);
    printf("\x1b[s"); printf("\x1b[B"); printf("\x1b[J"); printf("\x1b[u");
    printf("\x1b[%d;1H%s", end, str);

    int lines = 0;
    for (int i = 0; i < len; i++) if (str[i]=='\n') lines++;
    if (len>0 && str[len-1]!='\n') lines++;

    printf("\x1b[%d;1H", end + lines);
    printf("----------------------------------------\n");

    *currline = end + lines + 1;
}