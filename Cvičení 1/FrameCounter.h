#pragma once
#include <string>

class FrameCounter
{
public:
	FrameCounter();

	bool hasSecondPassed();
	int getNumberOfFrames();
	void setNumberOfFrames(int);
	std::string render();
private:
	double previousTime;
	double currentTime;
	int frameCount;
	int lastFrameCount;
};

