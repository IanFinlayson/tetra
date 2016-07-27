#include "ideCommandObserver.h"
#include <sstream>
#include "backend.h"
#include "frontend.h"

// Global symbol table
extern std::map<tstring, Symbol, less<tstring>, gc_allocator<pair<tstring, Symbol> > > globals;

IDECommandObserver::IDECommandObserver() {
}

IDECommandObserver::~IDECommandObserver() {
}

// Notifies the observer that a certain node in the program has been reached
void IDECommandObserver::notify_E(const Node* foundNode, TetraContext& context) {
    UNUSED(foundNode);
    UNUSED(context);
}

void IDECommandObserver::threadCreated_E(int threadNum, TetraContext& context) {
    UNUSED(threadNum);
    UNUSED(context);
}

void IDECommandObserver::threadDestroyed_E(int threadNum) {
    UNUSED(threadNum);
}

void IDECommandObserver::step_E(TetraContext& context) {
    UNUSED(context);
}

void IDECommandObserver::next_E(TetraContext& context) {
    UNUSED(context);
}

bool IDECommandObserver::break_E(int lineNum) {
    UNUSED(lineNum);
    return false;
}

// Removes a breakpoint from the given line number. Returns false if no
// breakpoint exists
bool IDECommandObserver::remove_E(int lineNum) {
    UNUSED(lineNum);
    return false;
}

void IDECommandObserver::continue_E() {
}

void IDECommandObserver::leftScope_E(TetraContext& context) {
    UNUSED(context);
}

void IDECommandObserver::notifyThreadSpecificVariable_E(tstring varName) {
    UNUSED(varName);
}

void IDECommandObserver::setYieldEnabled(bool enable) {
    UNUSED(enable);
}

std::vector<int, gc_allocator<int> > IDECommandObserver::getThreadLocations() {
    std::vector<int, gc_allocator<int> > dummy;
    return dummy;
}


