#include "Errors.h"

//Exceptions

void error(const char* message) {
	throw runtime_error(message);
}