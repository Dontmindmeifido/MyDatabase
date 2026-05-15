#pragma once
#include <math.h>
#include <iostream>
#include <string>
#include <GLFW/glfw3.h> 
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "misc/cpp/imgui_stdlib.h"
#include "../interpreter/interpreter.h"

class Interface {   
    static Interface* instance;
    std::string query_buffer{"Insert Jane, Karera, 21, YES in GUESTS;"};
    std::vector<Table*>* console_table_buffer;
    std::vector<std::vector<int>> colors{{50, 150, 200, 255}, 
                                         {60, 200, 160, 255},
                                         {200, 100, 50, 255},
                                         {200, 200, 220, 255},
                                         {150, 100, 100, 255},
                                         {150, 100, 150, 255},
                                         {150, 100, 150, 255},
                                         {255, 255, 255, 255}};
    int selected_table{-1};

    Interface();
    void render_database_window();
    void render_editor_window(int buttonHeight = 30);
    void render_editor_console();

public:
    static Interface* get_instance();
    GLFWwindow* init_window();
    void Render(GLFWwindow* window);
};