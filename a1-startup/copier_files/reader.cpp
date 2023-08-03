/**
 * startup code provided by Paul Miller for COSC1114 - Operating Systems
 * Principles
 **/
#include "reader.h"
#include <functional>

reader::reader(const std::string& name, writer& mywriter): thewriter(mywriter){
	this->in = std::ifstream(name);
}

void reader::run() {

}
