#include"backend.hpp"

class CommandConsole : public Console {
public:
	std::string receiveStandardInput() const;

	void processStandardOutput(const std::string output) const;
};
