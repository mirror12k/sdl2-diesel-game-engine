
#include "dynamic_entity_parser.hpp"



namespace diesel
{



dent_token::dent_token(dent_token_type type, const string& data)
: type(type), data(data)
{}






dynamic_entity_lexer::dynamic_entity_lexer(const string& text)
{
    this->parse(text);
}

void dynamic_entity_lexer::parse(const string& text)
{
    this->line_number = 0;
    this->line_offset = 0;

    uint offset = this->skip_whitespace(text, 0);
    uint found;
    if
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



}


