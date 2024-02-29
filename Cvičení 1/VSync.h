#pragma once
class VSync
{
public:
	static VSync& getInstance();
	void enable();
	void disable();
	bool isEnabled;

private:
	VSync() {} // Private constructor to prevent instantiation
	~VSync() {} // Private destructor to prevent deletion
	VSync(const VSync&) = delete; // Delete copy constructor
	VSync& operator=(const VSync&) = delete; // Delete assignment operator
};

