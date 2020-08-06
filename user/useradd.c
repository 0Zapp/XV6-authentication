/*
Sintaksa:
useradd [-d dir] [-u uid] [-c name] login

Program useradd se koristi za dodavanje novog korisnika u sistem. Korisničko ime za novog korisnika se navodi kao obavezan poslednji argument na komandnoj liniji. Ovaj program takođe automatski kreira novu grupu za ovog korisnika koja se zove isto kao korisnik, kao i njegov home direktorijum.

Ovaj program ne podešava šifru za korisnika. 

Ako se navede opcija koja nije podržana, program prijavljuje grešku i završava se bez kreiranja korisnika.

Opcija -d se koristi za navođenje direktorijuma koji će se koristiti kao home za ovog korisnika. Default vrednost je /home/korisnicko_ime. Ako navedeni direktorijum ne postoji, kreirati ga. Ako dođe do greške pri kreiranju direktorijuma, prijaviti grešku i završiti program bez kreiranja korisnika.

Opcija -u se koristi za navođenje UID za korisnika. Ova vrednost mora biti jedinstvena. U slučaju da nije, program prijavljuje grešku i završava se bez kreiranja korisnika. Default vrednost za ovaj parametar je najmanji broj koji je veći od 1000 i nije već zauzet.

Opcija -c se koristi za navođenje pravog imena korisnika.

Ako program ne može da pristupi /etc/passwd ili /etc/group datoteci, prijavljuje grešku i završava se bez kreiranja korisnika.
*/
#include "kernel/fcntl.h"

int main(int argc, char *argv[])
{
    int passwd;
    int group;

    char buf[512];
    char *USERNAME;
    char *PASSWORD = "password";
    char *UID = getFreeUid();
    char *GID;
    char *FULLNAME;
    char *HOMEDIR;

    int mode = 0;

    if (argc < 2 || argc > 8 || argc % 2 != 0)
    {
        printf("neispravan format, pravilan format: useradd [-d dir] [-u uid] [-c name] login\n");
        exit();
    }

    USERNAME = argv[argc - 1];

    for (int i = 1; i < argc - 1; i++)
    {
        if (i % 2 == 1)
        {
            char *c = argv[i];

            if (strcmp(c, "-d") == 0)
            {
                mode = 1;
            }
            else if (strcmp(c, "-u") == 0)
            {
                mode = 2;
            }
            else if (strcmp(c, "-c") == 0)
            {
                mode = 3;
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
                HOMEDIR = argv[i];
                break;

            case 2:
                UID = argv[i];
                break;
            case 3:
                FULLNAME = argv[i];
                break;
            default:
                printf("neispravan format, pravilan format: useradd [-d dir] [-u uid] [-c name] login\n");
                exit();
                break;
            }
        }
    }

    if (!GID)
    {
        GID = UID;
    }

    if (!FULLNAME)
    {
        FULLNAME = USERNAME;
    }

    if (!HOMEDIR)
    {
        HOMEDIR = USERNAME;
    }

    if (userExists(USERNAME)==1){
        printf("Korisnik sa korisnickim imenom: %s vec postoji\n",USERNAME);
        exit();
    }

    if(UIDExists(UID)==1){
        printf("Korisnik sa UID-om: %d vec postoji\n",UID);
        exit();
    }

    passwd = open("/etc/passwd", O_CREATE | O_RDWR);
    group = open("/etc/group", O_CREATE | O_RDWR);

    read(passwd, buf, 512);
    read(group, buf, 512);
    char *buffer;
    write(passwd, USERNAME, strlen(USERNAME));
    write(passwd, ":", 1);
    write(passwd, PASSWORD, strlen(PASSWORD));
    write(passwd, ":", 1);
    itoa(UID,buffer,10);
    write(passwd, buffer, strlen(buffer));
    write(passwd, ":", 1);
    itoa(GID,buffer,10);
    write(passwd, buffer, strlen(buffer));
    write(passwd, ":", 1);
    write(passwd, FULLNAME, strlen(FULLNAME));
    write(passwd, ":/home/", 7);
    write(passwd, HOMEDIR, strlen(HOMEDIR));
    write(passwd, "\n", 1);

    write(group, USERNAME, strlen(USERNAME));
    write(group, ":", 1);
    itoa(UID,buffer,10);
    write(group, buffer, strlen(buffer));
    write(group, ":", 1);
    write(group, USERNAME, strlen(USERNAME));
    write(group, "\n", 1);

    close(passwd);
    close(group);
    exit();
}