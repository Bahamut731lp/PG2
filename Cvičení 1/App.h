#pragma once
#include "Window.h"

enum Scene {
    SceneMainMenu,
    SceneLevelOne
};

class App {
public:
    App();

    bool init(void);
    void report(void);
    void configure(void);
    int run(void);

    void addMouseHandler(void);
    void onMouseMove(GLfloat xoffset, GLfloat yoffset, GLboolean constraintPitch);

    ~App();
private:
    Window* window;
    Scene scene;
};
