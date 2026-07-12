#include "agent.h"
#include "provided.h"
#include <string>

#include <set>

#include <fstream>
#include <sstream>

#include <iostream>


Agent::Agent(const IndexBase& index)
    : m_index(index)
{
}

Agent::~Agent()
{
}

bool Agent::load_prompts(const std::string& terms_file, const std::string& summarize_file)
{
    char c;

    std::ifstream t_file(terms_file);
    if (!t_file.is_open()) {
        return false;
    }
    m_terms_prompt = "";

    while (t_file.get(c)) {
        m_terms_prompt += c;
    }
    t_file.close();

    std::ifstream s_file(summarize_file);
    if (!s_file.is_open()) {
        return false;
    }
    m_summarize_prompt = "";
    while (s_file.get(c)) {
        m_summarize_prompt += c;
    }
    s_file.close();

    return true;
}

bool Agent::query(const std::string& question, std::string& answer)
{
    if (m_terms_prompt.empty() || m_summarize_prompt.empty()) return false;

    //Getting the LLM Terms
    std::string termsPrompt = "";
    for (int i = 0; i < m_terms_prompt.size(); i++) {
        if (i + 6 < m_terms_prompt.size() && m_terms_prompt.substr(i, 7) == "{query}") {
            termsPrompt += question;
            i += 6;
        }
        else {
            termsPrompt += m_terms_prompt[i];
        }
    }


    std::string llmTermsResponse;
    if (!query_llm("terms", termsPrompt, llmTermsResponse)) {
        return false;
    }

    std::vector<std::vector<std::string>> final_terms_container;
    std::stringstream ss(llmTermsResponse);
    std::string line;
    while (std::getline(ss, line)) {
        TokenizerBase* t = create_tokenizer();
        t->tokenize(line);
        std::vector<std::string> currentTerms;
        std::string word;
        while (t->next(word)) {
            currentTerms.push_back(word);
        }
        delete t;
        if (!currentTerms.empty()) final_terms_container.push_back(currentTerms);
    }

    //Query the Documents
    std::set<std::string> uniqueDocs;
    for (const std::vector<std::string>& group : final_terms_container) {
        std::vector<std::string> results = m_index.query(group);
        for (const std::string& docs : results) {
            uniqueDocs.insert(docs);
        }
    }
    if (uniqueDocs.empty()) return false;


    //Final result
    std::string allDocsText = "";

    int count = 0;
    for (const std::string& doc : uniqueDocs) {
        if (count == 10) {
            break;
        }

        std::ifstream docFile(doc);
        if (docFile.is_open()) {
            char c;
            while (docFile.get(c)) {
                allDocsText += c;
            }
            allDocsText += "\n";
            count++;
        }
    }

    std::string final_prompt = "";
    for (int i = 0; i < m_summarize_prompt.size(); i++) {
        if (i + 6 < m_summarize_prompt.size() && m_summarize_prompt.substr(i, 7) == "{query}") {
            final_prompt += question;
            i += 6;
        }
        else if (i + 10 < m_summarize_prompt.size() && m_summarize_prompt.substr(i, 11) == "{documents}") {
            final_prompt += allDocsText;
            i += 10;
        }
        else {
            final_prompt += m_summarize_prompt[i];
        }
    }

    return query_llm("summarize", final_prompt, answer);
}