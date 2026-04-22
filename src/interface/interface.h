#pragma once
#include <math.h>
#include <iostream>
#include <string>
#include <GLFW/glfw3.h> 
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "misc/cpp/imgui_stdlib.h"
#include "../Interpreter/Interpreter.h"

class Interface {   
    static Interface* instance;

    std::string queryBuffer{""};
    std::vector<Table*>* READRESPONSE;
    int selectedTable{-1};

    void DatabaseWindow();
    void EditorWindow(int buttonHeight = 30);
    void EditorResponse();

    Interface();

public:
    static Interface* getInstance();

    GLFWwindow* initWindow();

    void Render(GLFWwindow* window);
};