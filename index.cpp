#include "index.h"
#include "provided.h"
#include <string>
#include <vector>
#include <set>

#include <fstream>

Index::Index()
{
    m_multimap = create_multimap();
}

Index::~Index()
{
    delete m_multimap;
}

int Index::build_index(const std::string& path)
{
    std::vector<std::string> files = get_filenames(path);
    int count = 0;

    for (const std::string& file_name : files) {
        add_doc(file_name);
        count++;
    }

    return count; 
}

void Index::add_doc(const std::string& doc_file)
{
    std::ifstream infile(doc_file);
    if (!infile.is_open()) return;

    std::string text;
    char c;
    while (infile.get(c)) {
        text += c;
    }

    TokenizerBase* m_tokenizer = create_tokenizer();
    m_tokenizer->tokenize(text);

    std::string word;
    while (m_tokenizer->next(word)) {
        m_multimap->put(word, doc_file);
    }

    delete m_tokenizer;
}

std::vector<std::string> Index::query(const std::vector<std::string>& terms) const
{
    if (terms.empty()) {
        return {};
    }

    std::set<std::string> final;
    bool isFirst = true;

    for (const std::string& term : terms) {
        MultimapBase::IteratorBase* m_iterator = m_multimap->get(term);

        std::set<std::string> tempDocs;
        std::string doc;
        while (m_iterator->next(doc)) {
            tempDocs.insert(doc);
        }
        delete m_iterator;

        if (isFirst) {
            final = tempDocs;
            isFirst = false;
        }
        else {
            std::set<std::string> intersection;
            for (const std::string& s : final) {
                if (tempDocs.count(s)) {
                    intersection.insert(s);
                }
            }

            final = intersection;
        }

    }


    return std::vector<std::string>(final.begin(), final.end()); 
}