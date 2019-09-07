#include <stdio.h>

#define byte unsigned char

typedef struct instruction
{
    byte opcode;
    byte operand;
} instruction;

instruction prog[256];

int main(int argc, char const *argv[])
{
    FILE *file = fopen(argv[1], "rb");
    int num_inst = 0, pc = 0, acc;
    byte memo[256], stats = 0;
    short buff;

    while (fread(&buff, 1, 1, file) != 0)
    {
        prog[num_inst].opcode = (byte)(buff);
        fread(&buff, 1, 1, file);
        prog[num_inst].operand = (byte)(buff);
        num_inst++;
    }

    while (prog[pc].opcode != 15)
    {
        stats = 0;

        switch (prog[pc].opcode)
        {
        // LOAD$
        case 0:
            acc = memo[prog[pc].operand];
            pc++;
            break;

        // LOAD
        case 1:
            acc = prog[pc].operand;
            pc++;
            break;

        // STORE
        case 2:
            memo[prog[pc].operand] = acc;
            pc++;
            break;

        // ADD
        case 3:
            acc += memo[prog[pc].operand];

            if (acc < -255 || acc > 255)
            {
                stats = 2;
            }

            pc++;
            break;

        // SUB
        case 4:
            acc -= memo[prog[pc].operand];

            if (acc < -255 || acc > 255)
            {
                stats = 2;
            }

            pc++;
            break;

        // MUL
        case 5:
            acc *= memo[prog[pc].operand];

            if (acc < -255 || acc > 255)
            {
                stats = 4;
            }

            pc++;
            break;

        // DIV
        case 6:
            acc /= memo[prog[pc].operand];
            pc++;
            break;

        // INC
        case 7:
            acc += 1;

            if (acc < -255 || acc > 255)
            {
                stats = 2;
            }
            pc++;
            break;

        // DEC
        case 8:
            acc -= 1;

            if (acc < -255 || acc > 255)
            {
                stats = 2;
            }

            pc += 1;
            break;

        // AND
        case 9:
            acc = acc & memo[prog[pc].operand];
            pc++;
            break;

        // OR
        case 10:
            acc = acc | memo[prog[pc].operand];
            pc++;
            break;

        // NOT
        case 11:
            acc = ~acc;
            break;

        // JMP
        case 12:
            pc = prog[pc].operand;
            break;

        // JZ
        case 13:
            if (acc == 0)
            {
                pc = prog[pc].operand;
            }
            else
            {
                pc++;
            }
            break;

        // JNZ
        case 14:
            if (acc != 0)
            {
                pc = prog[pc].operand;
            }
            else
            {
                pc++;
            }
            break;
        }

        acc = (byte)acc;

        if (acc == 0)
        {
            stats = stats | 1;
        }
    }
    printf("acc: %d stat: %d\n", acc, stats);
    
    return 0;
}
