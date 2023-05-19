#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "complex.h"
#include <ctype.h>

#define MAX_COMMAND_LENGTH 100
# define ENTER_KEY 10 /* the ASCII value of the enter key */
#define MIN_COMMAND_LENGTH 4

char *error_messages[] = {
        "Undefined complex variable\n", /* "read_comp G, 3.1, 6.5", "read_comp a, 3.6, 5.1", "abs_comp 2.5" */
        "Undefined command name\n", /* "do_it A, B", "Add_Comp A, C" */
        "Invalid parameter, not a number\n", /* "read_comp A, 3.5, xyz", "mult_comp_img A, B" */
        "Missing parameter\n", /* "read_comp A, 3.5", "add_comp B", "abs_comp" */
        "Extraneous text after end of command\n", /* "read_comp A, 3.5, -3,", "print_comp C, D", "stop A" */
        "Multiple consecutive commas\n", /* "sub_comp F, , D" */
        "Missing comma\n", /* "read_comp A 3.5, -3", "print_comp C D" */
        "Illegal comma\n", /* "read_comp, A, 3.5" */
};

enum error_messages {
    UNDEFINED_COMPLEX_VAR,
    UNDEFINED_COMMAND_NAME,
    INVALID_PARAMETER_NOT_A_NUMBER,
    MISSING_PARAMETER,
    EXTRANEOUS_TEXT,
    MULTIPLE_CONSECUTIVE_COMMAS,
    MISSING_COMMA,
    ILLEGAL_COMMA
};

enum PARAMETER_TYPE {
    COMPLEX_VARS, /* for read_comp */
    COMPLEX_VAR, /* for all function except read_comp, stop */
    FLOAT, /* for mult_comp_real, mult_comp_img */
    FLOATS, /* read_comp */
    NONE /* for stop */
};

/* function prototypes */
void print_commands();

void empty_command_string(char *command_string);

int read_command_string(char *command_string);

void clear_command_buffer();

int get_function_index(char *command_name);

int get_complex_var_index(char complex_var);

int check_extraneous_text(char *command_string);

int next_char_is_comma(char *command_string, int start_index);

int check_multiple_consecutive_commas(char *command_string);

int check_last_string_char_is_not_letter_or_digit(char *command_string);

int check_null_or_empty_string(char *command_string);

/* Mapping between command name, command execution function and command parameters type */

struct {
    char *function_name;

    void (*function_exe)();

    enum PARAMETER_TYPE param_type;
} command[] = {
        {"read_comp",      read_comp,      FLOATS}, /* format read_comp A, 3.5, -3 */
        {"print_comp",     print_comp,     COMPLEX_VAR}, /* format print_comp A */
        {"add_comp",       add_comp,       COMPLEX_VARS}, /* format add_comp A, B */
        {"sub_comp",       sub_comp,       COMPLEX_VARS}, /* format sub_comp A, B */
        {"mult_comp_real", mult_comp_real, FLOAT}, /* format mult_comp_real A, 3.5 */
        {"mult_comp_img",  mult_comp_img,  FLOAT}, /* format mult_comp_img A, 3.5 */
        {"mult_comp_comp", mult_comp_comp, COMPLEX_VARS}, /* format mult_comp_comp A, B */
        {"abs_comp",       abs_comp,       COMPLEX_VAR}, /* format abs_comp A */
        {"stop",           stop,           NONE}, /* format stop */
        {"main_manu",      print_commands, NONE}, /* format main_manu */
};


complex A, B, C, D, E, F;

/* Mapping between complex name and complex variable */

struct {
    char complex_name;
    complex *complex_var;
} complex_vars_map[] = {
        {'A', &A},
        {'B', &B},
        {'C', &C},
        {'D', &D},
        {'E', &E},
        {'F', &F},
};

void print_commands() {
    printf("Please apply one of the following operations according to the following syntax:\n\n");
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
    char command_string[MAX_COMMAND_LENGTH];
    char command_string_copy[MAX_COMMAND_LENGTH];
    int function_index, complex_var_index = 0, complex_var2_index = 0;
    char *function_string, *complex_var_string, *additional_text, *real_string, *img_string, *complex_var2_string;
    double real_num = 0, img_num = 0;
    print_commands();

    while (true) {
        /* empty strings */
        function_string = "";
        complex_var_string = "";
        complex_var2_string = "";
        additional_text = "";
        real_string = "";
        img_string = "";

        empty_command_string(command_string);
        if (read_command_string(command_string) == 0) {
            continue;
        };

        /* copy command string */
        strcpy(command_string_copy, command_string);



        /* get function name */
        function_string = strtok(command_string, " ");

        /* get function index */
        function_index = get_function_index(function_string);
        if (function_index == -1) {
            printf("%s", error_messages[UNDEFINED_COMMAND_NAME]);
            continue;
        }

        /* check multiple consecutive commas */
        if (check_multiple_consecutive_commas(command_string_copy) == 1) {
            printf("%s", error_messages[MULTIPLE_CONSECUTIVE_COMMAS]);
            continue;
        }

        /* check illegal comma */

        if (next_char_is_comma(command_string_copy, strlen(function_string) - 1)) {
            printf("%s", error_messages[ILLEGAL_COMMA]);
            continue;
        }

        /* get extra based on command parameters type */

        if (command[function_index].param_type == COMPLEX_VAR) {
            complex_var_string = strtok(NULL, " ");
            if (complex_var_string == NULL) {
                printf("%s", error_messages[MISSING_PARAMETER]);
                continue;
            }
            complex_var_index = get_complex_var_index(complex_var_string[0]);
            if (complex_var_index == -1) {
                printf("%s", error_messages[UNDEFINED_COMPLEX_VAR]);
                continue;
            }
        } else if (command[function_index].param_type == COMPLEX_VARS ||
                   command[function_index].param_type == FLOATS ||
                   command[function_index].param_type == FLOAT
                ) {
            /* get first complex var */
            complex_var_string = strtok(NULL, " ");
            if (complex_var_string == NULL) {
                printf("%s", error_messages[MISSING_PARAMETER]);
                continue;
            }
            /* validate and remove comma */
            if (complex_var_string[strlen(complex_var_string) - 1] != ',') {

                if (check_null_or_empty_string(strtok(NULL, " "))) {
                    printf("%s", error_messages[MISSING_PARAMETER]);
                    continue;
                }
                printf("%s", error_messages[MISSING_COMMA]);
                continue;
            }

            complex_var_string[strlen(complex_var_string) - 1] = '\0';

            complex_var_index = get_complex_var_index(complex_var_string[0]);
            if (complex_var_index == -1) {
                printf("%s", error_messages[UNDEFINED_COMPLEX_VAR]);
                continue;
            }


            if (command[function_index].param_type == COMPLEX_VARS) {
                printf("complex_var_string: %s\n", complex_var_string);
                /* get second complex var */
                complex_var2_string = strtok(NULL, " ");
                if (complex_var2_string == NULL) {
                    printf("%s", error_messages[MISSING_PARAMETER]);
                    continue;
                }
                complex_var2_index = get_complex_var_index(complex_var_string[0]);
                if (complex_var2_index == -1) {
                    printf("%s", error_messages[UNDEFINED_COMPLEX_VAR]);
                    continue;
                }
            } else if (command[function_index].param_type == FLOATS) {
                /* get real number */
                real_string = strtok(NULL, " ");
                if (real_string == NULL) {
                    printf("%s", error_messages[MISSING_PARAMETER]);
                    continue;
                }


                /* validate and remove comma */
                if (real_string[strlen(real_string) - 1] != ',') {
                    /* check if there is a missing parameter */
                    if (check_null_or_empty_string(strtok(NULL, " "))) {
                        printf("%s", error_messages[MISSING_PARAMETER]);
                        continue;
                    } else {
                        printf("%s", error_messages[MISSING_COMMA]);
                        continue;
                    }
                }

                real_string[strlen(real_string) - 1] = '\0';

                real_num = atof(real_string);
                if (real_num == 0 && real_string[0] != '0' && sizeof real_string != 1) {
                    printf("%s", error_messages[INVALID_PARAMETER_NOT_A_NUMBER]);
                    continue;
                }

                /* get imaginary number */
                img_string = strtok(NULL, " ");
                if (img_string == NULL) {
                    printf("%s", error_messages[MISSING_PARAMETER]);
                    continue;
                }

                img_num = atof(img_string);
                if (img_num == 0 && img_string[0] != '0' && sizeof img_string != 1) {
                    printf("%s", error_messages[INVALID_PARAMETER_NOT_A_NUMBER]);
                    continue;
                }
            } else if (command[function_index].param_type == FLOAT) {
                /* get real number */
                real_string = strtok(NULL, " ");
                if (real_string == NULL) {
                    printf("%s", error_messages[MISSING_PARAMETER]);
                    continue;
                }

                real_num = atof(real_string);
                if (real_num == 0 && real_string[0] != '0' && sizeof real_string != 1) {
                    printf("%s", error_messages[INVALID_PARAMETER_NOT_A_NUMBER]);
                    continue;
                }
            }

            /* check last command char is ok */
            if (check_last_string_char_is_not_letter_or_digit(command_string_copy) == 1) {
                printf("%s", error_messages[EXTRANEOUS_TEXT]);
                continue;
            }
        }

        /* check if there is extra text (not including spaces) after command */
        additional_text = strtok(NULL, " ");
        if (check_extraneous_text(additional_text) == 1) {
            printf("%s", error_messages[EXTRANEOUS_TEXT]);
            continue;
        }

        /* execute command according to parameters type */
        if (command[function_index].param_type == COMPLEX_VAR) {
            command[function_index].function_exe(complex_vars_map[complex_var_index].complex_var);
        } else if (command[function_index].param_type == COMPLEX_VARS) {
            command[function_index].function_exe(complex_vars_map[complex_var_index].complex_var,
                                                 complex_vars_map[complex_var2_index].complex_var);
        } else if (command[function_index].param_type == FLOATS) {
            printf("floats\n");
            command[function_index].function_exe(complex_vars_map[complex_var_index].complex_var, real_num, img_num);
        } else if (command[function_index].param_type == FLOAT) {
            command[function_index].function_exe(complex_vars_map[complex_var_index].complex_var, real_num);
        } else if (command[function_index].param_type == NONE) {
            command[function_index].function_exe();
        } else {
            printf("%s", error_messages[UNDEFINED_COMMAND_NAME]);
        }

        continue;
    }


    return 0;
}

void empty_command_string(char *command_string) {
    int i = 0;
    while (command_string[i] != '\0') {
        command_string[i] = '\0';
        i++;
    }
}

int read_command_string(char *command_string) {
    int i, input_char = 0;
    for (i = 0; input_char != ENTER_KEY; i++) {
        input_char = getchar();
        if (input_char != ENTER_KEY) {
            command_string[i] = input_char;
        } else if (i > MAX_COMMAND_LENGTH) {
            printf("Command is too long. Please try again.\n");
            clear_command_buffer();
            return 0;
        } else {
            command_string[i] = input_char;
        }
    }

    if (i < MIN_COMMAND_LENGTH) {
        printf("Command is too short. Please try again.\n");
        clear_command_buffer();
        return 0;
    }

    return 1;
}

void clear_command_buffer() {
    while (getchar() != ENTER_KEY);
    return;
}

int get_function_index(char *command_name) {
    int i;
    for (i = 0; i < sizeof(command) / sizeof(command[0]); i++) {
        if (strncmp(command_name, command[i].function_name, strlen(command[i].function_name)) == 0) {
            return i;
        }
    }
    return -1;
}

int get_complex_var_index(char complex_var) {
    int i;
    for (i = 0; i < sizeof(complex_vars_map) / sizeof(complex_vars_map[0]); i++) {
        if (complex_var == complex_vars_map[i].complex_name) {
            return i;
        }
    }
    return -1;
}

/* check if there is extra text (not including spaces) after command */
int check_extraneous_text(char *command_string) {
    int i;
    if (command_string == NULL) {
        return 0;
    }
    /* check if extra test exists (ignore spaces, new lines and tabs) */
    for (i = 0; i < strlen(command_string); i++) {
        if (isspace(command_string[i]) || command_string[i] == '\n' || command_string[i] == '\t') {
            continue;
        } else {
            return 1;
        }
    }
    return 0;
}

int next_char_is_comma(char *command_string, int start_index) {
    int index = start_index;

    for (; index < strlen(command_string); index++) {
        if (isspace(command_string[index])) {
            continue;
        } else if (command_string[index] == ',') {
            return 1;
        } else {
            return 0;
        }
    }

    return 0;
}

int check_multiple_consecutive_commas(char *command_string) {
    int i;
    for (i = 0; i < strlen(command_string); i++) {
        if (command_string[i] == ',' && next_char_is_comma(command_string, i + 1) == 1) {
            return 1;
        }
    }
    return 0;
}

int check_last_string_char_is_not_letter_or_digit(char *command_string) {
    /* remove spaces from end of string */
    int i = strlen(command_string) - 1;
    while (isspace(command_string[i]) || command_string[i] == '\n' || command_string[i] == '\t') {
        i--;
    }

    if (isalpha(command_string[i]) || isdigit(command_string[i])) {
        return 0;
    }

    return 1;
}

int check_null_or_empty_string(char *command_string) {
    if (command_string == NULL || strlen(command_string) == 0 || isspace(command_string[0])) {
        return 1;
    }
    return 0;
}