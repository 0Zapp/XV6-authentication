/*
Sintaksa:
usermod [-l login] [-u UID] [-c name] [-d dir] [-m] [-G grupa…] [-a] login

Program usermod se koristi za modifikaciju postojećih korisnika. Korisničko ime korisnika koji se modifikuje se navodi kao obavezan poslednji argument na komandnoj liniji. Ako ne postoji korisnik sa tim korisničkim imenom, program prijavljuje grešku i završava se bez izmene korisnika.

Ako se navede opcija koja nije podržana, program prijavljuje grešku i završava se bez kreiranja korisnika.

Opcija -l se koristi za promenu korisničkog imena za korisnika. Novo korisničko ime treba da bude jedinstveno. Ako nije, program prijavljuje grešku i završava se bez izmene korisničkog imena.

Opcija -u se koristi za promenu korisnikovog UID. Novi UID treba da bude ceo broj i da bude jedinstven. Ako to nije slučaj, program prijavljuje grešku i završava se bez izmene UID. Sve datoteke u korisnikovom home direktorijumu koje pripadaju ovom korisniku treba izmeniti tako da reflektuju ovu izmenu.

Opcija -c se koristi za promenu korisnikovog pravog imena.

Opcija -d se koristi za promenu korisnikovog home direktorijuma. Novi direktorijum se kreira ako već ne postoji. Ako se uz ovu opciju navede i opcija -m, čitav sadržaj starog direktorijuma treba premestiti u novi. Ako dođe do greške pri kreiranju direktorijuma, ili premeštanju podataka prijaviti grešku i završiti program bez toga. Opcija -m treba da ne radi ništa ako nema opcije -d.

Opcija -G se koristi za dodavanje korisnika u jednu ili više grupa. Grupe se navode razdvojene zarezom, i treba da sve već postoje. Ako neka grupa ne postoji, prijaviti grešku o tome, ali dodati korisnika u grupe koje postoje. Ako je korisnik već bio u nekim grupama osim svoje lične grupe, on se briše iz njih, osim ako se navede opcija -a uz ovu opciju. Ako je opcija -a navedena, onda korisnik ostaje u grupama u kojima je prethodno bio. Opcija -a treba da ne radi ništa ako nema opcije -G.

Ako program ne može da pristupi /etc/passwd ili /etc/group datoteci, prijavljuje grešku i završava se bez modifikovanja korisnika.
*/
#include "kernel/fcntl.h"

int main(int argc, char *argv[])
{
    //  int passwd;
    //   int group;

    char buf[512];
    char *cUsername;
    char *nUsername;
    char *nUID;
    char *nFullName;
    char *nHomeDir;

    int mode = 0;

    if (argc < 2 || argc > 14)
    {
        printf("neispravan format, pravilan format: useradd [-d dir] [-u uid] [-c name] login\n");
        exit();
    }

    cUsername = argv[argc - 1];

    for (int i = 1; i < argc - 1; i++)
    {
        if (mode = 0)
        {
            char *c = argv[i];

            if (strcmp(c, "-l") == 0)
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
            else if (strcmp(c, "-d") == 0)
            {
                mode = 4;
            }
            else if (strcmp(c, "-m") == 0)
            {
                mode = 5;
            }
            else if (strcmp(c, "-G") == 0)
            {
                mode = 6;
            }
            else if (strcmp(c, "-a") == 0)
            {
                mode = 7;
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
                nUsername = argv[i];
                mode = 0;
                break;

            case 2:
                nUID = argv[i];
                mode = 0;
                break;
            case 3:
                nFullName = argv[i];
                mode = 0;
                break;
            case 4:
                nHomeDir = argv[i];
                mode = 0;
                break;

            case 5:
                //m
                mode = 0;
                break;
            case 6:
                //G
                mode = 0;
                break;
            case 7:
                //a
                mode = 0;
                break;
            default:
                printf("neispravan format, pravilan format: useradd [-d dir] [-u uid] [-c name] login\n");
                exit();
                break;
            }
        }
    }

    printf("%s %s %s %s %s", cUsername, nUsername, nUID, nFullName, nHomeDir);
    /*
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

    passwd = open("/etc/passwd", O_CREATE | O_RDWR);
    group = open("/etc/group", O_CREATE | O_RDWR);

    read(passwd, buf, 512);
    read(group, buf, 512);

    write(passwd, USERNAME, strlen(USERNAME));
    write(passwd, ":", 1);
    write(passwd, PASSWORD, strlen(PASSWORD));
    write(passwd, ":", 1);
    write(passwd, UID, strlen(UID));
    write(passwd, ":", 1);
    write(passwd, GID, strlen(GID));
    write(passwd, ":", 1);
    write(passwd, FULLNAME, strlen(FULLNAME));
    write(passwd, ":/home/", 7);
    write(passwd, HOMEDIR, strlen(HOMEDIR));
    write(passwd, "\n", 1);

    write(group, USERNAME, strlen(USERNAME));
    write(group, ":", 1);
    write(group, UID, strlen(UID));
    write(group, ":", 1);
    write(group, USERNAME, strlen(USERNAME));
    write(group, "\n", 1);

    close(passwd);
    close(group);
    */
    exit();
}