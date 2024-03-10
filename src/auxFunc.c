#include "../hed/main.h"

void memoryCheck(void *ptr)
{
    if (ptr == NULL)
    {
        fprintf(stderr, "ERROR: not enough memory available!\n");
        exit(4);
    }
}

void fileCheck(FILE *filePointer, char *fileName)
{
    if (filePointer == NULL)
    {
        fprintf(stderr, "ERROR: could not open file %s!\n", fileName);
        exit(3);
    }
}

bool ValidIPAddress(const char *ipAddress)
{
    int num, dots = 0;
    int last = -1;

    // Check if the IP length is reasonable
    if (strlen(ipAddress) > MAX_IP_LENGTH)
    {
        return true;
    }

    for (int i = 0; ipAddress[i] != '\0'; i++)
    {
        if (!isdigit(ipAddress[i]) && ipAddress[i] != '.')
        {
            return true;
        }

        if (ipAddress[i] == '.')
        {
            dots++;

            if (i == 0 || ipAddress[i + 1] == '\0')
            {
                return true;
            }

            num = atoi(ipAddress + last + 1);
            if (num < 0 || num > 255)
            {
                return true;
            }

            last = i;
        }

        if (dots > 3 || (i > 0 && ipAddress[i] == '.' && ipAddress[i - 1] == '.'))
        {
            return true;
        }

        if (i - last > 4)
        {
            return true;
        }

        if (isdigit(ipAddress[i]) && ipAddress[i + 1] == '\0')
        {
            num = atoi(ipAddress + last + 1);
            if (num < 0 || num > 255)
            {
                return true;
            }
        }
    }

    if (dots != 3)
    {
        return true;
    }

    return false;
}

void argsCheck(int argc, char *argv[], char *IP, int *TCP, char *regIP, int *regUDP)
{
    //Verificação do número de argumentos (2 ou 4) porque 
    //  regIP e regUDP são os parametros do servidor de registo e
    //  por omissão, estes parâmetros tomam os valores 193.136.138.142 e 59000, não sendo necessária a sua introdução.

    if (argc != 3 && argc != 5)
    {
        fprintf(stderr, "ERROR: wrong number of arguments!\n");
        exit(1);
    }
    
    //Verificação do IP
    if (ValidIPAddress(argv[1]))
    {
        fprintf(stderr, "ERROR: the entered IP address is not in a valid format!\n");
        exit(2);
    }

    strcpy(IP, argv[1]);
    
    //Verificação do TCP
    *TCP = atoi(argv[2]);
    if (*TCP < 1024 || *TCP > 65535)
    {
        fprintf(stderr, "ERROR: the entered TCP port is not in a valid format!\n");
        exit(2);
    }
    
    if (argc == 5)
    {
        //Verificação do regIP
        if (ValidIPAddress(argv[3]))
        {
            fprintf(stderr, "ERROR: the entered regIP address is not in a valid format!\n");
            exit(2);
        }

        //Verificação do regUDP
        *regUDP = atoi(argv[4]);
        if (*regUDP < 1024 || *regUDP > 65535)
        {
            fprintf(stderr, "ERROR: the entered regUDP port is not in a valid format!\n");
            exit(2);
        }        
    }
}

