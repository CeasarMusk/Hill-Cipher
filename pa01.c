/*============================================================================
| Assignment: pa01 - Encrypting a plaintext file using the Hill cipher
|
| Author: Ceasar Muscatella
| Language: c
| To Compile: gcc -o pa01 pa01.c
| 
| To Execute: c -> ./pa01 kX.txt pX.txt
| where kX.txt is the keytext file
| and pX.txt is plaintext file
| Note:
| All input files are simple 8 bit ASCII input
| All execute commands above have been tested on Eustis
|
| Class: CIS3360 - Security in Computing - Summer 2024
| Instructor: McAlpin
| Due Date: 6/19/2024
+===========================================================================*/


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_TEXT_SIZE 10000
#define MAX_MATRIX_SIZE 9

// Function to read the key matrix from the file
void read_key_matrix(const char *filename, int *n, int matrix[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening key file");
        exit(EXIT_FAILURE);
    }

    fscanf(file, "%d", n);
    for (int i = 0; i < *n; i++) {
        for (int j = 0; j < *n; j++) {
            fscanf(file, "%d", &matrix[i][j]);
        }
    }

    fclose(file);
}

// Function to read the plaintext from the file and process it
void read_plaintext(const char *filename, char *text, int *length) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening plaintext file");
        exit(EXIT_FAILURE);
    }

    int index = 0;
    char c;
    while ((c = fgetc(file)) != EOF) {
        if (isalpha(c)) {
            text[index++] = tolower(c);
        }
    }
    text[index] = '\0';
    *length = index;

    fclose(file);
}

// Function to pad the plaintext to match the key matrix size
void pad_text(char *text, int *length, int block_size) {
    int pad_size = block_size - (*length % block_size);
    if (pad_size < block_size) {
        for (int i = 0; i < pad_size; i++) {
            text[*length + i] = 'x';
        }
        *length += pad_size;
        text[*length] = '\0';
    }
}

// Function to multiply matrix and vector
void matrix_vector_multiply(int n, int matrix[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE], int *vector, int *result) {
    for (int i = 0; i < n; i++) {
        result[i] = 0;
        for (int j = 0; j < n; j++) {
            result[i] += matrix[i][j] * vector[j];
        }
        result[i] %= 26;
    }
}

// Function to encrypt the plaintext using the Hill cipher
void hill_cipher_encrypt(int n, int matrix[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE], char *plaintext, char *ciphertext) {
    int vector[MAX_MATRIX_SIZE];
    int result[MAX_MATRIX_SIZE];
    int length = strlen(plaintext);

    for (int i = 0; i < length; i += n) {
        for (int j = 0; j < n; j++) {
            vector[j] = plaintext[i + j] - 'a';
        }

        matrix_vector_multiply(n, matrix, vector, result);

        for (int j = 0; j < n; j++) {
            ciphertext[i + j] = (result[j] + 'a');
        }
    }
    ciphertext[length] = '\0';
}

// Function to print text in blocks of 80 characters
void print_text(const char *label, const char *text) {
    printf("\n%s:\n", label);
    int length = strlen(text);
    for (int i = 0; i < length; i++) {
        putchar(text[i]);
        if ((i + 1) % 80 == 0) {
            putchar('\n');
        }
    }
    if (length % 80 != 0) {
        putchar('\n');
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <key file> <plaintext file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Variable declarations
    int n;
    int key_matrix[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE];
    char plaintext[MAX_TEXT_SIZE];
    char ciphertext[MAX_TEXT_SIZE];
    int plaintext_length;

    // Construct file paths
    char key_file_path[256];
    char plaintext_file_path[256];
    snprintf(key_file_path, sizeof(key_file_path), "pa01testing/%s", argv[1]);
    snprintf(plaintext_file_path, sizeof(plaintext_file_path), "pa01testing/%s", argv[2]);

    // Read the key matrix from the file
    read_key_matrix(key_file_path, &n, key_matrix);

    // Read and process the plaintext from the file
    read_plaintext(plaintext_file_path, plaintext, &plaintext_length);

    // Pad the plaintext to match the key matrix size
    pad_text(plaintext, &plaintext_length, n);

    // Encrypt the plaintext using the Hill cipher
    hill_cipher_encrypt(n, key_matrix, plaintext, ciphertext);

    // Print the results
    printf("\n Key matrix:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%4d", key_matrix[i][j]);
        }
        printf("\n");
    }

    print_text("Plaintext", plaintext);
    print_text("Ciphertext", ciphertext);

    return EXIT_SUCCESS;
}


/*=============================================================================
| I Ceasar Muscatella (ce447461) affirm that this program is
| entirely my own work and that I have neither developed my code together with
| any another person, nor copied any code from any other person, nor permitted
| my code to be copied or otherwise used by any other person, nor have I
| copied, modified, or otherwise used programs created by others. I acknowledge
| that any violation of the above terms will be treated as academic dishonesty.
+=============================================================================*/