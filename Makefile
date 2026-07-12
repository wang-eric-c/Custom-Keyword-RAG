PRODUCT = p4
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra 
LDFLAGS = -lcurl

all: $(PRODUCT)

$(PRODUCT): main.o agent.o index.o multimap.o tokenizer.o provided.o
	$(CXX) $(CXXFLAGS) -o $(PRODUCT) main.o agent.o index.o multimap.o tokenizer.o provided.o $(LDFLAGS)

main.o: main.cpp provided.h
	$(CXX) $(CXXFLAGS) -c main.cpp

agent.o: agent.cpp agent.h provided.h
	$(CXX) $(CXXFLAGS) -c agent.cpp

index.o: index.cpp index.h provided.h 
	$(CXX) $(CXXFLAGS) -c index.cpp

multimap.o: multimap.cpp multimap.h provided.h
	$(CXX) $(CXXFLAGS) -c multimap.cpp

tokenizer.o: tokenizer.cpp tokenizer.h provided.h
	$(CXX) $(CXXFLAGS) -c tokenizer.cpp

provided.o: provided.cpp provided.h agent.h index.h multimap.h tokenizer.h
	$(CXX) $(CXXFLAGS) -c provided.cpp

clean:
	rm -f *.o $(PRODUCT)

.PHONY: all clean
