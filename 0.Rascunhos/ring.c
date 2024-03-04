#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define MAX_IP_LENGTH 15 // IPv4 has at most 15 characters

int isValidIPAddress(const char *ipAddress)
{
    int num, dots = 0;
    int segments = 0;
    int last = -1;

    // Check if the IP length is reasonable
    if (strlen(ipAddress) > MAX_IP_LENGTH)
    {
        return 1;
    }

    for (int i = 0; ipAddress[i] != '\0'; i++)
    {
        if (!isdigit(ipAddress[i]) && ipAddress[i] != '.')
        {
            return 1;
        }

        if (ipAddress[i] == '.')
        {
            dots++;

            if (i == 0 || ipAddress[i + 1] == '\0')
            {
                return 1;
            }

            num = atoi(ipAddress + last + 1);
            if (num < 0 || num > 255)
            {
                return 1;
            }

            last = i;
        }

        if (dots > 3 || (i > 0 && ipAddress[i] == '.' && ipAddress[i - 1] == '.'))
        {
            return 1;
        }

        if (i - last > 4)
        {
            return 1;
        }

        if (isdigit(ipAddress[i]) && ipAddress[i + 1] == '\0')
        {
            num = atoi(ipAddress + last + 1);
            if (num < 0 || num > 255)
            {
                return 1;
            }
        }
    }

    if (dots != 3)
    {
        return 1;
    }

    return 0;
}

int main()
{
    char input[256]; // Maximum input length
    char COR[4];
    char IP[MAX_IP_LENGTH + 1]; // +1 for null character
    int TCP;
    char regIP[MAX_IP_LENGTH + 1] = "193.136.138.142";
    int regUDP = 59000;

    // Prompt user to enter all elements of the command
    printf("Enter the command: ");

    while (1)
    {

        fgets(input, sizeof(input), stdin);

        // Parse the input command
        sscanf(input, "%s %s %d %s %d", COR, IP, &TCP, regIP, &regUDP);

        // Check if the entered IP address is valid
        if (isValidIPAddress(IP))
        {
            printf("The entered IP address is not in a valid format.\n");
            continue;
        }

        // Check if the entered regIP is in a valid format
        if (strcmp("193.136.138.142", regIP) != 0)
        {
            if (!isValidIPAddress(regIP))
            {
                printf("The entered regIP is not in a valid format.\n");
                continue;
            }
        }

        break;
    }
    printf("\nApplication COR invoked with the following parameters:\n");
    printf("IP: %s\n", IP);
    printf("TCP: %d\n", TCP);
    printf("regIP: %s\n", regIP);
    printf("regUDP: %d\n", regUDP);

    printf("Available comands:\njoin (j) ring id\ndirect join (dj) id succid succIP succTCP\nchord (c)\nremove chord (rc)\nshow topology (st)\nshow routing (sr) dest\nshow path (sp) dest\nshow forwarding (sf)\nmessage (m) dest message\nleave (l)\nexit (x)\n");

    while (1)
    {
        scanf("%s", input);
    }

    return 0;
}
