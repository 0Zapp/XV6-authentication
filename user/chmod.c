/*
Sintaksa 1:
chmod mode file...

Uz pomoć sistemskog poziva chmod() postavlja mode fajla (ili fajlova) na prosleđeni mode.

Sintaksa 2:
chmod mode_fmt file...

Gde mode_fmt je sastavljen od tri dela. Prvi deo mode_fmt je za koga se menja mode (opcije su: u za user, g za group, o za others i a za all). Drugi deo mode_fmt je oznaka + ili - koja predstavlja da li dodajemo ili oduzimamo neku od permisija odabranom targetu) i treći deo je permisija koju dodajemo/oduzimamo (opcije su: r - read, w - write, x - execute i s - setuid bit).

Primer korišćenja:
chmod a+x file1 - dodaje svima pravo na execute za file1.
chmod o-w file1 file2 - oduzima pravo pisanja za other usere (useri koji nisu vlasnik fajla i deo grupe) za file1 i file2.

Napomena: za drugu sintaksu će biti potrebno koristiti i sistemski poziv stat() pored sistemskog poziva chmod().

Ako format nije validan, ili nije moguć pristup datotekama program prijavljuje grešku i završava se bez izmene datoteka.
*/
#include "kernel/fcntl.h"

int main(int argc, char *argv[])
{
    char buf[512];
    char *nMode;
    char *sFile;

    int mode = -1;

    if (argc < 3)
    {
        printf("neispravan format, pravilan format: chmod mode file...\n");
        exit();
    }

    nMode = argv[2];

    for (int i = 2; i < argc; i++)
    {
        sFile = argv[i];
        mode = getMode();
        chmod(sFile, mode);
    }

    exit();
}