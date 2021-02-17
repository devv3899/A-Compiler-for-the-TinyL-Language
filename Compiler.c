/* -------------------------------------------------

            CFG for tinyL LANGUAGE

     PROGRAM ::= STMTLIST !
     STMTLIST ::= STMT MORESTMTS
     MORESTMTS ::= ; STMTLIST | epsilon
     STMT ::= ASSIGN | READ | PRINT
     ASSIGN ::= VAR = EXPR
     READ ::= % VAR
     PRINT ::= $ VAR
     EXPR ::= ARITH_EXPR |
               LOGICAL_EXPR|
               VAR |
              DIGIT
     ARITH_EXPR ::= + EXPR EXPR |
                      - EXPR EXPR |
                      * EXPR EXPR
     LOGICAL_EXPR ::= | EXPR EXPR |
                       & EXPR EXPR
              
     VAR ::= a | b | c | d | e | f
     DIGIT ::= 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9

     NOTE: tokens are exactly a single character long

     Example expressions:

           +12!
           +1b!
           +*34&78!
           -*+1+2a58!

     Example programs;

         %a;%b;c=&3*ab;d=+c1;$d!
         %a;b=|*+1+2a58;$b!

 ---------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "Instr.h"
#include "InstrUtils.h"
#include "Utils.h"

#define MAX_BUFFER_SIZE 500
#define EMPTY_FIELD 0xFFFFF
#define token *buffer

/* GLOBALS */
static char *buffer = NULL;    /* read buffer */
static int regnum = 1;        /* for next free virtual register number */
static FILE *outfile = NULL;    /* output of code generation */

/* Utilities */
static void CodeGen(OpCode opcode, int field1, int field2, int field3);
static inline void next_token();
static inline int next_register();
static inline int is_digit(char c);
static inline int to_digit(char c);
static inline int is_identifier(char c);
static char *read_input(FILE * f);

/* Routines for recursive descending parser LL(1) */
static void program();
static void stmtlist();
static void morestmts();
static void stmt();
static void assign();
static void read();
static void print();
static int expr();
static int var();
static int digit();
static int arith_expr();
static int logical_expr();


/*************************************************************************/
/* Definitions for recursive descending parser LL(1)                     */
/*************************************************************************/
static int digit()
{
    int reg;

    if (!is_digit(token)) {
        ERROR("Expected digit\n");
        exit(EXIT_FAILURE);
    }
    reg = next_register();
    CodeGen(LOADI, reg, to_digit(token), EMPTY_FIELD);
    next_token();
    return reg;
}

static int var()
{
    /* DONE */
    int A;
    char B;
           if(!is_identifier(token)){
               
               ERROR("Expected identifier\n");
               exit(EXIT_FAILURE);
           }
    
           B = token;
           A = next_register();
           CodeGen(LOAD, A, B, EMPTY_FIELD);
           next_token();
           return A;
    
}

static int expr()
{
    /* DONE */
    switch (token) {
            
        case '+':
            return arith_expr();
        case '-':
            return arith_expr();
        case '*':
            return arith_expr();
            
            
            
        case '|':
            return logical_expr();
        case '&':
            return logical_expr();
            
            

        case '0':
            return digit();
        case '1':
            return digit();
        case '2':
            return digit();
        case '3':
            return digit();
        case '4':
            return digit();
        case '5':
            return digit();
        case '6':
            return digit();
        case '7':
            return digit();
        case '8':
            return digit();
        case '9':
            return digit();
            
            
    
        case 'a':
            return var();
        case 'b':
            return var();
        case 'c':
            return var();
        case 'd':
            return var();
        case 'e':
            return var();
        case 'f':
            return var();
            
            
    default:
        ERROR("Symbol %c not known in TinyL Language\n", token);
        exit(EXIT_FAILURE);
    }
}

static int arith_expr()
{
    int reg, left_reg, right_reg;
    switch (token) {
            
    case '+':
                next_token();
                left_reg = expr();
                right_reg = expr();
                reg = next_register();
                CodeGen(ADD, reg, left_reg, right_reg);
                return reg;
        /* DONE */
          //Using same block of code as given for case +.
    case '-':
                next_token();
                left_reg = expr();
                right_reg = expr();
                reg = next_register();
                CodeGen(SUB, reg, left_reg, right_reg);
                return reg;
            
    case '*':
                next_token();
                left_reg = expr();
                right_reg = expr();
                reg = next_register();
                CodeGen(MUL, reg, left_reg, right_reg);
                return reg;
            
            default:
                ERROR("Symbol %c not known in TinyL Language\n", token);
                exit(EXIT_FAILURE);
            }
    }

static int logical_expr()
{
    /* DONE */
    int reg, left_reg, right_reg;
    
    switch (token) {
      
    case '|':
                next_token();
                right_reg = expr();
                left_reg = expr();
                reg = next_register();
                CodeGen(OR, reg, left_reg, right_reg);
                return reg;
            
    case '&':
                next_token();
                left_reg = expr();
                right_reg = expr();
                reg = next_register();
                CodeGen(AND, reg, left_reg, right_reg);
                return reg;
            
            default:
            ERROR("Symbol %c not known in TinyL Language\n", token);
            exit(EXIT_FAILURE);
            
    }
    
}

static void assign()
{
    /* Done */
    int A;
    char B;
    
    if (!is_identifier(token)) {
        ERROR("ERROR for Tinyl Language\n");
        exit(EXIT_FAILURE);
    }
    
    B = token;
    next_token();
    
    if (token != '=') {
        ERROR("= sign is expected for Tinyl Language");
        exit(EXIT_FAILURE);
    };
    
    next_token();
    A = expr();
    
    CodeGen(STORE, B, A, EMPTY_FIELD);
}

static void read()
{
    /* DONE */
    char A;
    
    if(token != '%') {
        ERROR("% is expected for Tinyl Language\n");
        exit(EXIT_FAILURE);
    }
    
    next_token();
    if(!is_identifier(token)){
        ERROR("ERROR for Tinyl Language\n");
        exit(EXIT_FAILURE);
    }
    
    A = token;
    next_token();
    CodeGen(READ, A, EMPTY_FIELD, EMPTY_FIELD);
}

static void print()
{
    /* DONE */
    //char A = token;
    
    if (token != '$') {
        ERROR("$ or Printing of STMT is expected for Tinyl Language\n");
        exit(EXIT_FAILURE);
    }
    
    next_token();
    if (!is_identifier(token)) {
        ERROR("Expected identifier for Tinyl Language\n");
        exit(EXIT_FAILURE);
    }
    
    char A = token;
    CodeGen(WRITE, A, EMPTY_FIELD, EMPTY_FIELD);
    next_token();
}

static void stmt()
{
    
    /* DONE */
    
    if(!is_identifier(token)
       && token!='%'
       && token!='$') {
        ERROR("Possibly not a statement for Tinyl Language");
        exit(EXIT_FAILURE);
    }
    
    if(is_identifier(token)){
        assign();
    }
    
    if(token =='%'){
        read();
    }
    
    if(token=='$'){
        print();
    }
    
}

static void morestmts()
{
    /* DONE */
    if(token=='!') {
        return;
    }
    
    if(token!=';') {
        ERROR("ERROR. Symbol is %c for Tinyl Language\n", token);
        exit(EXIT_FAILURE);
    }
    
    next_token();
    stmtlist();
    
}

static void stmtlist()
{
    /* DONE */
    stmt();

    if(token == ';') {
        morestmts();
    }
    
}

static void program()
{
    /* DONE */
    stmtlist();
    
    if (token != '!') {
        ERROR("ERROR. Symbol is %c for Tinyl Language\n", token);
        exit(EXIT_FAILURE);
    };
}

/*************************************************************************/
/* Utility definitions                                                   */
/*************************************************************************/
static void CodeGen(OpCode opcode, int field1, int field2, int field3)
{
    Instruction instr;

    if (!outfile) {
        ERROR("File error\n");
        exit(EXIT_FAILURE);
    }
    instr.opcode = opcode;
    instr.field1 = field1;
    instr.field2 = field2;
    instr.field3 = field3;
    PrintInstruction(outfile, &instr);
}

static inline void next_token()
{
    if (*buffer == '\0') {
        ERROR("End of program input\n");
        exit(EXIT_FAILURE);
    }
    printf("%c ", *buffer);
    if (*buffer == ';')
        printf("\n");
    buffer++;
    if (*buffer == '\0') {
        ERROR("End of program input\n");
        exit(EXIT_FAILURE);
    }
    if (*buffer == '!')
        printf("!\n");
}

static inline int next_register()
{
    return regnum++;
}

static inline int is_digit(char c)
{
    if (c >= '0' && c <= '9')
        return 1;
    return 0;
}

static inline int to_digit(char c)
{
    if (is_digit(c))
        return c - '0';
    WARNING("Non-digit passed to %s, returning zero\n", __func__);
    return 0;
}

static inline int is_identifier(char c)
{
    if (c >= 'a' && c <= 'f')
        return 1;
    return 0;
}

static char *read_input(FILE * f)
{
    size_t size, i;
    char *b;
    int c;

    for (b = NULL, size = 0, i = 0;;) {
        if (i >= size) {
            size = (size == 0) ? MAX_BUFFER_SIZE : size * 2;
            b = (char *)realloc(b, size * sizeof(char));
            if (!b) {
                ERROR("Realloc failed\n");
                exit(EXIT_FAILURE);
            }
        }
        c = fgetc(f);
        if (EOF == c) {
            b[i] = '\0';
            break;
        }
        if (isspace(c))
            continue;
        b[i] = c;
        i++;
    }
    return b;
}

/*************************************************************************/
/* Main function                                                         */
/*************************************************************************/

int main(int argc, char *argv[])
{
    const char *outfilename = "tinyL.out";
    char *input;
    FILE *infile;

    printf("------------------------------------------------\n");
    printf("CS314 compiler for tinyL\n");
    printf("------------------------------------------------\n");
    if (argc != 2) {
        ERROR("Use of command:\n  compile <tinyL file>\n");
        exit(EXIT_FAILURE);
    }
    infile = fopen(argv[1], "r");
    if (!infile) {
        ERROR("Cannot open input file \"%s\"\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    outfile = fopen(outfilename, "w");
    if (!outfile) {
        ERROR("Cannot open output file \"%s\"\n", outfilename);
        exit(EXIT_FAILURE);
    }
    input = read_input(infile);
    buffer = input;
    program();
    printf("\nCode written to file \"%s\".\n\n", outfilename);
    free(input);
    fclose(infile);
    fclose(outfile);
    return EXIT_SUCCESS;
}

