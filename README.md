# sisop_FP  

### Soal:  
Buatlah program C yang menyerupai crontab menggunakan daemon dan thread. Ada sebuah file crontab.data untuk menyimpan config dari crontab. Setiap ada perubahan file tersebut maka secara otomatis program menjalankan config yang sesuai dengan perubahan tersebut tanpa perlu diberhentikan. Config hanya sebatas * dan 0-9 (tidak perlu /,- dan yang lainnya).  

### Jawab:   
> Check : [Full SourceCode](https://github.com/rizanw/FP_SISOP19_B02/blob/master/main.c)

### Penjelasan:   

1. Menampilkan nama dari direktori yang saat ini sedang dibuka menggunakan `getcwd`

```sh
    if(getcwd(curPath, sizeof(curPath)) == NULL){
 
       perror("[error - getcwd]");
    
       }
```

2. Menyalin dan menggabungkan 2 buah string 

```sh
    strcpy(tabfile, curPath);
    strcat(tabfile, "/crontab.data");
```

3. Menampilkan direktori yang aktif 

```sh
   if ((chdir("/")) < 0) {
        exit(EXIT_FAILURE);
    }
```

4. Menampilkan yang error

```sh
if (tmstmp->tm_sec == 0){
            FILE *confile = fopen(tabfile, "r");
            if(confile == NULL){
                perror("[error - fopen]");
```

5. Mendapatkan hari,tanggal, waktu

```sh
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
 ```
 
 6. Membuat thread
 
 ```sh
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
```

7. Untuk mengetahui apakah `exec` sudah berjalan dan sudah selesai dijalankan

```sh
void* doWork(void *args){
    char *cmd = (char *) args;
    printf("mulai exec - %s\n", cmd);
    system(cmd);
    printf("exec selesai\n");
    return NULL;
}
```

#### Reference:   
- https://github.com/rohanaq/sisop-modul-1
- https://github.com/yoshimaputri/sisop-modul-2
- https://github.com/nahdazahra/sisop-modul-3
