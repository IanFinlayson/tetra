/*#ifndef SYSTEM_ERROR_H
#define SYSTEM_ERROR_H

#include "frontend.hpp"
#include "backend.hpp"

class SystemError: public Error{
public:
	RuntimeError(const std::string& pMessage, int pLine, const Node* pContext);
	const Node* getNode();
private:
	const Node* node;
};

#endif
*/
