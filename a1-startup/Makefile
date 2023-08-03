CXX=g++
CXXFLAGS= -Wall -Werror -std=c++20 -g -O

all: copier mtcopier

copier: copier_files/main.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ -lpthread

mtcopier: mtcopier_files/reader.cpp mtcopier_files/writer.cpp mtcopier_files/main.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ -lpthread

clean:
	rm -f copier mtcopier
