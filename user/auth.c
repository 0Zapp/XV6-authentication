// init: The initial user-level program
#include "auth.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"
#include "kernel/fcntl.h"

char username[16];
char password[16];
int uid;
int UserGid;
char fullName[16];
char path[128];

char groupname[16];
int groupGid;
char groupMember[16];

void uidToUser(int Tuid)
{

    int passwd = open("/etc/passwd", O_RDWR);
    char buf[512];
    read(passwd, buf, 512);

    int count = 0;
    int i = 0;
    int j = 0;

    uid = 0;
    UserGid = 0;

    while (buf[i] != '\0')
    {
        if (buf[i] == ':')
        {
            count++;
            j = 0;
        }
        else if (buf[i] == '\n' && uid == Tuid)
        {

            break;
        }
        else if (buf[i] == '\n')
        {
            count = 0;
            j = 0;
            uid = 0;
            UserGid = 0;
        }
        else if (count == 0)
        {
            username[j] = buf[i];
            j++;
            username[j] = '\0';
        }
        else if (count == 1)
        {
            password[j] = buf[i];
            j++;
            password[j] = '\0';
        }
        else if (count == 2)
        {
            uid = uid * 10 + buf[i] - '0';
        }

        else if (count == 3)
        {
            UserGid = UserGid * 10 + buf[i] - '0';
        }
        else if (count == 4)
        {
            fullName[j] = buf[i];
            j++;
            fullName[j] = '\0';
        }
        else if (count == 5)
        {
            path[j] = buf[i];
            j++;
            path[j] = '\0';
        }

        i++;
    }
    close(passwd);
}

void gidToGroup(int Tgid)
{

    int passwd = open("/etc/passwd", O_RDWR);
    char buf[512];
    read(passwd, buf, 512);

    int count = 0;
    int i = 0;
    int j = 0;

    groupGid = 0;

    while (buf[i] != '\0')
    {
        if (buf[i] == ':')
        {
            count++;
            j = 0;
        }
        else if (buf[i] == '\n' && uid == Tgid)
        {

            break;
        }
        else if (buf[i] == '\n')
        {
            count = 0;
            j = 0;
            groupGid = 0;
        }
        else if (count == 0)
        {
            groupname[j] = buf[i];
            j++;
            groupname[j] = '\0';
        }
        else if (count == 1)
        {
            groupGid = groupGid * 10 + buf[i] - '0';
        }
        else if (count == 2)
        {
            //za clanove grupe
        }

        i++;
    }
    close(passwd);
}

char *toMode(int x) //transformise mode iz broja u string
{
    char *buf = "----------";

    if ((x & D) != 0)
    {
        buf[0] = 'd';
    }
    if ((x & RO) != 0)
    {
        buf[1] = 'r';
    }
    if ((x & WO) != 0)
    {
        buf[2] = 'w';
    }
    if ((x & XO) != 0)
    {
        buf[3] = 'x';
    }
    if ((x & RG) != 0)
    {
        buf[4] = 'r';
    }
    if ((x & WG) != 0)
    {
        buf[5] = 'w';
    }
    if ((x & XG) != 0)
    {
        buf[6] = 'x';
    }
    if ((x & RU) != 0)
    {
        buf[7] = 'r';
    }
    if ((x & WU) != 0)
    {
        buf[8] = 'w';
    }
    if ((x & XU) != 0)
    {
        buf[9] = 'x';
    }

    return buf;
}

char *toUsername(int targetUid) //transformise uid u string
{

    uidToUser(targetUid);
    return username;
}

char *toPath(int targetUid) //transformise uid u string
{

    uidToUser(targetUid);
    return path;
}

char *toGroup(int targetUid)
{

    gidToGroup(targetUid);
    return groupname;
}

int autentification(char USERNAME[], char PASSWORD[])
{

    int passwd = open("/etc/passwd", O_RDWR);
    char buf[512];
    read(passwd, buf, 512);

    int count = 0;
    int i = 0;
    int j = 0;

    uid = 0;
    UserGid = 0;

    while (buf[i] != '\0')
    {
        if (buf[i] == ':')
        {
            count++;
            j = 0;
        }
        else if (buf[i] == '\n' && (strcmp(USERNAME, username) == 0))
        {

            break;
        }
        else if (buf[i] == '\n')
        {
            count = 0;
            j = 0;
            uid = 0;
            UserGid = 0;
        }
        else if (count == 0)
        {
            username[j] = buf[i];
            j++;
            username[j] = '\0';
        }
        else if (count == 1)
        {
            password[j] = buf[i];
            j++;
            password[j] = '\0';
        }
        else if (count == 2)
        {
            uid = uid * 10 + buf[i] - '0';
        }

        else if (count == 3)
        {
            UserGid = UserGid * 10 + buf[i] - '0';
        }
        else if (count == 4)
        {
            fullName[j] = buf[i];
            j++;
            fullName[j] = '\0';
        }
        else if (count == 5)
        {
            path[j] = buf[i];
            j++;
            path[j] = '\0';
        }

        i++;
    }
    close(passwd);
    if (strcmp(password, PASSWORD) == 0)
    {
        return uid;
    }
    return -1;
}

void changePassword(char USERNAME[], char PASSWORD[])
{

    int passwd = open("/etc/passwd", O_RDWR);
    char buf[512];
    read(passwd, buf, 512);

    int count = 0;
    int b = 0;
    int a = 0;
    int i = 0;
    int j = 0;
    int length = 0;
    int flag = 0;

    while ((PASSWORD[i]) != '\0')
    {
        length++;
        i++;
    }

    i = 0;

    char before[512];
    char after[512];

    uid = 0;
    UserGid = 0;

    while (buf[i] != '\0')
    {
        if (buf[i] == ':')
        {
            if (flag == 1)
            {
                flag++;
            }
            if ((strcmp(USERNAME, username) == 0) && (flag == 0))
            {
                flag++;
                before[b] = buf[i];
                b++;
                before[b] = '\0';
            }

            count++;
            j = 0;
        }
        else if (buf[i] == '\n')
        {
            count = 0;
            j = 0;
            uid = 0;
            UserGid = 0;
        }
        else if (count == 0)
        {
            username[j] = buf[i];
            j++;
            username[j] = '\0';
        }
        else if (count == 1)
        {
            password[j] = buf[i];
            j++;
            password[j] = '\0';
        }
        else if (count == 2)
        {
            uid = uid * 10 + buf[i] - '0';
        }

        else if (count == 3)
        {
            UserGid = UserGid * 10 + buf[i] - '0';
        }
        else if (count == 4)
        {
            fullName[j] = buf[i];
            j++;
            fullName[j] = '\0';
        }
        else if (count == 5)
        {
            path[j] = buf[i];
            j++;
            path[j] = '\0';
        }
        if (flag == 0)
        {
            before[b] = buf[i];
            b++;
            before[b] = '\0';
        }
        else if (flag == 2)
        {
            after[a] = buf[i];
            a++;
            after[a] = '\0';
        }

        i++;
    }
    close(passwd);
    passwd = open("/etc/passwd", O_RDWR);
    write(passwd, before, b);
    write(passwd, PASSWORD, length);
    write(passwd, after, a);
}

getFreeUid()
{
    int value=1000;
    for(;;){
        if(UIDExists(value)==0){
            return value;
        }else{
            value++;
        }
    }
}

getFreeGid()
{
    int value=1000;
    for(;;){
        if(GIDExists(value)==0){
            return value;
        }else{
            value++;
        }
    }
}

int getMode()
{
    return 0;
}

int userExists(char USERNAME[])
{

    int passwd = open("/etc/passwd", O_RDWR);
    char buf[512];
    read(passwd, buf, 512);

    int count = 0;
    int i = 0;
    int j = 0;

    uid = 0;
    UserGid = 0;

    while (buf[i] != '\0')
    {
        if (buf[i] == ':')
        {
            count++;
            j = 0;
            if (strcmp(USERNAME, username) == 0)
            {

                return 1;
            }
        }
        else if (buf[i] == '\n')
        {
            count = 0;
            j = 0;
            uid = 0;
            UserGid = 0;
        }
        else if (count == 0)
        {
            username[j] = buf[i];
            j++;
            username[j] = '\0';
        }
        else if (count == 1)
        {
            password[j] = buf[i];
            j++;
            password[j] = '\0';
        }
        else if (count == 2)
        {
            uid = uid * 10 + buf[i] - '0';
        }

        else if (count == 3)
        {
            UserGid = UserGid * 10 + buf[i] - '0';
        }
        else if (count == 4)
        {
            fullName[j] = buf[i];
            j++;
            fullName[j] = '\0';
        }
        else if (count == 5)
        {
            path[j] = buf[i];
            j++;
            path[j] = '\0';
        }

        i++;
    }

    return 0;
}

int *getGids(char USERNAME[])
{

    int gids[64];
    int idx = 0;

    int group = open("/etc/group", O_RDWR);
    char buf[512];
    read(group, buf, 512);

    int count = 0;
    int i = 0;
    int j = 0;

    while (buf[i] != '\0')
    {
        if (buf[i] == ':')
        {
            count++;
            j = 0;
        }
        else if (buf[i] == ',')
        {
            if (strcmp(USERNAME, groupMember) == 0)
            {

                gids[idx] = groupGid;
                idx++;
                j = 0;
            }
        }
        else if (buf[i] == '\n')
        {
            count = 0;
            j = 0;
            groupGid = 0;
        }
        else if (count == 0)
        {
            groupname[j] = buf[i];
            j++;
            groupname[j] = '\0';
        }
        else if (count == 1)
        {
            groupGid = groupGid * 10 + buf[i] - '0';
        }
        else if (count == 2)
        {
            groupMember[j] = buf[i];
            j++;
            groupMember[j] = '\0';
        }

        i++;
    }

    return gids;
}

int UIDExists(int UID)
{
    int passwd = open("/etc/passwd", O_RDWR);
    char buf[512];
    read(passwd, buf, 512);

    int count = 0;
    int i = 0;
    int j = 0;

    uid = 0;
    UserGid = 0;

    while (buf[i] != '\0')
    {
        if (buf[i] == ':')
        {
            count++;
            j = 0;
            if (UID == uid)
            {

                return 1;
            }
        }
        else if (buf[i] == '\n')
        {
            count = 0;
            j = 0;
            uid = 0;
            UserGid = 0;
        }
        else if (count == 0)
        {
            username[j] = buf[i];
            j++;
            username[j] = '\0';
        }
        else if (count == 1)
        {
            password[j] = buf[i];
            j++;
            password[j] = '\0';
        }
        else if (count == 2)
        {
            uid = uid * 10 + buf[i] - '0';
        }

        else if (count == 3)
        {
            UserGid = UserGid * 10 + buf[i] - '0';
        }
        else if (count == 4)
        {
            fullName[j] = buf[i];
            j++;
            fullName[j] = '\0';
        }
        else if (count == 5)
        {
            path[j] = buf[i];
            j++;
            path[j] = '\0';
        }

        i++;
    }

    return 0;
}

int GroupExists(char GROUPNAME[])
{

    int group = open("/etc/group", O_RDWR);
    char buf[512];
    read(group, buf, 512);

    int count = 0;
    int i = 0;
    int j = 0;

    while (buf[i] != '\0')
    {
        if (buf[i] == ':')
        {

            if (strcmp(GROUPNAME, groupname) == 0)
            {

                return 1;
            }
            count++;
            j = 0;
        }
        else if (buf[i] == '\n')
        {
            count = 0;
            j = 0;
            groupGid = 0;
        }
        else if (count == 0)
        {
            groupname[j] = buf[i];
            j++;
            groupname[j] = '\0';
        }
        else if (count == 1)
        {
            groupGid = groupGid * 10 + buf[i] - '0';
        }
        else if (count == 2)
        {
            groupMember[j] = buf[i];
            j++;
            groupMember[j] = '\0';
        }

        i++;
    }

    return 0;
}

int GIDExists(int GID)
{

    int group = open("/etc/group", O_RDWR);
    char buf[512];
    read(group, buf, 512);

    int count = 0;
    int i = 0;
    int j = 0;

    while (buf[i] != '\0')
    {
        if (buf[i] == ':')
        {

            if (GID==groupGid)
            {

                return 1;
            }
            count++;
            j = 0;
        }
        else if (buf[i] == '\n')
        {
            count = 0;
            j = 0;
            groupGid = 0;
        }
        else if (count == 0)
        {
            groupname[j] = buf[i];
            j++;
            groupname[j] = '\0';
        }
        else if (count == 1)
        {
            groupGid = groupGid * 10 + buf[i] - '0';
        }
        else if (count == 2)
        {
            groupMember[j] = buf[i];
            j++;
            groupMember[j] = '\0';
        }

        i++;
    }

    return 0;
}

inline void swap(char *x, char *y) {
    char t = *x; *x = *y; *y = t;
}

// function to reverse buffer[i..j]
char* reverse(char *buffer, int i, int j)
{
    while (i < j)
        swap(&buffer[i++], &buffer[j--]);

    return buffer;
}

// Iterative function to implement itoa() function in C
char* itoa(int value, char* buffer, int base)
{
    // invalid input
    if (base < 2 || base > 32)
        return buffer;

    // consider absolute value of number
    int n = value;
    if (n < 0)
        n *= -1;

    int i = 0;
    while (n)
    {
        int r = n % base;

        if (r >= 10) 
            buffer[i++] = 65 + (r - 10);
        else
            buffer[i++] = 48 + r;

        n = n / base;
    }

    // if number is 0
    if (i == 0)
        buffer[i++] = '0';

    // If base is 10 and value is negative, the resulting string 
    // is preceded with a minus sign (-)
    // With any other base, value is always considered unsigned
    if (value < 0 && base == 10)
        buffer[i++] = '-';

    buffer[i] = '\0'; // null terminate string

    // reverse the string and return it
    return reverse(buffer, 0, i - 1);
}