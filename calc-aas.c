#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <float.h>
#include <stdbool.h>

#define NaN 0.0
#define Inf 1.0/0.0

float tmp = 0;
int curr = 0;
int choice;
char op;

__attribute__((constructor))
void __constructor__(){
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
}

typedef enum {
    ADD = '+',
    SUB = '-',
    MUL = '*',
    DIV = '/'
} calc_op;

int to_idx(char op) {
    switch(op) {
        case ADD: return 0;
        case SUB: return 1;
        case MUL: return 2;
        case DIV: return 3;
    }
}

typedef void (*calc_func)(float*, float);

void f_add(float *a, float b) { *a = *a + b; }
void f_sub(float *a, float b) { *a = *a - b; }
void f_mul(float *a, float b) { *a = *a * b; }
void f_div(float *a, float b) { *a = *a / b; }

const calc_func calc_funcs[] = {
    f_add, f_sub, f_mul, f_div
};

bool is_valid_op(char op) {
    return \
        op == ADD ||
        op == SUB ||
        op == MUL ||
        op == DIV;
}

void calculator() {
    long max_arr_size = 0x10;
	float elems[max_arr_size];

    printf(
        "=== Welcome to Calc-aaS ===\n"
        "Supported operations: +, -, *, /\n"
    );
    while(true) {
        printf(
            "What do you want to do?\n"
            "1. Calc\n"
            "2. Read\n"
            "0. Byee\n"
            ">> "
        );

        if(scanf("%d%*c", &choice) != 1) {
            printf("Invalid choice\n");
            exit(-1);
        }

        if(choice == 0) {
            printf("Goodbye!\n");
            exit(0);
        }

        if(choice == 1) {
            if(curr > max_arr_size) {
                printf("Array is full\n");
                continue;
            }
            printf("Enter your expression: ");
            scanf("%c%*c", &op);
            if(!is_valid_op(op)) {
                printf("Invalid operation\n");
                exit(-1);
            }
            printf("Enter first operand: ");
            scanf("%f%*c", &elems[curr]);
            printf("Enter second operand: ");
            scanf("%f%*c", &tmp);
            if(op == DIV && tmp == 0) {
                printf("Division by zero\n");
                return;
            }
            printf("=> %.3f ", elems[curr]);
            calc_funcs[to_idx(op)](&elems[curr], tmp);
            printf("%c %.3f = %a\n\n", op, tmp, elems[curr]);
            curr++;
        } else if(choice == 2) {
            printf("Enter index: ");
            scanf("%d%*c", &choice);
            if(choice > curr) {
                printf("Invalid index\n");
                exit(-1);
            }
            printf("Stored result: %a\n\n", elems[choice]);
        }
    }
}

int main(int argc, char* argv, char* envp[]) {
    calculator();
}