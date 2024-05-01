#include "FrameCounter.h"
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include <iostream>
#include <string>

FrameCounter::FrameCounter()
{
    frameCount = 0;
    lastFrameCount = 0;
    previousTime = glfwGetTime();
    currentTime = glfwGetTime();
}

bool FrameCounter::hasSecondPassed()
{
    currentTime = glfwGetTime();
    frameCount++;

    return currentTime - previousTime >= 1.0;
}

int FrameCounter::getNumberOfFrames()
{
    return frameCount;
}

void FrameCounter::setNumberOfFrames(int frames)
{
    lastFrameCount = frameCount;
    frameCount = frames;
    previousTime = glfwGetTime();
}

std::string FrameCounter::render()
{
    return "FPS :" + std::to_string(this->lastFrameCount);
}
