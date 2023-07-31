CXX=g++
CXXFLAGS= -Wall -Werror -std=c++20 -lpthread -g -O

all: copier mtcopier

copier: copier_files/Copier.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

mtcopier: Queue.cpp Reader.cpp Writer.cpp MTCopier.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f copier mtcopier
