#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "complex.h"
#include <ctype.h>


void print_commands();

complex A, B, C, D, E, F;

char *commands[] = {"read_comp", "print_comp", "add_comp", "sub_comp", "mult_comp_real", "mult_comp_img",
                    "mult_comp_comp", "abs_comp", "stop", "main_manu"};


complex *get_complex(char complex_name) {
    switch (complex_name) {
        case 'A':
            return &A;
        case 'B':
            return &B;
        case 'C':
            return &C;
        case 'D':
            return &D;
        case 'E':
            return &E;
        case 'F':
            return &F;
        default:
            return NULL;
    }
}

int validate_complex_name(char complex_name) {
    if (complex_name < 'A' || complex_name > 'F') {
        return 0;
    }
    return 1;
}


void print_commands() {
    printf("Please apply one of the following operations according to the following syntax:\n");
    printf("1. read_comp <complex number>, <real>, <imaginary>\n");
    printf("2. print_comp <complex number>\n");
    printf("3. add_comp <complex number>, <complex number>\n");
    printf("4. sub_comp <complex number>, <complex number>\n");
    printf("5. mult_comp_real <complex number>, <real number>\n");
    printf("6. mult_comp_img <complex number>, <imaginary number>\n");
    printf("7. mult_comp_comp <complex number>, <complex number>\n");
    printf("8. abs_comp <complex number>\n");
    printf("9. stop\n");
    printf("10. main_manu\n\n");
}


int main() {
    char *token;
    char *command;
    int command_index;
    char complex_name, complex_name2;
    char real_str, imag_str, num_str;
    double real, imag, num;
    int i;
    char comma, comma2;
    int continue_flag = 0;
    size_t command_len;
    complex *comp, *comp2;

    print_commands();
    while (true) {
        if (continue_flag == 1) {
            continue_flag = 0;
            free(command);
        }
        command = malloc(MAX_COMMAND_LENGTH * sizeof(char));
        printf("Please enter your command:\n");
        /* scan the full line */
        fgets(command, 20, stdin);
        /* remove trailing whitespace */
        command_len = strlen(command);
        while (command_len > 0 && isspace(command[command_len-1])) {
            command_len--;
        }
        command[command_len] = '\0'; /* terminate the string */
        /* remove newline character from command */
        command[strcspn(command, "\n")] = 0;
        /* split the command to tokens */
        token = strtok(command, " ");
        /* validate the first token is valid command */
        if (token == NULL) {
            printf("Invalid command!\n");
            continue_flag = 1;
        }
        /* get the command index and print if not valid */
        command_index = -1;
        for (i = 0; i < 10; i++) {
            if (strcmp(token, commands[i]) == 0) {
                command_index = i;
                break;
            }
        }

        if (command_index == -1) {
            printf("Invalid command!\n");
            continue_flag = 1;
            continue;
        }

        /* if the command is main menu, print the commands and continue */
        if (command_index == 9) {
            print_commands();
            continue_flag = 1;
            continue;
        }

        /* validate stop command in the following format: stop */
        if (command_index == 8) {
            /* validate the third token is '\n' or EOF */
            token = strtok(NULL, " ");
            if (token != NULL && strcmp(token, "\n") != 0) {
                printf("Invalid command!\n");
                continue_flag = 1;
                continue;
            }
            stop();
        }

        /* validate the command */

        /* validate second token is complex name */
        token = strtok(NULL, " ");
        if (token == NULL) {
            printf("second token should be complex name!\n");
            continue_flag = 1;
            continue;
        }

        complex_name = token[0];

        /* get the complex number */
        comp = get_complex(complex_name);
        if (comp == NULL) {
            printf("Invalid complex number!\n");
            continue_flag = 1;
            continue;
        }

        /* validate read_comp: rest of the command should be: ", <real>, <imaginary>" */
        if (command_index == 0) {
            /* validate the third token is ',' */
            comma = token[1];

            if (comma != ',') {
                printf("Invalid command!\n");
                continue_flag = 1;
                continue;
            }

            /* get next char */
            token = strtok(NULL, " ");

            /* the char can be 1.2323123 or 1 for example, so we nead to assume it is real number */
            printf("token: %s\n", token);
            real = atof(&real_str);

            printf("real: %c\n", real);

        }

        /* validate print_comp command in the following format: print_comp <complex number> */
        if (command_index == 1) {
            /* validate the third token is '\n' or EOF */
            token = strtok(NULL, " ");
            if (token != NULL && strcmp(token, "\n") != 0) {
                printf("Invalid command!\n");
                continue_flag = 1;
                continue;
            }
            /* print the complex number */
            print_comp(comp);
            continue_flag = 1;
            continue;
        }

        /* validate add_comp, sub_comp, mult_comp_comp commands in the following format: <command> <complex number>, <complex number> */
        if (command_index == 2 || command_index == 3 || command_index == 6) {
            /* validate the third token is ',' */
            token = strtok(NULL, " ");
            if (token == NULL || strcmp(token, ",") != 0) {
                printf("Invalid command!\n");
                continue_flag = 1;
                continue;
            }
            /* validate the fourth token is complex number */
            token = strtok(NULL, " ");
            if (token == NULL) {
                printf("Invalid command!\n");
                continue_flag = 1;
                continue;
            }
            complex_name2 = token[0];

            /* get the second complex number */
            comp2 = get_complex(complex_name2);
            if (comp2 == NULL) {
                printf("Invalid complex number!\n");
                continue_flag = 1;
                continue;
            }
            /* validate the fifth token is '\n' or EOF */
            token = strtok(NULL, " ");
            if (token != NULL && strcmp(token, "\n") != 0) {
                printf("Invalid command!\n");
                continue_flag = 1;
                continue;
            }
            /* perform the command */
            if (command_index == 2) {
                add_comp(comp, comp2);
            } else if (command_index == 3) {
                sub_comp(comp, comp2);
            } else {
                mult_comp_comp(comp, comp2);
            }
            continue_flag = 1;
            continue;
        }

        /* validate mult_comp_real, mult_comp_img commands in the following format: <command> <complex number>, <number> */
        if (command_index == 4 || command_index == 5) {
            /* validate the third token is ',' */
            token = strtok(NULL, " ");
            if (token == NULL || strcmp(token, ",") != 0) {
                printf("Invalid command!\n");
                continue_flag = 1;
                continue;
            }
            /* validate the fourth token is number */
            token = strtok(NULL, " ");
            if (token == NULL) {
                printf("Invalid command!\n");
                continue_flag = 1;
                continue;
            }
            num = atof(token);
            /* validate the fifth token is '\n' or EOF */
            token = strtok(NULL, " ");
            if (token != NULL && strcmp(token, "\n") != 0) {
                printf("Invalid command!\n");
                continue_flag = 1;
                continue;
            }
            /* perform the command */
            if (command_index == 4) {
                mult_comp_real(comp, num);
            } else {
                mult_comp_img(comp, num);
            }
            continue_flag = 1;
            continue;
        }

        /* validate abs_comp command in the following format: abs_comp <complex number> */
        if (command_index == 7) {
            /* validate the third token is '\n' or EOF */
            token = strtok(NULL, " ");
            if (token != NULL && strcmp(token, "\n") != 0) {
                printf("Invalid command!\n");
                continue_flag = 1;
                continue;
            }
            /* perform the command */
            abs_comp(comp);
            continue_flag = 1;
            continue;
        }
    }
}

