/*
Sintaksa:
chgrp group files…

Ovaj program menja grupu koja je vlasnik za navedene datoteke. Datoteke kojima se menja vlasnik se navode kao poslednji argument na komandnoj liniji. Grupa može da se navede kao GID ili pomoću naziva grupe.

Ako navedena grupa ne postoji, program ispisuje grešku i završava se bez izmene navedenih datoteka.
*/
#include "kernel/fcntl.h"

int main(int argc, char *argv[])
{
    char buf[512];
    char *nGroup;
    char *sFile;

    int mode = -1;

    if (argc < 3)
    {
        printf("neispravan format, pravilan format: chmod mode file...\n");
        exit();
    }

    nGroup = argv[2];

    for (int i = 2; i < argc; i++)
    {
        sFile = argv[i];
        //changeGroup(sFile,nGroup);
    }

    exit();
}