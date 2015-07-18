#include "frontend.hpp"
#include "backend.hpp"

SystemError::SystemError(const std::string& pMessage, int pLine, const Node* pNode) : Error(pMessage,pLine), node(pNode) {
	//nothing else
}

const Node* SystemError::getNode() {
	return node;
}
