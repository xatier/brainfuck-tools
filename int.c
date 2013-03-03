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
} command_t;

typedef struct _instruction {
    command_t opcode;
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
		cell->next = NULL;
        // recursively parse loops
		cell->loop = (op == LOOP_START) ? parse_iter() : NULL;
	}
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
            case INC:   ++*ptr;             break;
            case DEC:   --*ptr;             break;

            case NEXT:  ptr++;             break;
            case PREV:  ptr--;             break;

            case PUT:   putchar(*ptr);     break;
            case GET:   *ptr = getchar();  break;

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
    interpret(prog);
    free_i(prog);
    return 0;
}
