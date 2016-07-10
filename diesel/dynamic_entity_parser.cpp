
#include "dynamic_entity_parser.hpp"


#include "exception.hpp"



namespace diesel
{



dent_token::dent_token(dent_token_type type, const string& data, uint line_number, uint line_offset, uint text_offset)
: type(type), data(data), line_number(line_number), line_offset(line_offset), text_offset(text_offset)
{}

string dent_token::to_string() const
{
    return "token type #" + std::to_string(this->type) + " '" + this->data + "'";
}









dynamic_entity_lexer::dynamic_entity_lexer(const string& text)
{
    this->parse(text);
}

void dynamic_entity_lexer::parse(const string& text)
{
    this->line_number = 0;
    this->line_offset = 0;

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
            this->line_offset = 0;
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



dent_token dynamic_entity_lexer::next_token()
{
    if (this->is_end())
        throw generic_exception("TODO past-end-of-list");

    dent_token ret = this->tokens.front();
    this->tokens.pop_front();
    return ret;
}



bool dynamic_entity_lexer::is_end() const
{
    return this->tokens.size() == 0;
}



}


