// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>


// char* ArrayToString(int Arr[], int size) {
//     // Calculate the length of the resulting string
//     int total_length = size * 3; // 2 digits + 1 '-' separator for each element, plus 1 for the null terminator
//     char* result = malloc(total_length * sizeof(char));
//     if (result == NULL) {
//         printf("Memory allocation failed\n");
//         exit(1);
//     }
//     // Initialize the result string
//     result[0] = '\0';

//     // Iterate over the Array and append each element to the result string
//     for (int i = 0; i < size; i++) {
//         // Convert the integer to a string
//         char num_str[3]; // Assuming each number has at most 2 digits
//         snprintf(num_str, sizeof(num_str), "%02d", Arr[i]); // Ensures that each number is represented with at least 2 digits

//         // Append the string representation of the number to the result string
//         strcat(result, num_str);

//         // Append a '-' separator unless it's the last element
//         if (i < size - 1) {
//             strcat(result, "-");
//         }
//     }

//     return result;
// }

// int main() {
//     int array[] = {01, 22, 03, 99, 17};

//     // Call the ArrayToString function
//     char* path = ArrayToString(array, sizeof(array)/sizeof(array[0]));

//     // Print the resulting string
//     printf("%s\n", path);

//     // Free the dynamically allocated memory
//     free(path);

//     return 0;
// }


// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// // #include <main.h>

// #define MAX_NODES 100
// #define MAX_Arr_LENGTH 256


   
// char* ArrayToString(int Arr[], int size, int personal) {
//     // Calculate the length of the resulting string
//     int total_length = size * 3 ; // 2 digits + 1 '-' separator for each element, plus 1 for the null terminator
//     char* result = malloc(total_length * sizeof(char));
//     if (result == NULL) {
//         printf("Memory allocation failed\n");
//         exit(1);
//     }
//     // Initialize the result string
//     snprintf(result, sizeof(result), "%d-", personal);

//     // Iterate over the Array and append each element to the result string
//     for (int i = 0; i < size; i++) {
//         // Convert the integer to a string
//         char num_str[3]; // Assuming each number has at most 2 digits
//         snprintf(num_str, sizeof(num_str), "%d", Arr[i]); // Ensures that each number is represented with at least 2 digits

//         // Append the string representation of the number to the result string
//         strcat(result, num_str);

//         // Append a '-' separator unless it's the last element
//         if (i < size - 1) {
//             strcat(result, "-");
//         }
//     }

   
//     return result;
// }

// int main() {
//     char Arr[MAX_Arr_LENGTH];
//     int pathSize = 0;
//     int personal = 24;
//     int pathArray[] = {1, 22, 3, 99, 17};
// ;
//     pathSize = sizeof(pathArray)/2;
//     // Call the ArrayToString function
//     char* path = ArrayToString(pathArray, sizeof(pathArray)/sizeof(pathArray[0]), personal);

//     // Print the resulting string
//     printf("%s\n", path);

//     // Free the dynamically allocated memory
//     free(path);

//     return 0;
// }

// #include <stdio.h>
// #include <string.h>

// int countHyphens(const char *str) {
//     int count = 0;
//     for (int i = 0; str[i] != '\0'; i++) {
//         if (str[i] == '-') {
//             count++;
//         }
//     }
//     return count;
// }

// int main() {
//     char str[] = "1--2-45-98-7-6-14-23-76"; // Example string
//     int hyphenCount = countHyphens(str);

//     printf("Number of hyphens: %d\n", hyphenCount);

//     return 0;
// }
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char minPath[] = "10-22-35-47-59"; // Example minPath string
    int expPath; // Integer to store the second number

    int count = 0;
    char secondNum[3] = {0}; // Initialize the secondNum array

    // Find and extract the second number
    for (int i = 0; minPath[i] != '\0'; i++) {
        if (minPath[i] == '-') {
            count++;
            if (count == 1) { // If it's the first "-", the next number is the second number
                // Extract the two digits following the "-"
                sprintf(secondNum, "%c%c", minPath[i + 1], minPath[i + 2]);
                // Convert the extracted substring to an integer
                expPath = atoi(secondNum);
                break; // Exit the loop after finding the second number
            }
        }
    }

    printf("expPath: %d\n", expPath); // Print the expPath integer

    return 0;
}