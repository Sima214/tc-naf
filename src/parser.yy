%token_prefix TOKEN_

%stack_size 160

%include {
    #include <tokenizer.hpp>

    #include <Logger.hpp>
}

%token_type { TokenData }

%left OP_ADD OP_SUB.
%left OP_MUL OP_DIV.
%nonassoc OP_EQUAL OP_LESS OP_GREATER OP_LESS_EQUAL OP_GREATER_EQUAL OP_NOT_EQUAL.

%parse_accept {
    // Parse OK.
    ssce::logi("Syntax Analysis OK!");
}

%syntax_error {
    // Syntax Error.
    TokenData tk = TOKEN;
    ssce::loge("Syntax error at %s:", yyTokenName[tk.token_type]);
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

code_block ::= BLOCK_START statement_list BLOCK_END.

statement_list ::= statement_list statement.
statement_list ::= .

statement ::= code_block.
statement ::= statement_assign.
statement ::= statement_for.
statement ::= statement_while.
statement ::= statement_if.
statement ::= statement_declare.
statement ::= statement_func_call.
statement ::= statement_null.

statement_declare ::= TYPE_INTEGER id_list STATEMENT_END.
statement_declare ::= TYPE_FLOAT id_list STATEMENT_END.

// Right recursion?
id_list ::= ID COMMA_SEP id_list.
id_list ::= ID.

statement_null ::= STATEMENT_END.

statement_assign ::= expression_assign STATEMENT_END.

expression_assign ::= ID ASSIGNMENT expression.

// Right recursion?
expression ::= expression_assign.
expression ::= rval.

expression_assign_opt ::= expression_assign.
expression_assign_opt ::= .

expression_bool_opt ::= expression_bool.
expression_bool_opt ::= .

statement_for ::= FOR PRNTH_OPEN expression_assign_opt STATEMENT_END expression_bool_opt STATEMENT_END expression_assign_opt PRNTH_CLOSE statement.

statement_while ::= WHILE PRNTH_OPEN expression_bool PRNTH_CLOSE statement.

statement_if ::= IF PRNTH_OPEN expression_bool PRNTH_CLOSE statement_else.

statement_else ::= ELSE statement.
statement_else ::= .

statement_func_call ::= ID PRNTH_OPEN PRNTH_CLOSE.

expression_bool ::= expression comp_op expression.

rval ::= rval add_op term.
rval ::= term.

term ::= term mult_op factor.
term ::= factor.

factor ::= PRNTH_OPEN expression PRNTH_CLOSE.
factor ::= OP_SUB factor.
factor ::= ID.
factor ::= CONST_INTEGER.
factor ::= CONST_FLOAT.

comp_op ::= OP_EQUAL | OP_LESS | OP_GREATER | OP_LESS_EQUAL | OP_GREATER_EQUAL | OP_NOT_EQUAL.

add_op ::= OP_ADD | OP_SUB.

mult_op ::= OP_MUL | OP_DIV.