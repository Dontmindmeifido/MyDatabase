#pragma once
#include <GLFW/glfw3.h> 
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/misc/cpp/imgui_stdlib.h"
#include <math.h>
#include <iostream>
#include <string>
#include "Interpreter.h"

class Interface {   
    static Interface* instance;

    std::string queryBuffer{"CREATE FIRST_NAME: VARCHAR, LAST_NAME: VARCHAR, BIRTHDATE: DATETIME in MODEL_EMPLOYEES;\nADDROW John, Cena, 12-12-1999 in MODEL_EMPLOYEES;\nDELROW 1 in MODEL_EMPLOYEES;\nREAD SURNAME in EMPLOYEES where SALARY > 300;"};
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