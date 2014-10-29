/*#ifndef RUNTIME_ERROR_H
#define RUNTIME_ERROR_H

#include "frontend.hpp"
#include "backend.hpp"

class RuntimeError: public Error{
public:
	RuntimeError(const std::string& pMessage, int pLine, TetraContext& pContext);
	TetraContext& getContext();
private:
	TetraContext context;
};

#endif
*/
