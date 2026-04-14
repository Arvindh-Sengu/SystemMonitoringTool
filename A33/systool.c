#include "systool.h"

///_|> descry: checks if string is a valid integer
///_|> str: input string to check
///_|> returning: returns 1 if string is an integer, otherwise 0
int is_integer(const char *str) {
    if (!str || *str == '\0') return 0;
    char *end;
    strtol(str, &end, 10);
    return *end == '\0';
}

///_|> descry: computes the ceiling function output of a double
///_|> value: input double 
///_|> returning: ceiling function of the double
int ceil_fn(double value) {
    int v = (int)value;
    return (value > v) ? v + 1 : v;
}

///_|> descry: reads exactly count bytes from fd (file descriptor)
///_|> fd: file descriptor to read from
///_|> buf: buffer to store the data
///_|> count: number of bytes to read
///_|> returning: total bytes read 
ssize_t read_full(int fd, void *buf, size_t count) {
    size_t total = 0;
    while (total < count) {
        ssize_t n = read(fd, (char*)buf + total, count - total);
        if (n <= 0) return n;
        total += n;
    }
    return total;
}

///_|> descry: gets total system memory in gb
///_|> returning: total memory in gb 
double get_total_memory() {
    FILE *f = fopen("/proc/meminfo", "r");
    if (!f) { perror("meminfo"); return -1; }
    long total = 0;
    char line[256];
    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, "MemTotal: %ld kB", &total) == 1) break;
    }
    fclose(f);
    return total / (1024.0 * 1024.0);
}

///_|> descry: gets currently used system memory in gb
///_|> returning: used memory in gb 
double get_used_memory_gb() {
    FILE *f = fopen("/proc/meminfo", "r");
    if (!f) { perror("meminfo"); return -1; }
    long total=0, avail=0;
    char line[256];
    while (fgets(line, sizeof(line), f)) {
        sscanf(line, "MemTotal: %ld kB", &total);
        sscanf(line, "MemAvailable: %ld kB", &avail);
    }
    fclose(f);
    return (total - avail) / (1024.0 * 1024.0);
}

///_|> descry: parses CLAs from user to set flags
///_|> argc: argument count
///_|> argv: argument strings
///_|> cpu: pointer to cpu flag
///_|> mem: pointer to memory flag
///_|> users: pointer to users flag
///_|> samples: pointer to number of samples
///_|> tdelay: pointer to delay between samples in microseconds
///_|> returning: returnms nothing
void parse_args(int argc, char *argv[], int *cpu, int *mem, int *users, int *samples, int *tdelay) {
    *cpu = 0;
    *mem = 0;
    *users = 0;
    *samples = 20;
    *tdelay = 500000;
   
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--cpu") == 0) {
            *cpu = 1;
        } else if (strcmp(argv[i], "--memory") == 0) {
            *mem = 1;
        } else if (strcmp(argv[i], "--users") == 0) {
            *users = 1;
        } else if (is_integer(argv[i]) && i == 1) {
            *samples = atoi(argv[i]);
            if (*samples <= 0) {
                fprintf(stderr, "Error: samples must be positive (got %d)\n", *samples);
                exit(1);
            }
        } else if (is_integer(argv[i]) && i == 2) {
            *tdelay = atoi(argv[i]);
            if (*tdelay <= 0) {
                fprintf(stderr, "Error: tdelay must be positive (got %d)\n", *tdelay);
                exit(1);
            }
        }
        else if (strncmp(argv[i], "--samples=", 10) == 0) {
            *samples = atoi(argv[i] + 10);
            if (*samples <= 0) {
                fprintf(stderr, "Error: --samples must be positive (got %d)\n", *samples);
                exit(1);
            }
        }
        else if (strncmp(argv[i], "--tdelay=", 9) == 0) {
            *tdelay = atoi(argv[i] + 9);
            if (*tdelay <= 0) {
                fprintf(stderr, "Error: --tdelay must be positive (got %d)\n", *tdelay);
                exit(1);
            }
        }
        else {
            fprintf(stderr, "Error: Unknown argument '%s'\n", argv[i]);
            exit(1);
        }
    }

    if(!(*mem || *cpu || *users)) {
        *mem = 1;
        *cpu = 1;
        *users = 1;
    }
}
