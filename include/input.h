#pragma once
#include "common.h"
#include "map"
#include "string"
using namespace std;

typedef void (*FuncPtr)(float, GLFWwindow* w);
class CustomInput
{
private:
    map<int, FuncPtr> set_btn;
    GLFWwindow* w;
public:
    CustomInput();
    void init(GLFWwindow* window);
    ~CustomInput();
    void BindPress(int btn, void (*lambda)(float, GLFWwindow* w));
    void Input_loop(float dt, GLFWwindow* w);
};

CustomInput::CustomInput()
{
}

CustomInput::~CustomInput()
{
}

void CustomInput::init(GLFWwindow* window) {
    w = window;
}

void CustomInput::BindPress(int btn, void (*lambda)(float, GLFWwindow* w)) {
    set_btn.insert(pair<int, FuncPtr>(btn, lambda));
}

void CustomInput::Input_loop(float dt, GLFWwindow* w) {
    for (auto it = set_btn.begin(); it != set_btn.end(); ++it) {
        if (glfwGetKey(w, it->first) == GLFW_PRESS) {
            it->second(dt, w);
        }
    }
}