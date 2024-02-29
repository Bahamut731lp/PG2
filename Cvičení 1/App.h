#pragma once

class App {
public:
    App();

    bool init(void);
    void report(void);
    int run(void);

    ~App();
private:
    GLFWwindow* window;
};
