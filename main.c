#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <pthread.h>
#include <limits.h>

pthread_t tid[1001];
void* doWork(void *args);

int main() {
    char curPath[PATH_MAX], tabfile[PATH_MAX];
    if(getcwd(curPath, sizeof(curPath)) == NULL){
        perror("[error - getcwd]");
    }
    strcpy(tabfile, curPath);
    strcat(tabfile, "/crontab.data");
    
    pid_t pid, sid;
    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }
    umask(0);
    sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }
    if ((chdir("/")) < 0) {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    while(1) {
        time_t rawtime;
        struct tm *tmstmp;
        time(&rawtime);
        tmstmp = localtime(&rawtime);

        if (tmstmp->tm_sec == 0){
            FILE *confile = fopen(tabfile, "r");
            if(confile == NULL){
                perror("[error - fopen]");
            }else{
                char min[3], hour[3], mday[3], mon[3], wday[2], cmd[1000];
                int cnt = 0;
                while(EOF != fscanf(confile, "%2s %2s %2s %2s %2s %[^\r\n]", min, hour, mday, mon, wday, cmd)){
                    if (fgetc(confile) != '\n') 
                        getc(confile);
                    if((strcmp(min, "*") == 0? 1 : atoi(min) == tmstmp->tm_min) &&
                        (strcmp(hour, "*") == 0? 1 : atoi(hour) == tmstmp->tm_hour) &&
                        (strcmp(mday, "*") == 0? 1 : atoi(mday) == tmstmp->tm_mday) &&
                        (strcmp(mon, "*") == 0? 1 : atoi(mon) == (tmstmp->tm_mon + 1)) &&
                        (strcmp(wday, "*") == 0? 1 : atoi(wday) == tmstmp->tm_wday)
                    ){                        
                        printf("line ke-%d: ", cnt);
                        printf(" %2s %2s %2s %2s %2s %s \n", min, hour, mday, mon, wday, cmd);
                        int terr;
                        terr = pthread_create(&(tid[cnt]), NULL, &doWork, &cmd);
                        if(terr!=0){
                            printf("can't create thread[%d] : [%s]\n",cnt, strerror(terr));
                        }else{
                            printf("create thread[%d] success\n", cnt);
                        }
                        terr = pthread_join(tid[cnt], NULL);
                        if(terr != 0){
                            printf("can't join thread[%d] : [%s]\n",cnt, strerror(terr));
                        }else{
                            printf("join thread[%d] success\n", cnt);
                        }
                        cnt++;
                    }
                }
                fclose(confile);
            }
        }
        printf("detik ke-%d - min ke-%d \n", tmstmp->tm_sec, tmstmp->tm_min);
        sleep(1);
    }
    exit(EXIT_SUCCESS);
}

void* doWork(void *args){
    char *cmd = (char *) args;
    printf("mulai exec - %s\n", cmd);
    system(cmd);
    printf("exec selesai\n");
    return NULL;
}