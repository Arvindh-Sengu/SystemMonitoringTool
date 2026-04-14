#include "draw.h"

///_|> descry: draws an empty memory usage graph
///_|> samples: number of sample columns to display, type int
///_|> currline: pointer to current cursor line in terminal, type int ptr
///_|> endline: pointer to store the last line of the graph, type int ptr
///_|> total: total system memory in GB, type double
///_|> curr: current used memory in GB, type double
///_|> returning: returns nothing
void draw_empty_memorygraph(int samples, int *currline, int *endline, double total, double curr) {
    printf(">>> Memory   %.2lf GB\n", curr);
    printf("%.2lf GB|\n", total);
    int height = 11;

    for(int i = 0; i < height; i++) {
        printf("        |");
        for(int j = 0; j < samples; j++) printf(" ");
        printf("\n");
    }
    printf("0 GB    <");
    for(int h = 0; h < samples; h++) printf("-");
    printf("\n");

    *endline = *currline + 2 + height;
    *currline = *endline + 2;
    printf("\x1b[%d;%df", *currline, 1);
}

///_|> descry: draws an empty CPU usage graph
///_|> samples: number of sample columns to display, type int
///_|> currline: pointer to current cursor line in terminal, type int ptr
///_|> endline: pointer to store the last line of the graph, type int ptr
///_|> curr: current CPU usage percentage, type double
///_|> returning: returns nothing
void draw_empty_cpugraph(int samples, int *currline, int *endline, double curr) {
    printf(">>> CPU   %.2lf%%\n", curr);
    printf("100%%    |\n");
    int height = 9;

    for(int i = 0; i < height; i++) {
        printf("        |");
        for(int j = 0; j < samples; j++) printf(" ");
        printf("\n");
    }
    printf("0 %%     <");
    for(int h = 0; h < samples; h++) printf("-");
    printf("\n");

    *endline = *currline + 2 + height;
    *currline = *endline + 2;
    printf("\x1b[%d;%df", *currline, 1);
}

///_|> descry: draws an empty users/sessions chart
///_|> currline: pointer to current cursor line in terminal, type int ptr
///_|> endline: pointer to store the last line of the chart, type int ptr
///_|> returning: returns nothing
void draw_empty_userschart(int *currline, int *endline) {
    printf("----------------------------------------\n");
    printf("### Sessions/users ###\n");

    *endline = *currline + 2;
    *currline = *endline + 2;

    printf("\x1b[%d;%df", *endline + 1, 1);
}