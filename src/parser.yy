%token_prefix TOKEN_

%left ADD SUB.
%left MUL DIV.

%token_type { int }

%include {
    #define YYPARSEFREENEVERNULL
}

%syntax_error {
    // Error print.
}

%parse_failure {
    // Failure print.
}

%start_symbol program

program ::= INT. {
    //
}