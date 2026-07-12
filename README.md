# Custom RAG

A RAG (retrieval-augmented generation) pipeline written in C++. You ask a
question, an LLM turns it into search terms, the terms are looked up in an
inverted index built over your documents, and the matching documents get fed
back to the LLM to answer the question using only those documents.

Retrieval is keyword matching, not embeddings. No vector database. The
multimap, tokenizer, and index are implemented from scratch.

## How it works

```
question --> LLM (terms.txt prompt) --> sets of search terms
         --> inverted index query (AND within a set, OR across sets)
         --> up to 10 matching documents
         --> LLM (summarize.txt prompt) --> answer grounded in the documents
```

## Files

| file            | role                                                    |
|-----------------|---------------------------------------------------------|
| `multimap.*`    | BST multimap from scratch: term -> vector of doc names, with an iterator |
| `tokenizer.*`   | splits text into lowercase alphanumeric tokens          |
| `index.*`       | inverted index on top of the multimap, multi-term AND queries via set intersection |
| `agent.*`       | the pipeline: fill prompt templates, parse LLM terms, query index, summarize |
| `provided.*`    | HTTP layer for the OpenRouter API (WinHTTP on Windows, curl otherwise) |
| `terms.txt`     | prompt template for extracting search terms (`{query}`) |
| `summarize.txt` | prompt template for the final answer (`{query}`, `{documents}`) |

## Setup

1. Make a file called `.orkey` in the working directory with your
   [OpenRouter](https://openrouter.ai) API key on one line. It's gitignored.
   Don't commit it.
2. Put your `.txt` documents in a folder and point `DOCUMENT_DIRECTORY` in
   `main.cpp` at it.
3. Build in Visual Studio (Windows, uses WinHTTP, no dependencies). A
   Makefile is included for Linux/macOS (needs libcurl).

## Run

```
Loading documents...
Indexed 42 documents.
Enter question (or 'quit' to exit): what did the report say about battery life?

The documents state that battery life averaged 11 hours under mixed use...
```

## Design decisions worth being able to explain

- **Keyword retrieval instead of embeddings** -- no vector DB dependency and
  the retrieval is fully inspectable. The LLM term-extraction step gets back
  some of the flexibility embeddings would give. Embedding-based retrieval is
  the natural upgrade.
- **AND within a term set, OR across sets** -- each line the LLM outputs is a
  strict query (doc must contain every term), and generating up to 20
  alternative lines is what gives recall.
- **Grounding** -- the final prompt tells the model to only use the retrieved
  documents, which cuts hallucination vs. asking the LLM directly.

## Known limitations

- No relevance ranking, documents aren't scored
- Context capped at the first 10 matching documents
- No embeddings / semantic search
- JSON parsing is hand-rolled for this API's response shape
- The multimap BST is unbalanced (worst case O(n) lookups)
