
#include "dynamic_entity_parser.hpp"


#include "exception.hpp"



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

dent_statement* parse_statement(dynamic_entity_lexer& lexer)
{
    if (lexer.is_next_token(DENT_TOKEN_NAME) and lexer.is_after_next_token(DENT_TOKEN_SYMBOL, "="))
    {
        string name = lexer.next_token().data;
        lexer.skip_token();
        return dent_assignment_statement(name, this->parse_expression(lexer));
    }
    else
    {
        return dent_expressional_statement(this->parse_expression(lexer));
    }
}



dent_expression* parse_expression(dynamic_entity_lexer& lexer)
{
    if (lexer.is_next_token(DENT_TOKEN_NAME, "new") and lexer.is_after_next_token(DENT_TOKEN_NAME) and
            lexer.is_n_token(DENT_TOKEN_SYMBOL, "(", 2))
    {

    }
}



}


