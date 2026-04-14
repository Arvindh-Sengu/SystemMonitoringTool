#include "child.h"
#include "systool.h"
#include <utmp.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

///_|> descry: this function monitors memory usage in a child process
///_|> fd: pipe file descriptor to write the memory usage data, type int
///_|> samples: number of memory samples to collect, type int
///_|> tdelay: delay between samples in microseconds, type int
///_|> returning: returns nothing
void child_memory(int fd, int samples, int tdelay) {
    for(int i=0; i<samples; i++){
        usleep(tdelay);
        double val = get_used_memory_gb();
        write(fd, &val, sizeof(val));
    }
    close(fd);
    exit(0);
}

///_|> descry: this function monitors CPU usage in a child process
///_|> fd: pipe file descriptor to write the CPU usage data, type int
///_|> samples: number of CPU samples to collect, type int
///_|> tdelay: delay between samples in microseconds, type int
///_|> returning: returns nothing
void child_cpu(int fd, int samples, int tdelay) {
    unsigned long long u,n,s,i,iw,irq,si;

    FILE *f = fopen("/proc/stat","r");
    fscanf(f,"cpu %llu %llu %llu %llu %llu %llu %llu",&u,&n,&s,&i,&iw,&irq,&si);
    fclose(f);

    unsigned long long prev_total = u+n+s+i+iw+irq+si;
    unsigned long long prev_idle = i;

    for(int k=0; k<samples; k++){
        usleep(tdelay);

        f = fopen("/proc/stat","r");
        fscanf(f,"cpu %llu %llu %llu %llu %llu %llu %llu",&u,&n,&s,&i,&iw,&irq,&si);
        fclose(f);

        unsigned long long total = u+n+s+i+iw+irq+si;
        unsigned long long idle = i;

        double usage = 100.0 * ( (total-prev_total)-(idle-prev_idle) ) / (total-prev_total);
        write(fd,&usage,sizeof(usage));

        prev_total = total;
        prev_idle = idle;
    }
    close(fd);
    exit(0);
}

///_|> descry: this function monitors logged-in users in a child process
///_|> fd: pipe file descriptor to write user session data, type int
///_|> samples: number of samples to collect, type int
///_|> tdelay: delay between samples in microseconds, type int
///_|> returning: returns nothing
void child_users(int fd, int samples, int tdelay) {
    for(int i=0; i<samples; i++) {
        usleep(tdelay);

        char buf[10000] = {0};
        int offset = 0;

        struct utmp *entry;
        setutent();

        while ((entry = getutent()) != NULL) {
            if (entry->ut_type == USER_PROCESS || entry->ut_type == LOGIN_PROCESS) {
                offset += snprintf(buf + offset,
                                   sizeof(buf) - offset,
                                   "%-10s %10s%s",
                                   entry->ut_user,
                                   entry->ut_line,
                                   entry->ut_host[0] ? entry->ut_host : " ()");
                offset += snprintf(buf + offset, sizeof(buf) - offset, "\n");
                if (offset >= (int)sizeof(buf) - 1) break;
            }
        }

        endutent();

        uint32_t len = offset;
        write(fd, &len, sizeof(len));
        write(fd, buf, len);
    }
    close(fd);
    exit(0);
}