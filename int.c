#include <stdio.h>
#include <string.h>

void eval (char *input) {
    char mem_cell[5000], c;
    memset(mem_cell, 0, sizeof(mem_cell));

    int in, in_ptr, loop;
    char *ptr = mem_cell;

    in_ptr = loop = 0;

    while (in_ptr < (int)strlen(input)) {
        in = input[in_ptr];

        if      (in == '>') { ptr++;            }
        else if (in == '<') { ptr--;            }
        else if (in == '+') { ++*ptr;           }
        else if (in == '-') { --*ptr;           }
        else if (in == '.') { putchar(*ptr);    }
        else if (in == ',') { *ptr = getchar(); }

        else if (in == '[') {
            if (*ptr == 0) {
                loop = 1;
                while (loop > 0) {
                    c = input[++in_ptr];
                    if ('[' == c || c == ']') loop -= c - '\\';
                }
            }
        }
        else if (in == ']') {
            loop = 1;
            while (loop > 0) {
                c = input[--in_ptr];
                if ('[' == c || c == ']') loop += c - '\\';
            }
            in_ptr--;
        }
        in_ptr++;
    }
}

int main (void) {
    char input[100000];
    int in;
    int ptr = 0;

    while ((in = getchar()) != EOF)
        input[ptr++] = (char)in;

    eval(input);
    return 0;
}
