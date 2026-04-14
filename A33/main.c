#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "systool.h"
#include "draw.h"
#include "update.h"
#include "child.h"

void sigint_handler(int sig){
    (void)sig; // suppress unused parameter warning
    signal(SIGINT, SIG_IGN);

    char c[10];
    printf("\nQuit? (y/n): ");
    fflush(stdout);

    if(fgets(c,10,stdin) && (c[0]=='y'||c[0]=='Y')){
        kill(0,SIGTERM);
        exit(0);
    }

    signal(SIGINT, sigint_handler);
}

void sigtstp_handler(int sig){}

int main(int argc,char *argv[]) {

    struct sigaction sa1,sa2;
    sa1.sa_handler=sigint_handler;
    sigemptyset(&sa1.sa_mask);
    sigaction(SIGINT,&sa1,NULL);

    sa2.sa_handler=sigtstp_handler;
    sigemptyset(&sa2.sa_mask);
    sigaction(SIGTSTP,&sa2,NULL);

    struct sigaction sa_tstp;
    sa_tstp.sa_handler = SIG_IGN; 
    sigemptyset(&sa_tstp.sa_mask);
    sa_tstp.sa_flags = 0;
    sigaction(SIGTSTP, &sa_tstp, NULL);

    int cpu,mem,users,samples,tdelay;
    parse_args(argc,argv,&cpu,&mem,&users,&samples,&tdelay);

    int mp[2],cp[2],up[2];
    pid_t mpid=-1,cpid=-1,upid=-1;

    if(mem){
        if(pipe(mp) == -1){ perror("pipe"); exit(1);}
        if((mpid=fork())==0){
            signal(SIGINT, SIG_IGN);
            close(mp[0]);
            child_memory(mp[1],samples,tdelay);
        }
        close(mp[1]);
    }

    if(cpu){
        if(pipe(cp) == -1){ perror("pipe"); exit(1);}
        if((cpid=fork())==0){
            signal(SIGINT, SIG_IGN);
            close(cp[0]);
            child_cpu(cp[1],samples,tdelay);
        }
        close(cp[1]);
    }

    if(users){
        if(pipe(up) == -1){ perror("pipe"); exit(1);}
        if((upid=fork())==0){
            signal(SIGINT, SIG_IGN);
            close(up[0]);
            child_users(up[1],samples,tdelay);
        }
        close(up[1]);
    }

    printf("\033[2J\033[H");
    printf("Nbr of samples: %d every %d microseconds (%.3f seconds)\n", samples, tdelay, tdelay / 1000000.0);

    int currline = 2, endline = 1;
    int mem_endline = 0, cpu_endline = 0, users_endline = 0;
    int curr_mem_line = 0, curr_cpu_line = 0;

    double total = get_total_memory();

    if(mem){
        draw_empty_memorygraph(samples, &currline, &endline, total, get_used_memory_gb());
        mem_endline = endline;
        curr_mem_line = 2;
    }

    if(cpu){
        curr_cpu_line = currline;
        draw_empty_cpugraph(samples, &currline, &endline, 0);
        cpu_endline = endline;
    }

    if(users){
        draw_empty_userschart(&currline, &endline);
        users_endline = endline;
    }

    printf("\x1b[%d;1H", currline);

    for(int i=0;i<samples;i++){
        double mv,cv;
        char *ustr=NULL;

        if(mem){
            read_full(mp[0],&mv,sizeof(mv));
            update_memory_graph(i,mem_endline,10,curr_mem_line,total,mv);
        }
        if(cpu){
            read_full(cp[0],&cv,sizeof(cv));
            update_cpu_graph(i,cpu_endline,10,curr_cpu_line,cv);
        }
        if(users){
            uint32_t len;
            read_full(up[0],&len,sizeof(len));
            ustr=malloc(len+1);
            read_full(up[0],ustr,len);
            ustr[len]=0;
            update_userschart(users_endline,ustr,&currline,len);
            free(ustr);
        }

        fflush(stdout);
        printf("\x1b[%d;%df", currline, 1);
        usleep(tdelay);
    }

    if(mem){ close(mp[0]); waitpid(mpid,NULL,0); }
    if(cpu){ close(cp[0]); waitpid(cpid,NULL,0); }
    if(users){ close(up[0]); waitpid(upid,NULL,0); }

    return 0;
}