/*
Sintaksa:
groupadd [-g gid] naziv_grupe

Program groupadd se koristi za dodavanje nove grupe korisnika. Naziv grupe se navodi kao obavezan poslednji argument na komandnoj liniji.

Ako se navede opcija koja nije podržana, program prijavljuje grešku i završava se bez kreiranja grupe.

Opcija -g se koristi za navođenje GID za grupu. Ova vrednost mora biti jedinstvena. U slučaju da nije, program prijavljuje grešku i završava se bez kreiranja grupe. Default vrednost za ovaj parametar je najmanji broj koji je veći od 1000 i nije već zauzet.

Ako program ne može da pristupi /etc/group datoteci, prijavljuje grešku i završava se bez kreiranja grupe.
*/
#include "kernel/fcntl.h"

int main(int argc, char *argv[])
{
    int group;

    char buf[512];
    char *GROUPNAME;
    char *GID = getFreeGid();

    int mode = 0;

    if (argc < 2 || argc > 4 || argc % 2 != 0)
    {
        printf("neispravan format, pravilan format: groupadd [-g gid] naziv_grupe\n");
        exit();
    }

    GROUPNAME = argv[argc - 1];

    for (int i = 1; i < argc - 1; i++)
    {
        if (i % 2 == 1)
        {
            char *c = argv[i];

            if (strcmp(c, "-g") == 0)
            {
                mode = 1;
            }
            else
            {
                printf("neispravan format, pravilan format: useradd [-d dir] [-u uid] [-c name] login\n");
                exit();
            }
        }
        else
        {
            switch (mode)
            {
            case 1:
                GID = argv[i];
                break;
            default:
                printf("neispravan format, pravilan format: useradd [-d dir] [-u uid] [-c name] login\n");
                exit();
                break;
            }
        }
    }

    if (GroupExists(GROUPNAME)==1){
        printf("Grupa sa nazivom: %s vec postoji\n",GROUPNAME);
        exit();
    }

    if(GIDExists(GID)==1){
        printf("Grupa sa GID-om: %d vec postoji\n",GID);
        exit();
    }

    group = open("/etc/group", O_CREATE | O_RDWR);

    read(group, buf, 512);

    write(group, GROUPNAME, strlen(GROUPNAME));
    write(group, ":", 1);
    char *buffer;
    itoa(GID,buffer,10);
    write(group, buffer, strlen(buffer));
    write(group, ":\n", 2);

    close(group);
    exit();
}