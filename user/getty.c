
#include "kernel/fcntl.h"

#define BUFF_SZ 512

char *argv[] = {"sh", 0};

int main(void)
{
    clrscr();
    int pid, wpid;

    char buf[BUFF_SZ];
    char USERNAME[BUFF_SZ];
    char PASSWORD[BUFF_SZ];
    int issue;
    int motd;
    int fd;
    int flag = 0;
    int uid = -1;
    char *path;
    int *gids;

    //procitas issue
    issue = open("/etc/issue",O_CREATE| O_RDONLY);
    read(issue, buf, BUFF_SZ);
    printf("%s\n", buf);

    //nabavis username i password
    //autentikacija

    while (flag == 0)
    {
        printf("USERNAME:\n");
        fd = open("/dev/console", O_RDWR);
        read(fd, USERNAME, BUFF_SZ);
        close(fd);
        USERNAME[strlen(USERNAME) - 1] = '\0';

        printf("PASSWORD:\n");

        hide();
        fd = open("/dev/console", O_RDWR);
        read(fd, PASSWORD, BUFF_SZ);
        close(fd);
        PASSWORD[strlen(PASSWORD) - 1] = '\0';

        show();

        uid = autentification(USERNAME, PASSWORD);
        //nabavi grupe.
        if (uid != -1)
        {
            flag = 1;
            path == toPath(uid);
        }
        else
        {
            printf("Username i/ili pasword je pogresan!\n");
        }

        gids=getGids(USERNAME);
    }
    //procitas motd

    motd = open("/etc/motd", O_CREATE|O_RDONLY);
    read(motd, buf, BUFF_SZ);
    printf("%s\n", buf);

    //zatvoris fajlove

    close(issue);
    close(motd);
    close(fd);

    //pokrenes shell

    pid = fork(); //forkujemo

    if (pid < 0)
    {
        printf("init: fork failed\n");
        exit();
    }
    if (pid == 0)
    {

        setgroups(gids);//dodaj grupe
        setuid(uid);
        chdir(toPath(uid));
        exec("/bin/sh", argv); //izmeniti za korisnika
        printf("init: exec sh failed\n");
        exit();
    }

    while ((wpid = wait()) >= 0 && wpid != pid)
        printf("zombie!\n");
}