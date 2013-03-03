#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// the brainfuck lexicon
typedef enum {
    INC        = '+',
    DEC        = '-',
    NEXT       = '>',
    PREV       = '<',
    GET        = ',',
    PUT        = '.',
    LOOP_START = '[',
    LOOP_END   = ']',
    /* - extensions ------ */
    ZERO       = '0',
} command_t;

typedef struct _instruction {
    command_t opcode;
    int value;    /* repetition value: +++ = 3 */
    struct _instruction *loop;
    struct _instruction *next;
} instruction;



static int istoken(char c)
{
    return (c == INC)  || (c == DEC)  ||
           (c == NEXT) || (c == PREV) ||
           (c == GET)  || (c == PUT)  ||
           (c == LOOP_START) || (c == LOOP_END);
}


static int lex_next(void)
{
	int c;
    // skip other tokens
	do {
		if ((c = fgetc(stdin)) == EOF)
			return 0;
	} while (!istoken(c));
	return c;
}


// prepare the linked-list of instruction cells
static instruction *parse_iter(void)
{
	command_t op;
	instruction *prog = NULL, *ptr = NULL, *cell;

	while ((op = lex_next()) != 0) {
		if (op == LOOP_END)
			return prog;

		cell = (instruction *) malloc(sizeof(instruction));

		if (prog == NULL)
			prog = cell;
		else
			ptr->next = cell;
		ptr = cell;

		cell->opcode = op;
		cell->value = 1;
		cell->next = NULL;
        // recursively parse loops
		cell->loop = (op == LOOP_START) ? parse_iter() : NULL;
	}
	return prog;
}

/* Optimizations */

// compact: compact repeat instructions
static instruction *compact (instruction *prog) {
    instruction *ret = prog;

    while (prog) {
        switch ((int) prog->opcode) {
            case LOOP_START:
                prog->loop = compact(prog->loop);
                break;
            case INC:
            case DEC:
            case NEXT:
            case PREV:
                while (prog->next &&
                        prog->opcode == prog->next->opcode) {
                    instruction *t = prog->next;

                    prog->next = t->next;
                    prog->value += t->value;
                    free(t);
                }
                break;
        }
        prog = prog->next;
    }
    return ret;
}

static instruction *deduce_zeros (instruction *prog)
{
    instruction *t = prog;
    if (prog->opcode == DEC && prog->next == NULL)
        prog->opcode = ZERO;

    while (prog) {
        if (prog->opcode == LOOP_START)
            prog->loop = deduce_zeros(prog->loop);
        prog = prog->next;
    }
    return t;
}

instruction *optimize (instruction *prog) {
    prog = compact(prog);
    prog = deduce_zeros(prog);
    return prog;
}

// our tape
#define HEAPSIZE 30000
static char heap[HEAPSIZE], *ptr = heap;


// interpret the list
static void interpret(instruction *prog)
{
	while (prog) {
		switch (prog->opcode) {
            case ZERO: *ptr = 0;                   break;
            case INC:  *ptr = *ptr + prog->value;  break;
            case DEC:  *ptr = *ptr - prog->value;  break;

            case NEXT:  ptr += prog->value;        break;
            case PREV:  ptr -= prog->value;        break;

            case PUT:   putchar(*ptr);             break;
            case GET:   *ptr = getchar();          break;

            case LOOP_START:
                while(*ptr) interpret(prog->loop); break;

            default:
                /* Ignore unknown symbol */
                break;
		}
		prog = prog->next;
	}
}

static void free_i(instruction *prog) {
    while (prog) {
        if (prog->opcode == LOOP_START)
            free_i(prog->loop);
        instruction *t = prog;
        prog = prog->next;
        free(t);
    }
}


int main (void) {
    instruction *prog = parse_iter();
    interpret(optimize(prog));
    //interpret(prog);
    free_i(prog);
    return 0;
}
