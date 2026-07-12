#include "tokenizer.h"
#include <string>

#include <cctype>

Tokenizer::Tokenizer()
    : s(""), m_index(0)
{
    // TODO: implement
}

void Tokenizer::tokenize(const std::string& input)
{
    // TODO: implement
    s = input;
    m_index = 0;
}

bool Tokenizer::next(std::string& token)
{
    while (m_index < s.size()) {
        char c = s[m_index];
        if ((c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= '0' && c <= '9')) {
            break;
        }
        m_index++;
    }

    if (m_index >= s.size()) {
        return false;
    }

    token = "";
    while (m_index < s.size()) {
        char c = s[m_index];
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
            token += tolower(c);
        }
        else if (c >= '0' && c <= '9') {
            token += c;
        }
        else {
            break;
        }
        m_index++;
    }

    return true; // TODO: replace this line with your implementation
}