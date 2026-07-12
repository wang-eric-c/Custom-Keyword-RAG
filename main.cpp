#include "provided.h"
#include <iostream>
#include <string>

const std::string DOCUMENT_DIRECTORY = "C:/Users/wange/OneDrive/Desktop/skeleton/docs";


int main() {

    // Build index from all files in the docs directory
    std::cout << "Loading documents..." << std::endl;
    IndexBase* index = create_index();
    int numDocs = index->build_index(DOCUMENT_DIRECTORY);
    std::cout << "Indexed " << numDocs << " documents." << std::endl;

    AgentBase* agent = create_agent(*index);
    agent->load_prompts("terms.txt", "summarize.txt");

    std::string input;
    for (;;) {
        std::cout << "Enter question (or 'quit' to exit): ";
        std::getline(std::cin, input);

        if (input.empty()) continue;
        if (input == "quit" || input == "exit" || input == "q") break;

        std::cout << std::endl;
        std::string answer;
        if (!agent->query(input, answer)) {
            std::cerr << "Query failed." << std::endl;
        } else {
            std::cout << answer << std::endl;
        }
        std::cout << std::endl;
    }

    delete agent;
    delete index;
}
