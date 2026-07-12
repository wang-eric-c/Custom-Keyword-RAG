#ifndef MULTIMAP_H
#define MULTIMAP_H

#include "provided.h"
#include <string>

class Multimap : public MultimapBase
{
public:
    // Nested Iterator class
    class Iterator : public MultimapBase::IteratorBase
    {
    public:
        // TODO: declare at least one constructor

        Iterator(std::vector<std::string>* values);

        virtual bool next(std::string& value);

    private:
        // TODO: add private members
        int m_index;
        const std::vector<std::string>* m_vals;
    };

    Multimap();
    virtual ~Multimap();
    virtual void put(const std::string& key, const std::string& value);
    virtual MultimapBase::IteratorBase* get(const std::string& key) const;
    virtual bool empty() const;
    virtual int size() const;

private:
    // TODO: add private members
    struct Node {
        std::string m_key;
        std::vector<std::string> m_values;

        Node* left = nullptr;
        Node* right = nullptr;

        Node(std::string key, std::string val) : m_key(key) {
            m_values.push_back(val);
        }

        ~Node() {
            delete left;
            delete right;
        }
    };

    Node* m_head;
    int m_count;
};

#endif // MULTIMAP_H