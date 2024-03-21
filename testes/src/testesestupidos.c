#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to transform an integer array into a string
char* ArrayToString(int Arr[], int size) {
    // Calculate the length of the resulting string
    int total_length = size * 3; // 2 digits + 1 '-' separator for each element, plus 1 for the null terminator
    char* result = malloc(total_length * sizeof(char));
    if (result == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    // Initialize the result string
    result[0] = '\0';

    // Iterate over the Array and append each element to the result string
    for (int i = 0; i < size; i++) {
        // Convert the integer to a string
        char num_str[3]; // Assuming each number has at most 2 digits
        snprintf(num_str, sizeof(num_str), "%02d", Arr[i]); // Ensures that each number is represented with at least 2 digits

        // Append the string representation of the number to the result string
        strcat(result, num_str);

        // Append a '-' separator unless it's the last element
        if (i < size - 1) {
            strcat(result, "-");
        }
    }

    return result;
}

int main() {
    int array[] = {01, 22, 03, 99, 17};

    // Call the ArrayToString function
    char* path = ArrayToString(array, sizeof(array)/sizeof(array[0]));

    // Print the resulting string
    printf("%s\n", path);

    // Free the dynamically allocated memory
    free(path);

    return 0;
}
