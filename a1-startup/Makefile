CXX=g++
CXXFLAGS= -Wall -Werror -std=c++20 -g 

all: copier mtcopier

copier: copier_files/copier.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ -lpthread

mtcopier: mtcopier_files/reader.cpp mtcopier_files/writer.cpp mtcopier_files/main.cpp mtcopier_files/queue.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ -lpthread

clean:
	rm -f copier mtcopier
