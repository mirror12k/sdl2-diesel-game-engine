
#include "dynamic_entity_parser.hpp"


#include "exception.hpp"



#include <iostream>

using std::cout;
using std::endl;



namespace diesel
{



dent_token::dent_token(dent_token_type type, const string& data, uint line_number, uint line_offset, uint text_offset)
: type(type), data(data), line_number(line_number), line_offset(line_offset), text_offset(text_offset)
{}

string dent_token::to_string() const
{
    return "token type #" + std::to_string(this->type) + " '" + this->data +
            "' from line #" + std::to_string(this->line_number) + " offset " + std::to_string(this->line_offset);
}









dynamic_entity_lexer::dynamic_entity_lexer(const string& text)
{
    this->parse(text);
}

void dynamic_entity_lexer::parse(const string& text)
{
    // for some reason everyone refers to line numbers and column numbers from 1...
    this->line_number = 1;
    this->line_offset = 1;

    uint offset = this->skip_whitespace(text, 0);
    while (offset < text.size())
    {
        if (text.compare(offset, 1, "(") == 0)
            this->tokens.push_back(dent_token(DENT_TOKEN_SYMBOL, "(", this->line_number, this->line_offset++, offset++));
        else if (text.compare(offset, 1, ")") == 0)
            this->tokens.push_back(dent_token(DENT_TOKEN_SYMBOL, ")", this->line_number, this->line_offset++, offset++));
        else if (text.compare(offset, 1, "=") == 0)
            this->tokens.push_back(dent_token(DENT_TOKEN_SYMBOL, "=", this->line_number, this->line_offset++, offset++));
        else if (text.compare(offset, 1, ",") == 0)
            this->tokens.push_back(dent_token(DENT_TOKEN_SYMBOL, ",", this->line_number, this->line_offset++, offset++));
        else if (text.compare(offset, 1, "\"") == 0)
            offset = this->extract_string(text, offset);
        else if (text.compare(offset, 1, "'") == 0)
            offset = this->extract_string(text, offset);
        else if ((text.at(offset) >= '0') && (text.at(offset) <= '9'))
            offset = this->extract_number(text, offset);
        else if (((text.at(offset) >= 'a') && (text.at(offset) <= 'z')) || ((text.at(offset) >= 'A') && (text.at(offset) <= 'Z')))
            offset = this->extract_name(text, offset);
        else
            throw generic_exception("TODO");

        offset = this->skip_whitespace(text, offset);
    }
}





uint dynamic_entity_lexer::skip_whitespace(const string& text, uint offset)
{
    while (offset < text.size())
    {
        char c = text.at(offset);
        if ((c == ' ') || (c == '\t') || (c == '\r'))
        {
            this->line_offset++;
            offset++;
        }
        else if (c == '\n')
        {
            this->line_number++;
            this->line_offset = 1;
            offset++;
        }
        else if (c == '#')
        {
            while ((offset < text.size()) && (text.at(offset) != '\n'))
                offset++;
        }
        else
            return offset;
    }
    return offset;
}



uint dynamic_entity_lexer::extract_string(const string& text, uint offset)
{
    char quote = text.at(offset);
    offset++;
    this->line_offset++;

    uint found = text.find(quote, offset);
    if (found == string::npos)
        throw generic_exception("missing closing quote");

    this->tokens.push_back(dent_token(DENT_TOKEN_STRING, text.substr(offset, found - offset), this->line_number, this->line_offset - 1, offset - 1));

    for (; offset <= found; offset++)
    {
        if (text.at(offset) == '\n')
        {
            this->line_number++;
            this->line_offset = 1;
        }
        else
        {
            this->line_offset++;
        }
    }

    return found + 1;
}

uint dynamic_entity_lexer::extract_number(const string& text, uint offset)
{
    uint start_offset = offset;
    uint line_offset = this->line_offset;
    while ((offset <= text.size()) && (text.at(offset) >= '0') && (text.at(offset) <= '9'))
    {
        offset++;
        this->line_offset++;
    }

    this->tokens.push_back(dent_token(DENT_TOKEN_INTEGER, text.substr(start_offset, offset - start_offset),
        this->line_number, line_offset, start_offset));

    return offset;
}

uint dynamic_entity_lexer::extract_name(const string& text, uint offset)
{
    uint start_offset = offset;
    uint line_offset = this->line_offset;

    offset++;
    this->line_offset++;

    char c = text.at(offset);
    while ((offset <= text.size()) && (
        ((c >= 'a') && (c <= 'z')) ||
        ((c >= 'A') && (c <= 'Z')) ||
        ((c >= '0') && (c <= '9')) ||
        (c == '_')
    ))
    {
        offset++;
        this->line_offset++;
        c = text.at(offset);
    }

    this->tokens.push_back(dent_token(DENT_TOKEN_NAME, text.substr(start_offset, offset - start_offset),
        this->line_number, line_offset, start_offset));

    return offset;
}






dent_token dynamic_entity_lexer::next_token()
{
    if (this->is_end())
        throw generic_exception("TODO past-end-of-list");

    return this->tokens[this->tokens_iteration_index++];
}

void dynamic_entity_lexer::skip_token()
{
    if (this->is_end())
        throw generic_exception("TODO past-end-of-list");

    this->tokens_iteration_index++;
}


void dynamic_entity_lexer::assert_skip_token(dent_token_type type)
{
    if (this->is_next_token(type))
        this->skip_token();
    else
        throw generic_exception("TODO token assertion failed");
}
void dynamic_entity_lexer::assert_skip_token(dent_token_type type, const string& value)
{
    if (this->is_next_token(type, value))
        this->skip_token();
    else
        throw generic_exception("TODO token assertion failed");
}



bool dynamic_entity_lexer::is_next_token(dent_token_type type) const
{
    if (this->is_end())
        throw generic_exception("TODO past-end-of-list");

    return this->tokens[this->tokens_iteration_index].type == type;
}

bool dynamic_entity_lexer::is_next_token(dent_token_type type, const string& value) const
{
    if (this->is_end())
        throw generic_exception("TODO past-end-of-list");

    return (this->tokens[this->tokens_iteration_index].type == type) and (this->tokens[this->tokens_iteration_index].data == value);
}

bool dynamic_entity_lexer::is_after_next_token(dent_token_type type) const
{
    if (this->tokens_iteration_index + 1 == this->tokens.size())
        throw generic_exception("TODO past-end-of-list");

    return this->tokens[this->tokens_iteration_index + 1].type == type;
}

bool dynamic_entity_lexer::is_after_next_token(dent_token_type type, const string& value) const
{
    if (this->tokens_iteration_index + 1 == this->tokens.size())
        throw generic_exception("TODO past-end-of-list");

    return (this->tokens[this->tokens_iteration_index + 1].type == type) and (this->tokens[this->tokens_iteration_index + 1].data == value);
}

bool dynamic_entity_lexer::is_n_token(dent_token_type type, int offset) const
{
    if (this->tokens_iteration_index + offset == this->tokens.size())
        throw generic_exception("TODO past-end-of-list");

    return this->tokens[this->tokens_iteration_index + offset].type == type;
}

bool dynamic_entity_lexer::is_n_token(dent_token_type type, const string& value, int offset) const
{
    if (this->tokens_iteration_index + offset == this->tokens.size())
        throw generic_exception("TODO past-end-of-list");

    return (this->tokens[this->tokens_iteration_index + offset].type == type) and (this->tokens[this->tokens_iteration_index + offset].data == value);
}








bool dynamic_entity_lexer::is_end() const
{
    return this->tokens_iteration_index == this->tokens.size();
}











dent_expression::dent_expression(dent_expression_type type)
: type(type)
{}

dent_expression::~dent_expression()
{}


dent_expression_expression::dent_expression_expression(dent_expression_type type, dent_expression* exp)
: dent_expression(type), exp(exp)
{}

dent_expression_expression::~dent_expression_expression()
{
    delete this->exp;
}

dent_two_sided_expression::dent_two_sided_expression(dent_expression_type type, dent_expression* left, dent_expression* right)
: dent_expression(type), left(left), right(right)
{}

dent_two_sided_expression::~dent_two_sided_expression()
{
    delete this->left;
    delete this->right;
}



dent_static_value_expression::dent_static_value_expression(const dynamic_value& value)
: dent_expression(DENT_EXPRESSION_STATIC_VALUE), value(value)
{}


dent_load_variable_expression::dent_load_variable_expression(const string& name)
: dent_expression(DENT_EXPRESSION_LOAD_VARIABLE), name(name)
{}


dent_function_call_expression::dent_function_call_expression(dent_expression* function_exp, dent_expression* args_exp)
: dent_two_sided_expression(DENT_EXPRESSION_FUNCTION_CALL, function_exp, args_exp)
{}

dent_instantiate_expression::dent_instantiate_expression(dent_expression* class_exp, dent_expression* args_exp)
: dent_two_sided_expression(DENT_EXPRESSION_INSTANTIATION, class_exp, args_exp)
{}

dent_object_instruction::dent_object_instruction(const string& name, dent_expression* exp)
: name(name), exp(exp)
{}


dent_object_expression::dent_object_expression()
: dent_expression(DENT_EXPRESSION_OBJECT)
{}

dent_object_expression::~dent_object_expression()
{
    for (list<dent_object_instruction>::iterator iter = this->instructions.begin(), iter_end = this->instructions.end(); iter != iter_end; iter++)
    {
        delete iter->exp;
    }
}







dent_statement::dent_statement(dent_statement_type type)
: type(type)
{}

dent_statement::~dent_statement()
{}


dent_expression_statement::dent_expression_statement(dent_statement_type type, dent_expression* exp)
: dent_statement(type), exp(exp)
{}

dent_expression_statement::~dent_expression_statement()
{
    delete this->exp;
}


dent_assignment_statement::dent_assignment_statement(const string& name, dent_expression* exp)
: dent_expression_statement(DENT_STATEMENT_ASSIGNMENT, exp),
name(name)
{}


dent_expressional_statement::dent_expressional_statement(dent_expression* exp)
: dent_expression_statement(DENT_STATEMENT_EXPRESSIONAL, exp)
{}






dent_syntax_tree::dent_syntax_tree(dynamic_entity_lexer& lexer)
{
    this->parse(lexer);
}


void dent_syntax_tree::parse(dynamic_entity_lexer& lexer)
{
    while (not lexer.is_end())
    {
        this->statements.push_back(this->parse_statement(lexer));
    }
}

dent_statement* dent_syntax_tree::parse_statement(dynamic_entity_lexer& lexer)
{
    if (lexer.is_next_token(DENT_TOKEN_NAME) and lexer.is_after_next_token(DENT_TOKEN_SYMBOL, "="))
    {
        string name = lexer.next_token().data;
        lexer.skip_token();
        return new dent_assignment_statement(name, this->parse_expression(lexer));
    }
    else
    {
        return new dent_expressional_statement(this->parse_expression(lexer));
    }
}

dent_expression* dent_syntax_tree::parse_expression(dynamic_entity_lexer& lexer)
{
    if (lexer.is_next_token(DENT_TOKEN_NAME, "new") and lexer.is_after_next_token(DENT_TOKEN_NAME) and
            lexer.is_n_token(DENT_TOKEN_SYMBOL, "(", 2))
    {
        lexer.skip_token();
        string name = lexer.next_token().data;
        lexer.skip_token();

        dent_expression* obj = this->parse_object_expression(lexer);
        lexer.assert_skip_token(DENT_TOKEN_SYMBOL, ")");

        return new dent_instantiate_expression(new dent_load_variable_expression(name), obj);
    }
    else if (lexer.is_next_token(DENT_TOKEN_NAME))
    {
        string name = lexer.next_token().data;
        return this->parse_more_expression(lexer, new dent_load_variable_expression(name));
    }
    else if (lexer.is_next_token(DENT_TOKEN_INTEGER))
    {
        string data = lexer.next_token().data;
        return this->parse_more_expression(lexer, new dent_static_value_expression(dynamic_value(std::stoi(data))));
    }
    else if (lexer.is_next_token(DENT_TOKEN_STRING))
    {
        string data = lexer.next_token().data;
        return this->parse_more_expression(lexer, new dent_static_value_expression(dynamic_value(data)));
    }
    else
    {
        cout << "debug: " << lexer.next_token().data << endl;
        throw generic_exception("TODO failed to parse expression");
    }
}


dent_expression* dent_syntax_tree::parse_more_expression(dynamic_entity_lexer& lexer, dent_expression* exp)
{
    if (lexer.is_end())
    {
        return exp;
    }
    if (lexer.is_next_token(DENT_TOKEN_SYMBOL, "("))
    {
        lexer.skip_token();
        dent_expression* obj = this->parse_object_expression(lexer);
        lexer.assert_skip_token(DENT_TOKEN_SYMBOL, ")");

        return this->parse_more_expression(lexer, new dent_function_call_expression(exp, obj));
    }
    else
    {
        return exp;
    }
}



dent_expression* dent_syntax_tree::parse_object_expression(dynamic_entity_lexer& lexer)
{
    dent_object_expression* obj = new dent_object_expression();
    if (lexer.is_next_token(DENT_TOKEN_SYMBOL, ")") or lexer.is_next_token(DENT_TOKEN_SYMBOL, "}"))
        return obj;

    while (true)
    {
        if (not lexer.is_next_token(DENT_TOKEN_NAME))
            throw generic_exception("TODO expected name token here");

        string name = lexer.next_token().data;
        lexer.assert_skip_token(DENT_TOKEN_SYMBOL, "=");
        dent_expression* exp = this->parse_expression(lexer);

        obj->instructions.push_back(dent_object_instruction(name, exp));

        if (lexer.is_next_token(DENT_TOKEN_SYMBOL, ","))
            lexer.skip_token();
        else
            return obj;
    }
}


}


