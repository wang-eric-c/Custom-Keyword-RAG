#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "provided.h"
#include <string>

class Tokenizer : public TokenizerBase
{
public:
    Tokenizer();
    virtual void tokenize(const std::string& input);
    virtual bool next(std::string& token);

private:
    // TODO: add private members
    std::string s;
    int m_index;
};

#endif // TOKENIZER_H