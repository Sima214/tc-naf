%token_prefix TOKEN_

%stack_size 160

%include {
    #include <common.hpp>
    #include <expressions.hpp>
    #include <program.hpp>
    #include <statements.hpp>
    #include <tokenizer.hpp>

    #include <Logger.hpp>

    #include <vector>
    #include <string>
}

%token_type { TokenData }

%extra_argument { Program* prg }

%nonassoc PRNTH_CLOSE.
%nonassoc ELSE.

%parse_accept {
    // Parse OK.
    ssce::logi("Syntax Analysis OK!");
}

%syntax_error {
    // Syntax Error.
    TokenData tk = TOKEN;
    ssce::loge("Invalid token `%s` at %u:%u", yyTokenName[tk.token_type], tk.meta_data.line, tk.meta_data.column);
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

// Non terminals types(AST).
%type code_block {StatementList*}
%type statement_list {StatementList*}
%type statement {BaseStatement*}
%type statement_func_call {StatementFunction*}
%type statement_if {StatementIfElse*}
%type statement_for {StatementForLoop*}
%type statement_while {StatementWhileLoop*}
%type statement_declare {StatementDeclare*}

%type expression {BaseExpression*}
%type rval {BaseExpression*}
%type term {BaseExpression*}
%type factor {BaseExpression*}
%type expression_bool {ExpressionLogical*}
%type expression_bool_opt {ExpressionLogical*}
%type expression_assign_opt {ExpressionAssignment*}
%type expression_assign {ExpressionAssignment*}
%type statement_assign {ExpressionAssignment*}

%type id_list {std::vector<std::string>*}

// Lower case are non-terminal.
program ::= CLASS ID(name) BLOCK_START FUNCTION ID(func) PRNTH_OPEN PRNTH_CLOSE code_block(blk) BLOCK_END. {
    prg->setName(name.extra_data.const_str);
    prg->addFunc(func.extra_data.const_str, blk);
    delete func.extra_data.const_str;
}

code_block(A) ::= BLOCK_START statement_list(B) BLOCK_END. {
    A = B;
}

statement_list(A) ::= statement_list(B) statement(E). {
    A = B;
    if(E != NULL) {
        A->appendStatement(E);
    }
}
statement_list(A) ::= . {
    A = new StatementList;
}

statement(A) ::= code_block(B). {
    A = B;
}
statement(A) ::= statement_assign(B). {
    A = B;
}
statement(A) ::= statement_for(B). {
    A = B;
}
statement(A) ::= statement_while(B). {
    A = B;
}
statement(A) ::= statement_if(B). {
    A = B;
}
statement(A) ::= statement_declare(B). {
    A = B;
}
statement(A) ::= statement_func_call(B). {
    A = B;
}
statement(A) ::= statement_null. {
    A = NULL;
}

// Right recursion?
id_list(A) ::= ID(name) COMMA_SEP id_list(B). {
    A = B;
    A->push_back(name.extra_data.const_str);
    delete name.extra_data.const_str;
}
id_list(A) ::= ID(name). {
    A = new std::vector<std::string>;
    A->push_back(name.extra_data.const_str);
    delete name.extra_data.const_str;
}

statement_declare(A) ::= TYPE_INTEGER id_list(B) STATEMENT_END. {
    A = new StatementDeclare(RETTYPE_INT, B);
}
statement_declare(A) ::= TYPE_FLOAT id_list(B) STATEMENT_END. {
    A = new StatementDeclare(RETTYPE_FLOAT, B);
}

statement_null ::= STATEMENT_END. {
    /* Ignored */
}

statement_for(A) ::= FOR PRNTH_OPEN expression_assign_opt(I) STATEMENT_END expression_bool_opt(C) STATEMENT_END expression_assign_opt(L) PRNTH_CLOSE statement(B). {
    A = new StatementForLoop(I, C, L, B);
}

statement_while(A) ::= WHILE PRNTH_OPEN expression_bool(C) PRNTH_CLOSE statement(B). {
    A = new StatementWhileLoop(C, B);
}

statement_if(A) ::= IF PRNTH_OPEN expression_bool(L) PRNTH_CLOSE statement(I). {
    A = new StatementIfElse(L, I, NULL);
}
statement_if(A) ::= IF PRNTH_OPEN expression_bool(L) PRNTH_CLOSE statement(I) ELSE statement(E). {
    A = new StatementIfElse(L, I, E);
}

statement_func_call(A) ::= ID(name) PRNTH_OPEN expression(expr) PRNTH_CLOSE STATEMENT_END. {
    A = new StatementFunction(name.extra_data.const_str, expr);
    delete name.extra_data.const_str;
}

statement_assign(A) ::= expression_assign(B) STATEMENT_END. {
    A = B;
}

expression_assign(A) ::= ID(name) ASSIGNMENT expression(E). {
    A = new ExpressionAssignment(name.extra_data.const_str, E);
    delete name.extra_data.const_str;
}

// Right recursion?
expression(A) ::= expression_assign(B). {
    A = B;
}
expression(A) ::= rval(B). {
    A = B;
}

expression_assign_opt(A) ::= expression_assign(B). {
    A = B;
}
expression_assign_opt(A) ::= . {
    A = NULL;
}

expression_bool_opt(A) ::= expression_bool(B). {
    A = B;
}
expression_bool_opt(A) ::= . {
    A = NULL;
}

expression_bool(A) ::= expression(B) OP_EQUAL expression(C). {
    A = new ExpressionLogical(BOOP_EQ, B, C);
}
expression_bool(A) ::= expression(B) OP_LESS expression(C). {
    A = new ExpressionLogical(BOOP_L, B, C);
}
expression_bool(A) ::= expression(B) OP_GREATER expression(C). {
    A = new ExpressionLogical(BOOP_G, B, C);
}
expression_bool(A) ::= expression(B) OP_LESS_EQUAL expression(C). {
    A = new ExpressionLogical(BOOP_LE, B, C);
}
expression_bool(A) ::= expression(B) OP_GREATER_EQUAL expression(C). {
    A = new ExpressionLogical(BOOP_GE, B, C);
}
expression_bool(A) ::= expression(B) OP_NOT_EQUAL expression(C). {
    A = new ExpressionLogical(BOOP_NEQ, B, C);
}

rval(A) ::= rval(B) OP_ADD term(C). {
    A = new ExpressionAdd(B, C);
}
rval(A) ::= rval(B) OP_SUB term(C). {
    A = new ExpressionSub(B, C);
}
rval(A) ::= term(B). {
    A = B;
}

term(A) ::= term(B) OP_MUL factor(C). {
    A = new ExpressionMul(B, C);
}
term(A) ::= term(B) OP_DIV factor(C). {
    A = new ExpressionDiv(B, C);
}
term(A) ::= factor(B). {
    A = B;
}

factor(A) ::= PRNTH_OPEN expression(B) PRNTH_CLOSE. {
    A = B;
}
factor(A) ::= OP_SUB factor(B). {
    A = new ExpressionNegate(B);
}
factor(A) ::= ID(name). {
    A = new ExpressionVariable(name.extra_data.const_str);
    delete name.extra_data.const_str;
}
factor(A) ::= CONST_INTEGER(v). {
    A = new ExpressionConstInteger(v.extra_data.const_int);
}
factor(A) ::= CONST_FLOAT(v). {
    A = new ExpressionConstFloat(v.extra_data.const_flt);
}
