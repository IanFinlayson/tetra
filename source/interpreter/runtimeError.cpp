#include "frontend.h"
#include "backend.h"

RuntimeError::RuntimeError(const std::string& pMessage, int pLine, TetraContext& pContext) : Error(pMessage,pLine), context(pContext) {
  //nothing else
}

TetraContext& RuntimeError::getContext() {
  return context;
}
