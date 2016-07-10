
#pragma once


#include <list>
using std::list;



namespace diesel
{



enum dent_token_type
{
    DENT_TOKEN_SYMBOL,
    DENT_TOKEN_NAME,
    DENT_TOKEN_STRING,
    DENT_TOKEN_INTEGER,
    DENT_TOKEN_FLOAT,
}



struct dent_token
{
    dent_token_type type;
    string data;
    uint line_number;
    uint offset_char;

    dent_token(dent_token_type type, const string& data);
};



class dynamic_entity_lexer
{
protected:
    list<const dent_token> tokens;

    uint line_number, line_offset;

public:
    dynamic_entity_lexer(const string& text);

    void parse(const string& text);
    uint skip_whitespace(const string& text, uint offset);

    const dent_token next_token();
    bool is_next_token(dent_token_type type) const;
    bool is_n_token(dent_token_type type, int offset) const;

    bool is_end() const;

};



}


