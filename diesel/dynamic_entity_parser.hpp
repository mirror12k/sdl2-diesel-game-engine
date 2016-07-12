
#pragma once


#include <list>
using std::list;

#include <vector>
using std::vector;

#include <string>
using std::string;



#include "dynamic_values.hpp"


namespace diesel
{



enum dent_token_type
{
    DENT_TOKEN_SYMBOL,
    DENT_TOKEN_STRING,
    DENT_TOKEN_INTEGER,
    DENT_TOKEN_FLOAT,
    DENT_TOKEN_NAME,
};



struct dent_token
{
    dent_token_type type;
    string data;
    uint line_number;
    uint line_offset;
    uint text_offset;

    dent_token(dent_token_type type, const string& data, uint line_number, uint line_offset, uint text_offset);

    string to_string() const;
};



class dynamic_entity_lexer
{
protected:
    vector<dent_token> tokens;

    uint line_number, line_offset;

    uint tokens_iteration_index = 0;

public:
    dynamic_entity_lexer(const string& text);

    void parse(const string& text);
    uint skip_whitespace(const string& text, uint offset);
    uint extract_string(const string& text, uint offset);
    uint extract_number(const string& text, uint offset);
    uint extract_name(const string& text, uint offset);

    dent_token next_token();
    void skip_token();
    void assert_skip_token(dent_token_type type);
    void assert_skip_token(dent_token_type type, const string& value);

    bool is_next_token(dent_token_type type) const;
    bool is_next_token(dent_token_type type, const string& value) const;
    bool is_after_next_token(dent_token_type type) const;
    bool is_after_next_token(dent_token_type type, const string& value) const;
    bool is_n_token(dent_token_type type, int offset) const;
    bool is_n_token(dent_token_type type, const string& value, int offset) const;

    bool is_end() const;

};







enum dent_expression_type
{
    DENT_EXPRESSION_INSTANTIATION,
    DENT_EXPRESSION_FUNCTION_CALL,
    DENT_EXPRESSION_LOAD_VARIABLE,
    DENT_EXPRESSION_OBJECT,
    DENT_EXPRESSION_STATIC_VALUE,
};


class dent_expression
{
public:
    const dent_expression_type type;

    dent_expression(dent_expression_type type);
    virtual ~dent_expression();
};

class dent_expression_expression : public dent_expression
{
public:
    dent_expression* exp;
    dent_expression_expression(dent_expression_type type, dent_expression* exp);
    virtual ~dent_expression_expression();
};

class dent_two_sided_expression : public dent_expression
{
public:
    dent_expression* left, * right;
    dent_two_sided_expression(dent_expression_type type, dent_expression* left, dent_expression* right);
    virtual ~dent_two_sided_expression();
};







struct dent_object_instruction
{
    string name;
    dent_expression* exp;

    dent_object_instruction(const string& name, dent_expression* exp);
};

class dent_object_expression : public dent_expression
{
public:
    list<dent_object_instruction> instructions;

    dent_object_expression();
    virtual ~dent_object_expression();
};






class dent_static_value_expression : public dent_expression
{
public:
    dynamic_value value;
    dent_static_value_expression(const dynamic_value& value);
};

class dent_load_variable_expression : public dent_expression
{
public:
    string name;
    dent_load_variable_expression(const string& name);
};

class dent_function_call_expression : public dent_two_sided_expression
{
public:
    dent_function_call_expression(dent_expression* function_exp, dent_expression* args_exp);
};

class dent_instantiate_expression : public dent_two_sided_expression
{
public:
    dent_instantiate_expression(dent_expression* class_exp, dent_expression* args_exp);
};





enum dent_statement_type
{
    DENT_STATEMENT_ASSIGNMENT,
    DENT_STATEMENT_EXPRESSIONAL,
};




class dent_statement
{
public:
    const dent_statement_type type;

    dent_statement(dent_statement_type type);
    virtual ~dent_statement();
};

// an abstract class for easy expression holding
class dent_expression_statement : public dent_statement
{
public:
    dent_expression* exp;
    dent_expression_statement(dent_statement_type type, dent_expression* exp);
    virtual ~dent_expression_statement();
};

class dent_assignment_statement : public dent_expression_statement
{
public:
    string name;
    dent_assignment_statement(const string& name, dent_expression* exp);
};

class dent_expressional_statement : public dent_expression_statement
{
public:
    dent_expressional_statement(dent_expression* exp);
};



class dent_syntax_tree
{
protected:
    list<dent_statement*> statements;

public:
    dent_syntax_tree(dynamic_entity_lexer& lexer);

    void parse(dynamic_entity_lexer& lexer);
    dent_statement* parse_statement(dynamic_entity_lexer& lexer);
    dent_expression* parse_expression(dynamic_entity_lexer& lexer);
    dent_expression* parse_more_expression(dynamic_entity_lexer& lexer, dent_expression* exp);
    dent_expression* parse_object_expression(dynamic_entity_lexer& lexer);
};



}


