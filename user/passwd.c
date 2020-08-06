/*
Sintaksa:
passwd [USER]

Korisnički program passwd služi za izmenu korisničke šifre. Obični korisnički nalog može da menja samo svoju šifru, dok root nalog može da menja šifru za bilo kog korisnika. Naziv korisnika za kojeg se menja šifra se navodi kao opcioni argument na komandnoj liniji. Ako se taj argument izostavi, podrazumeva se da se menja šifra za trenutnog korisnika. Kada obični korisnički nalog menja svoju šifru, prvo mora da unese svoju trenutnu šifru. Root korisnik može da preskoči ovaj korak.

Pri unošenju nove šifre, ona mora da se unese dva puta identično. Šifra mora da bude duža od 6 karaktera. Ako bilo koji od ova dva uslova nije ispunjen, šifra se ne menja i program se završava uz ispis odgovarajuće poruke o grešci.

Pri unošenju svake šifre (stare ili nove, oba puta) ne prikazivati unešen tekst na ekranu.

passwd je jedini program koji mora imati postavljen setuid bit (da bi mogao modifikovati /etc/passwd kada je pokrenut od strane običnog korisnika).
*/
#include "kernel/fcntl.h"

int main(int argc, char *argv[])
{

    int currentUid = getuid();
    char *currentUser = toUsername(currentUid);
    char *user = toUsername(currentUid);
    int flag=1;
    
    if (argc > 2)
    {
        printf("Pogresan broj argumenata\n");
        exit();
    }
    else if (argc == 2)
    {
        user = argv[1];
        if(userExists(user)==0){
            printf("korisnik %s ne postoji!\n",user);
            exit();
        }
    }

    if ((strcmp(user, currentUser) == 0) || (currentUid == 0))
    {
        if (currentUid != 0)
        {
            flag = 0;
        }
        while (flag == 0)
            {
                char PASSWORD[512];
                int fd;

                printf("unesite trenutni Password za %s:\n",user);
                hide();
                fd = open("/dev/console", O_RDWR);
                read(fd, PASSWORD, 512);
                close(fd);
                PASSWORD[strlen(PASSWORD) - 1] = '\0';
                show();

                if(autentification(user,PASSWORD)!=-1){
                    flag++;
                }
            }
        flag=0;

        while(flag==0){
                char PASSWORD1[512];
                char PASSWORD2[512];
                int fd;

                printf("unesite novi Password za %s:\n",user);
                hide();
                fd = open("/dev/console", O_RDWR);
                read(fd, PASSWORD1, 512);
                close(fd);
                PASSWORD1[strlen(PASSWORD1) - 1] = '\0';
                show();
                printf("unesite novi Password za %s jos jednaput:\n",user);
                hide();
                fd = open("/dev/console", O_RDWR);
                read(fd, PASSWORD2, 512);
                close(fd);
                PASSWORD2[strlen(PASSWORD2) - 1] = '\0';
                show();
                if((strcmp(PASSWORD1,PASSWORD2)==0)&&(strlen(PASSWORD1)>6)){
                    printf("password promenje\n");
                    changePassword(user,PASSWORD1);
                    flag++;

                }else{
                    printf("sifre se ne poklapaju ili nemaju vise od 6 karaktera, pokusajte ponovo\n");
                }
        }


    }
    exit();
}