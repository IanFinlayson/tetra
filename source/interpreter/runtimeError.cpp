#include "frontend.hpp"
#include "backend.hpp"

RuntimeError::RuntimeError(const std::string& pMessage, int pLine, TetraContext& pContext) :
		Error(pMessage,pLine), context(pContext) {
	//nothing else
}

TetraContext& RuntimeError::getContext() {
	return context;
}
