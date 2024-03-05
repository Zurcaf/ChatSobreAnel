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

void argsCheck(int argc, char *argv[], char *IP, int *TCP, char *regIP, char *regUDP)
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
        

        //Verificação do regUDP
        
    }
}

// while (1)
// {
//     sscanf(input, "%s %s %d %s %d", COR, IP, &TCP, regIP, &regUDP);
//     // Check if the entered IP address is valid
//     if (isValidIPAddress(IP))
//     {
//         printf("The entered IP address is not in a valid format.\n");
//         continue;
//     }
//     // Check if the entered regIP is in a valid format
//     if (strcmp("193.136.138.142", regIP) != 0)
//     {
//         if (!isValidIPAddress(regIP))
//         {
//             printf("The entered regIP is not in a valid format.\n");
//             continue;
//         }
//     }
//     break;
// }

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