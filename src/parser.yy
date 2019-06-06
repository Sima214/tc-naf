%token_prefix TOKEN_

%stack_size 160

%include {
    #include <tokenizer.hpp>

    #include <Logger.hpp>
}

%token_type { TokenData }

// %left ADD SUB.
// %left MUL DIV.

%parse_accept {
    // Parse OK.
    ssce::logi("Syntax Analysis OK!");
}

%syntax_error {
    // Syntax Error.
    TokenData tk = TOKEN;
    ssce::loge("Syntax error at todo:");
}

%parse_failure {
    // Syntax Failure.
    ssce::logf("Syntax failure!");
}

%stack_overflow {
    // Parser's stack overflow.
    ssce::logf("Parser stack overflow!");
}

%start_symbol program

// Lower case are non-terminal.
program ::= CLASS ID BLOCK_START FUNCTION ID PRNTH_OPEN PRNTH_CLOSE code_block BLOCK_END.

code_block ::= BLOCK_START statement_list BLOCK_START.

statement_list ::= statement_list statement.
statement_list ::= .

statement ::= code_block | statement_assign | statement_for | statement_while | statement_if | statement_declare | statement_func_call | statement_null.

statement_declare ::= type id_list STATEMENT_END.

type ::= TYPE_INTEGER | TYPE_NUMERICAL.

// Right recursion?
id_list ::= ID COMMA_SEP id_list | ID.

statement_null ::= STATEMENT_END
/*
NULL-STMT ::= ;
ASSIGN-STMT ::= ASSIGN-EXPR;
ASSIGN-EXPR ::= id = EXPR
EXPR ::= ASSIGN-EXPR
| RVAL
FOR-STMT ::= for ( OPASSIGN-EXPR; OPBOOL-EXPR; OPASSIGN-EXPR ) STMT
OPASSIGN-EXPR ::= ASSIGN-EXPR
| ε
OPBOOL-EXPR ::= BOOL-EXPR
| ε
WHILE-STMT ::= while ( BOOL-EXPR ) STMT
IF-STMT ::= if ( BOOL-EXPR ) STMT ELSE-PART
ELSE-PART ::= else STMT
| ε
BOOL-EXPR ::= EXPR C-OP EXPR
C-OP ::= == | < | > | <= | >= | !=
RVAL ::= RVAL + TERM
| RVAL - TERM
| TERM
TERM ::= TERM * FACTOR
| TERM / FACTOR
| FACTOR
FACTOR ::= ( EXPR )
| - FACTOR
| id
| num*/