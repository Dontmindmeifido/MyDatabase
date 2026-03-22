#include <math.h>
#include <iostream>
#include <string>

#include <GLFW/glfw3.h> 

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/misc/cpp/imgui_stdlib.h"
#include "Compiler.cpp"

string strip(string value) {
    string dummy = "";
    for (auto x: value) {
        if (x != ' ' && x != '\n') {
            dummy += x;
        }
    }

    return dummy;
}

std::string queryBuffer = "CREATE (FIRST_NAME, LAST_NAME, BIRTHDATE) in MODEL_EMPLOYEES;\nADDROW (John, Cena, 12-12-1999) in MODEL_EMPLOYEES;\nDELROW (1) in MODEL_EMPLOYEES;\nREAD (SURNAME) in EMPLOYEES;";
int idx = -1;

Lexer& lexer = *(new Lexer());
vector<Table> READRESPONSE;
Database& db = *(new Database());

void DatabaseWindow() {
    ImVec2 screenSize = ImGui::GetIO().DisplaySize;

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(screenSize.x * 0.5f, screenSize.y));
    
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImGui::GetStyleColorVec4(ImGuiCol_TitleBg));
    ImGui::Begin("Database", NULL, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

    if (ImGui::BeginMenuBar()) {
        for (int i = 0; i < db.getDatabaseData().size(); i++) {
            if (ImGui::MenuItem(&(db.getDatabaseData()[i].getTableName()[0]))) {
                if (idx != i) {
                    idx = i;
                }
            }
        }
        ImGui::EndMenuBar();
    }

    // Print table IDEA HERE: ADD TYPING SUGGESTIONS LIKE SYNTAX OR TABLE_NAME Suggestsions
    if (idx != -1) {
        ImVec2 cursor = ImGui::GetCursorScreenPos();
        Table& table = db.getDatabaseData()[idx];
        ImGui::Dummy(ImVec2(0, 20 * (int(table.getTableData().size()))));
        int w = (ImGui::GetIO().DisplaySize.x * 0.5 - 38) / min(10, int(table.getTableData()[0].getRowData().size())); // Soft limit on 10 visible cols
        ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(cursor.x, cursor.y), ImVec2(cursor.x + 20 - 1, cursor.y + 20 - 1), IM_COL32(30, 30, 30, 255));
        for (int j = 1; j < table.getTableData().size(); j++) {
            string text = to_string(j);
            ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(cursor.x, cursor.y + 20*j), ImVec2(cursor.x + 20 - 1, cursor.y + 20*(j + 1) - 1), IM_COL32(30, 30, 30, 255));
            ImGui::GetWindowDrawList()->AddText(ImVec2(((text.length() == 1) ? 5 : 1) + cursor.x, cursor.y + 20*j), IM_COL32(255, 255, 255, 255), &(text[0]), &(text[text.size()]));
        }
        for (int j = 0; j < table.getTableData()[0].getRowData().size(); j++) {
            string text = table.getTableData()[0].getRowData()[j].getValue();
            ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(20 + cursor.x + w*j, cursor.y), ImVec2(20 + cursor.x + w*(j + 1) - 1, cursor.y + 20 - 1), IM_COL32(40, 50, 30, 255));
            ImGui::GetWindowDrawList()->AddText(ImVec2(20 + 5 + cursor.x + w*j, cursor.y), IM_COL32(255, 255, 255, 255), &(text[0]), &(text[text.size()]));
        }
        for (int i = 1; i < table.getTableData().size(); i++) {
            for (int j = 0; j < table.getTableData()[i].getRowData().size(); j++) {
                string text = table.getTableData()[i].getRowData()[j].getValue();
                ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(20 + cursor.x + w*j, cursor.y + 20*i), ImVec2(20 + cursor.x + w*(j + 1) - 1, cursor.y + 20*(i + 1) - 1), IM_COL32(30, 30, 30, 255));
                if (ImGui::IsMouseHoveringRect(ImVec2(20 + cursor.x + w*j, cursor.y + 20*i), ImVec2(20 + cursor.x + w*(j + 1) - 1, cursor.y + 20*(i + 1) - 1))) {
                    ImGui::BeginTooltip();
                    ImGui::Text(&(table.getTableData()[i].getRowData()[j].getData()->getType()[0]));
                    ImGui::Text(&(("ROW " + to_string(i) + " " + "COL " + to_string(j + 1))[0]));
                    ImGui::EndTooltip();
                }
                ImGui::GetWindowDrawList()->AddText(ImVec2(20 + 5 + cursor.x + w*j, cursor.y + 20*i), IM_COL32(255, 255, 255, 255), &(text[0]), &(text[text.size()]));
            }
        }
    }

    ImGui::End();
    ImGui::PopStyleColor(1);
}

void EditorWindow(int buttonHeight = 30) {
    ImVec2 screenSize = ImGui::GetIO().DisplaySize;
    ImGui::SetNextWindowPos(ImVec2(screenSize.x * 0.5f, 0));
    ImGui::SetNextWindowSize(ImVec2(screenSize.x * 0.5f, screenSize.y - 200));

    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImGui::GetStyleColorVec4(ImGuiCol_TitleBg));
    ImGui::Begin("Editor", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);


    // WIPPPP! Very crude SYNTAX HIGHLIGHTER WIPPPP! ------------ BUG: DOES NOT HANDLE TABS!!
    vector<string> wordList = lexer.getWords(queryBuffer);
    for (int i = 0; i < wordList.size(); i++) {
        if (retToLower(strip(wordList[i])) == "create") {
            ImGui::GetWindowDrawList()->AddText(ImVec2(ImGui::GetCursorScreenPos().x + 3, ImGui::GetCursorScreenPos().y + 3), IM_COL32(50, 150, 200, 255), &(wordList[i][0]));
        } else if (retToLower(strip(wordList[i])) == "addrow") {
            ImGui::GetWindowDrawList()->AddText(ImVec2(ImGui::GetCursorScreenPos().x + 3, ImGui::GetCursorScreenPos().y + 3), IM_COL32(60, 200, 160, 255), &(wordList[i][0]));
        } else if (retToLower(strip(wordList[i])) == "delrow") {
            ImGui::GetWindowDrawList()->AddText(ImVec2(ImGui::GetCursorScreenPos().x + 3, ImGui::GetCursorScreenPos().y + 3), IM_COL32(200, 100, 50, 255), &(wordList[i][0]));
        } else if (retToLower(strip(wordList[i])) == "read") {
            ImGui::GetWindowDrawList()->AddText(ImVec2(ImGui::GetCursorScreenPos().x + 3, ImGui::GetCursorScreenPos().y + 3), IM_COL32(200, 200, 220, 255), &(wordList[i][0]));
        } else if (retToLower(strip(wordList[i])) == "in") {
            ImGui::GetWindowDrawList()->AddText(ImVec2(ImGui::GetCursorScreenPos().x + 3, ImGui::GetCursorScreenPos().y + 3), IM_COL32(150, 100, 100, 255), &(wordList[i][0]));
        } else if (wordList[i] != "") {
            ImGui::GetWindowDrawList()->AddText(ImVec2(ImGui::GetCursorScreenPos().x + 3, ImGui::GetCursorScreenPos().y + 3), IM_COL32(255, 255, 255, 255), &(wordList[i][0]));
        }
    }

    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.8f, 0.0f)); // Transparent
    ImGui::InputTextMultiline(
        "##Editor", 
        &queryBuffer, // Add type hints
        ImVec2(-FLT_MIN, ImGui::GetWindowHeight() - 2.5 * buttonHeight), 
        ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_CtrlEnterForNewLine
    );
    ImGui::PopStyleColor(2);


    if (ImGui::Button("Compile Script", ImVec2(150, buttonHeight))) {
        std::cout << "\n[COMPILER INTERCEPT] Executing DFA on:\n";
        std::cout << queryBuffer << "\n";

        READRESPONSE.clear();
        lexer.runLexer(db, queryBuffer, READRESPONSE);
    }

    ImGui::End();
    ImGui::PopStyleColor(1);
}

void EditorResponse() {
    ImVec2 screenSize = ImGui::GetIO().DisplaySize;
    ImGui::SetNextWindowPos(ImVec2(screenSize.x * 0.5f, screenSize.y - 200));
    ImGui::SetNextWindowSize(ImVec2(screenSize.x * 0.5f, screenSize.y));

    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImGui::GetStyleColorVec4(ImGuiCol_TitleBg));
    ImGui::Begin("Console", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

    if (READRESPONSE.size() != 0) {
        ImVec2 cursor = ImGui::GetCursorScreenPos();
        Table& table = READRESPONSE[0];
        for (int j = 1; j < table.getTableData().size(); j++) {
            string text = to_string(j);
            ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(cursor.x, cursor.y + 20*j), ImVec2(cursor.x + 20 - 1, cursor.y + 20*(j + 1) - 1), IM_COL32(30, 30, 30, 255));
            ImGui::GetWindowDrawList()->AddText(ImVec2(((text.length() == 1) ? 5 : 1) + cursor.x, cursor.y + 20*j), IM_COL32(255, 255, 255, 255), &(text[0]), &(text[text.size()]));
        }
        for (int j = 0; j < table.getTableData()[0].getRowData().size(); j++) {
            string text = table.getTableData()[0].getRowData()[j].getValue();
            ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(20 + cursor.x + 100*j, cursor.y), ImVec2(20 + cursor.x + 100*(j + 1) - 1, cursor.y + 20 - 1), IM_COL32(40, 50, 30, 255));
            ImGui::GetWindowDrawList()->AddText(ImVec2(20 + 5 + cursor.x + 100*j, cursor.y), IM_COL32(255, 255, 255, 255), &(text[0]), &(text[text.size()]));
        }
        for (int i = 1; i < table.getTableData().size(); i++) {
            for (int j = 0; j < table.getTableData()[i].getRowData().size(); j++) {
                string text = table.getTableData()[i].getRowData()[j].getValue();
                ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(20 + cursor.x + 100*j, cursor.y + 20*i), ImVec2(20 + cursor.x + 100*(j + 1) - 1, cursor.y + 20*(i + 1) - 1), IM_COL32(30, 30, 30, 255));
                if (ImGui::IsMouseHoveringRect(ImVec2(20 + cursor.x + 100*j, cursor.y + 20*i), ImVec2(20 + cursor.x + 100*(j + 1) - 1, cursor.y + 20*(i + 1) - 1))) {
                    ImGui::BeginTooltip();
                    ImGui::Text(&(table.getTableData()[i].getRowData()[j].getData()->getType()[0]));
                    ImGui::Text(&(("ROW " + to_string(i) + " " + "COL " + to_string(j + 1))[0]));
                    ImGui::EndTooltip();
                }
                ImGui::GetWindowDrawList()->AddText(ImVec2(20 + 5 + cursor.x + 100*j, cursor.y + 20*i), IM_COL32(255, 255, 255, 255), &(text[0]), &(text[text.size()]));
            }
        }
        ImGui::Dummy(ImVec2(0, screenSize.y - 200 + 20*(table.getTableData().size()))); // Dependency on Editor height here
    }

    ImGui::End();
    ImGui::PopStyleColor(1);
}

GLFWwindow* initWindow() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(1280, 720, "MyDB", NULL, NULL);
    glfwMakeContextCurrent(window);

    ImGui::CreateContext();
    ImGui::GetIO().Fonts->AddFontFromFileTTF("font.ttf", 18.0f);
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    return window;
}

void Render(GLFWwindow* window) {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        DatabaseWindow();
        EditorWindow();
        EditorResponse();

        ImGui::Render();
        
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.15f, 0.15f, 0.15f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }
}

int main(int, char**) {
    Create* create = new Create(db);
    Read* read = new Read(db);
    Update* update = new Update(db);
    Delete* del = new Delete(db);

    create->createTable("EMPLOYEES", vector<string> {"NAME:VARCHAR", "SURNAME:VARCHAR", "SALARY:NUMBER"});
    create->createTable("HR", vector<string> {"NAME:VARCHAR", "SURNAME:VARCHAR", "SALARY:NUMBER", "HEIGHT:VARCHAR"});
    update->addRow("EMPLOYEES", vector<string> {"John", "Doe", "300"});
    update->addRow("EMPLOYEES", vector<string> {"John", "Tyrell", "500"});
    update->addRow("EMPLOYEES", vector<string> {"Park", "Lee", "257"});
    update->addRow("EMPLOYEES", vector<string> {"Frederick", "Doe", "300"});
    update->addRow("EMPLOYEES", vector<string> {"Stevenson", "Stevens", "310"});
    update->addRow("EMPLOYEES", vector<string> {"Brian", "Walter", "410"});
    update->addRow("EMPLOYEES", vector<string> {"Trisha", "Goldomore", "600"});
    update->addRow("EMPLOYEES", vector<string> {"John", "Doe", "300"});
    update->addRow("EMPLOYEES", vector<string> {"Frederickson", "Gustavo", "300"});
    update->addRow("EMPLOYEES", vector<string> {"Summer", "Tyrell", "500"});
    update->addRow("EMPLOYEES", vector<string> {"Kim", "Lee", "257"});
    update->addRow("EMPLOYEES", vector<string> {"Frederick", "James", "300"});
    update->addRow("EMPLOYEES", vector<string> {"Jackson", "Stevens", "310"});
    update->addRow("EMPLOYEES", vector<string> {"Brian", "Walter", "410"});
    update->addRow("EMPLOYEES", vector<string> {"Cooper", "Goldomore", "600"});
    update->addRow("EMPLOYEES", vector<string> {"Britney", "Johnson", "300"});

    update->addRow("HR", vector<string> {"Sabrina", "Park", "520", "5'4"});
    update->addRow("HR", vector<string> {"Ashley", "Johnson", "600", "5'8"});
    update->addRow("HR", vector<string> {"Lia", "Xiu", "550", "5'6"});
    update->addRow("HR", vector<string> {"Brian", "Walter", "410", "6'1"});
    update->addRow("HR", vector<string> {"Trisha", "Goldomore", "600", "5'5"});
    update->addRow("HR", vector<string> {"John", "Doe", "300", "6'2"});
    update->addRow("HR", vector<string> {"John", "Tyrell", "500", "5'11"});
    update->addRow("HR", vector<string> {"Park", "Lee", "257", "5'7"});
    update->addRow("HR", vector<string> {"Frederick", "Doe", "300", "5'8"});
    update->addRow("HR", vector<string> {"Stevenson", "Stevens", "310", "6'3"});

    GLFWwindow* window = initWindow();
    Render(window);

    return 0;
}