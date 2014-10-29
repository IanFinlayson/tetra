#include"backend.hpp"

class CommandConsole : public VirtualConsole {
public:
	std::string receiveStandardInput() const;

	void processStandardOutput(const std::string output) const;
};
