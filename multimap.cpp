#include "multimap.h"
#include <string>

// TODO: implement at least one constructor
//Multimap::Iterator::Iterator( /* your constructor arguments */ )
//{
//    // TODO: implement
//}

Multimap::Iterator::Iterator(std::vector<std::string>* values)
	: m_vals(values), m_index(0)
{
}

bool Multimap::Iterator::next(std::string& value) {
	if (m_vals == nullptr || m_index >= m_vals->size()) {
		return false;
	}

	value = (*m_vals)[m_index];
	m_index++;
	return true;
}


Multimap::Multimap()
	: m_count(0), m_head(nullptr)
{
	// TODO: implement
}

Multimap::~Multimap()
{
	// TODO: implement
	delete m_head;
}

void Multimap::put(const std::string& key, const std::string& value)
{
	if (m_head == nullptr) {
		m_head = new Node(key, value);
		m_count++;
		return;
	}

	Node* curr = m_head;
	while (curr != nullptr) {
		if (curr->m_key == key) {
			for (const std::string s : curr->m_values) {
				if (s == value) return;
			}
			curr->m_values.push_back(value);
			m_count++;
			return;
		}
		if (key < curr->m_key) {
			if (curr->left == nullptr) {
				curr->left = new Node(key, value);
				m_count++;
				return;
			}
			curr = curr->left;
		}
		else {
			if (curr->right == nullptr) {
				curr->right = new Node(key, value);
				m_count++;
				return;
			}
			curr = curr->right;
		}
	}
}

MultimapBase::IteratorBase* Multimap::get(const std::string& key) const
{
	Node* curr = m_head;
	while (curr != nullptr) {
		if (curr->m_key == key) {
			return new Iterator(&(curr->m_values));
			break;
		}
		if (key < curr->m_key) {
			curr = curr->left;
		}
		else {
			curr = curr->right;
		}
	}
	return new Iterator(nullptr); // TODO: replace this line with your implementation
}

bool Multimap::empty() const
{
	return (m_count == 0); // TODO: replace this line with your implementation
}

int Multimap::size() const
{
	return m_count; // TODO: replace this line with your implementation
}