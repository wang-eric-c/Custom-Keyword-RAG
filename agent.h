#ifndef AGENT_H
#define AGENT_H

#include "provided.h"
#include <string>
#include <vector>
#include <set>

class Agent : public AgentBase
{
public:
    Agent(const IndexBase& index);
    ~Agent();
    virtual bool load_prompts(const std::string& terms_file, const std::string& summarize_file);
    virtual bool query(const std::string& question, std::string& answer);

private:
    const IndexBase& m_index;
    std::string m_terms_prompt;
    std::string m_summarize_prompt;
};

#endif