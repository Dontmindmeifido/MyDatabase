#include "05Interface.h"

void Interface::DatabaseWindow() {
    Database* db = Database::getInstance();

    ImVec2 screenSize = ImGui::GetIO().DisplaySize;

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(screenSize.x * 0.5f, screenSize.y));
    
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImGui::GetStyleColorVec4(ImGuiCol_TitleBg));
    ImGui::Begin("Database", NULL, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

    // Table names
    if (ImGui::BeginMenuBar()) {
        for (int i = 0; i < db->getDatabaseData().size(); i++) {
            if (ImGui::MenuItem(&(db->getDatabaseData()[i].getTableName()[0]))) {
                if (selectedTable != i) {
                    selectedTable = i;
                }
            }
        }
        ImGui::EndMenuBar();
    }

    // Table cells
    if (selectedTable != -1) {
        ImVec2 cursor = ImGui::GetCursorScreenPos();
        Table table = db->getDatabaseData()[selectedTable];
        ImGui::Dummy(ImVec2(0, 20 * (int(table.getTableData().size()))));
        int w = (ImGui::GetIO().DisplaySize.x * 0.5 - 38) / std::min(10, int(table.getTableData()[0].getRowData().size())); // Soft limit on 10 visible cols
        ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(cursor.x, cursor.y), ImVec2(cursor.x + 20 - 1, cursor.y + 20 - 1), IM_COL32(30, 30, 30, 255));
        for (int j = 1; j < table.getTableData().size(); j++) {
            std::string text = std::to_string(j);
            ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(cursor.x, cursor.y + 20*j), ImVec2(cursor.x + 20 - 1, cursor.y + 20*(j + 1) - 1), IM_COL32(30, 30, 30, 255));
            ImGui::GetWindowDrawList()->AddText(ImVec2(((text.length() == 1) ? 5 : 1) + cursor.x, cursor.y + 20*j), IM_COL32(255, 255, 255, 255), &(text[0]), &(text[text.size()]));
        }
        for (int j = 0; j < table.getTableData()[0].getRowData().size(); j++) {
            std::string text = table.getTableData()[0].getRowData()[j].getValue();
            ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(20 + cursor.x + w*j, cursor.y), ImVec2(20 + cursor.x + w*(j + 1) - 1, cursor.y + 20 - 1), IM_COL32(40, 50, 30, 255));
            ImGui::GetWindowDrawList()->AddText(ImVec2(20 + 5 + cursor.x + w*j, cursor.y), IM_COL32(255, 255, 255, 255), &(text[0]), &(text[text.size()]));
        }
        for (int i = 1; i < table.getTableData().size(); i++) {
            for (int j = 0; j < table.getTableData()[i].getRowData().size(); j++) {
                std::string text = table.getTableData()[i].getRowData()[j].getValue();
                ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(20 + cursor.x + w*j, cursor.y + 20*i), ImVec2(20 + cursor.x + w*(j + 1) - 1, cursor.y + 20*(i + 1) - 1), IM_COL32(30, 30, 30, 255));
                if (ImGui::IsMouseHoveringRect(ImVec2(20 + cursor.x + w*j, cursor.y + 20*i), ImVec2(20 + cursor.x + w*(j + 1) - 1, cursor.y + 20*(i + 1) - 1))) {
                    ImGui::BeginTooltip();
                    ImGui::Text(&((table.getTableData()[0].getRowData()[j].getValue() + ": " + table.getTableData()[i].getRowData()[j].getDataType())[0]));
                    ImGui::Text(&(("ROW " + std::to_string(i) + " " + "COL " + std::to_string(j + 1))[0]));
                    ImGui::EndTooltip();
                }
                ImGui::GetWindowDrawList()->AddText(ImVec2(20 + 5 + cursor.x + w*j, cursor.y + 20*i), IM_COL32(255, 255, 255, 255), &(text[0]), &(text[text.size()]));
            }
        }
    }

    ImGui::End();
    ImGui::PopStyleColor(1);
}

void Interface::EditorWindow(int buttonHeight) {
    Lexer* lexer = Lexer::getInstance();
    Database* db = Database::getInstance();
    Interpreter* interpreter = Interpreter::getInstance();

    ImVec2 screenSize = ImGui::GetIO().DisplaySize;
    ImGui::SetNextWindowPos(ImVec2(screenSize.x * 0.5f, 0));
    ImGui::SetNextWindowSize(ImVec2(screenSize.x * 0.5f, screenSize.y - 200));

    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImGui::GetStyleColorVec4(ImGuiCol_TitleBg));
    ImGui::Begin("Editor", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);


    // Syntax Highlight
    std::vector<std::string> wordList = lexer->getSpacedWords(queryBuffer);
    for (int i = 0; i < wordList.size(); i++) {
        if (lexer->retToLower(lexer->retStrip(wordList[i])) == "create") {
            ImGui::GetWindowDrawList()->AddText(ImVec2(ImGui::GetCursorScreenPos().x + 3, ImGui::GetCursorScreenPos().y + 3), IM_COL32(50, 150, 200, 255), &(wordList[i][0]));
        } else if (lexer->retToLower(lexer->retStrip(wordList[i])) == "addrow") {
            ImGui::GetWindowDrawList()->AddText(ImVec2(ImGui::GetCursorScreenPos().x + 3, ImGui::GetCursorScreenPos().y + 3), IM_COL32(60, 200, 160, 255), &(wordList[i][0]));
        } else if (lexer->retToLower(lexer->retStrip(wordList[i])) == "delrow") {
            ImGui::GetWindowDrawList()->AddText(ImVec2(ImGui::GetCursorScreenPos().x + 3, ImGui::GetCursorScreenPos().y + 3), IM_COL32(200, 100, 50, 255), &(wordList[i][0]));
        } else if (lexer->retToLower(lexer->retStrip(wordList[i])) == "read") {
            ImGui::GetWindowDrawList()->AddText(ImVec2(ImGui::GetCursorScreenPos().x + 3, ImGui::GetCursorScreenPos().y + 3), IM_COL32(200, 200, 220, 255), &(wordList[i][0]));
        } else if (lexer->retToLower(lexer->retStrip(wordList[i])) == "in") {
            ImGui::GetWindowDrawList()->AddText(ImVec2(ImGui::GetCursorScreenPos().x + 3, ImGui::GetCursorScreenPos().y + 3), IM_COL32(150, 100, 100, 255), &(wordList[i][0]));
        } else if (lexer->retToLower(lexer->retStrip(wordList[i])) == "where") {
            ImGui::GetWindowDrawList()->AddText(ImVec2(ImGui::GetCursorScreenPos().x + 3, ImGui::GetCursorScreenPos().y + 3), IM_COL32(150, 100, 150, 255), &(wordList[i][0]));
        } else if (lexer->retToLower(lexer->retStrip(wordList[i])) == "orderby") {
            ImGui::GetWindowDrawList()->AddText(ImVec2(ImGui::GetCursorScreenPos().x + 3, ImGui::GetCursorScreenPos().y + 3), IM_COL32(150, 100, 150, 255), &(wordList[i][0]));
        } else if (wordList[i] != "") {
            ImGui::GetWindowDrawList()->AddText(ImVec2(ImGui::GetCursorScreenPos().x + 3, ImGui::GetCursorScreenPos().y + 3), IM_COL32(255, 255, 255, 255), &(wordList[i][0]));
        }
    }

    // Name suggestions
    std::string lastWord = "";
    for (int i = (int(queryBuffer.size()) - 1); i >= 0; i--) {
        if (queryBuffer[i] != ' ' && queryBuffer[i] != '\n' && queryBuffer[i] != ';') {
            lastWord = queryBuffer[i] + lastWord;
        } else {
            break;
        }
    }

    ImVec2 cursorpos = ImVec2(lexer->getCursorPosition(queryBuffer).first, lexer->getCursorPosition(queryBuffer).second);
    std::vector<std::string> snippet = lexer->getSnippet(lastWord, *db);
    for (int i = 0; i < std::min(int(snippet.size()), 3); i++) {
        ImGui::GetWindowDrawList()->AddText(ImVec2(ImGui::GetCursorScreenPos().x + cursorpos.x + 15, ImGui::GetCursorScreenPos().y + 20 * i + 20 + cursorpos.y), IM_COL32(160, 200, 160, 255), &(snippet[i][0]));
    }

    // Text input field
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.8f, 0.0f)); // Transparent
    ImGui::InputTextMultiline(
        "##Editor", 
        &queryBuffer,
        ImVec2(-FLT_MIN, ImGui::GetWindowHeight() - 2.5 * buttonHeight), 
        ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_CtrlEnterForNewLine
    );
    ImGui::PopStyleColor(2);

    // Compile script button
    if (ImGui::Button("Compile Script", ImVec2(150, buttonHeight))) {
        std::cout << "\nExecuting DFA on:\n";
        std::cout << queryBuffer << "\n";
        std::cout << "---------" << "\n";

        for (auto table: *READRESPONSE) {
            delete table; // Memory leak solve in query when creating the return table (sepcifically the new from 01Crud in readTable)
        }
        
        READRESPONSE->clear();
        
        interpreter->runInterpreter(queryBuffer, READRESPONSE);
    }

    ImGui::End();
    ImGui::PopStyleColor(1);
}

void Interface::EditorResponse() {
    ImVec2 screenSize = ImGui::GetIO().DisplaySize;
    ImGui::SetNextWindowPos(ImVec2(screenSize.x * 0.5f, screenSize.y - 200));
    ImGui::SetNextWindowSize(ImVec2(screenSize.x * 0.5f, screenSize.y));

    // Console with read response
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImGui::GetStyleColorVec4(ImGuiCol_TitleBg));
    ImGui::Begin("Console", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

    if (READRESPONSE->size() != 0) {
        ImVec2 cursor = ImGui::GetCursorScreenPos();
        Table& table = *((*READRESPONSE)[0]);
        int w = (ImGui::GetIO().DisplaySize.x * 0.5 - 38) / std::min(10, int(table.getTableData()[0].getRowData().size())); // Soft limit on 10 visible cols
        ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(cursor.x, cursor.y), ImVec2(cursor.x + 20 - 1, cursor.y + 20 - 1), IM_COL32(30, 30, 30, 255));
        for (int j = 1; j < table.getTableData().size(); j++) {
            std::string text = std::to_string(j);
            ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(cursor.x, cursor.y + 20*j), ImVec2(cursor.x + 20 - 1, cursor.y + 20*(j + 1) - 1), IM_COL32(30, 30, 30, 255));
            ImGui::GetWindowDrawList()->AddText(ImVec2(((text.length() == 1) ? 5 : 1) + cursor.x, cursor.y + 20*j), IM_COL32(255, 255, 255, 255), &(text[0]), &(text[text.size()]));
        }
        for (int j = 0; j < table.getTableData()[0].getRowData().size(); j++) {
            std::string text = table.getTableData()[0].getRowData()[j].getValue();
            ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(20 + cursor.x + w*j, cursor.y), ImVec2(20 + cursor.x + w*(j + 1) - 1, cursor.y + 20 - 1), IM_COL32(40, 50, 30, 255));
            ImGui::GetWindowDrawList()->AddText(ImVec2(20 + 5 + cursor.x + w*j, cursor.y), IM_COL32(255, 255, 255, 255), &(text[0]), &(text[text.size()]));
        }
        for (int i = 1; i < table.getTableData().size(); i++) {
            for (int j = 0; j < table.getTableData()[i].getRowData().size(); j++) {
                std::string text = table.getTableData()[i].getRowData()[j].getValue();
                ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(20 + cursor.x + w*j, cursor.y + 20*i), ImVec2(20 + cursor.x + w*(j + 1) - 1, cursor.y + 20*(i + 1) - 1), IM_COL32(30, 30, 30, 255));
                if (ImGui::IsMouseHoveringRect(ImVec2(20 + cursor.x + w*j, cursor.y + 20*i), ImVec2(20 + cursor.x + w*(j + 1) - 1, cursor.y + 20*(i + 1) - 1))) {
                    ImGui::BeginTooltip();
                    ImGui::Text(&((table.getTableData()[0].getRowData()[j].getValue() + ": " + table.getTableData()[i].getRowData()[j].getDataType())[0]));
                    ImGui::Text(&(("ROW " + std::to_string(i) + " " + "COL " + std::to_string(j + 1))[0]));
                    ImGui::EndTooltip();
                }
                ImGui::GetWindowDrawList()->AddText(ImVec2(20 + 5 + cursor.x + w*j, cursor.y + 20*i), IM_COL32(255, 255, 255, 255), &(text[0]), &(text[text.size()]));
            }
        }
        ImGui::Dummy(ImVec2(0, screenSize.y - 200 + 20*(table.getTableData().size()))); // Dependency on Editor height here
    }

    ImGui::End();
    ImGui::PopStyleColor(1);
}

Interface::Interface() {
    this->READRESPONSE = new std::vector<Table*>;
}

Interface* Interface::getInstance() {
    if (!instance) instance = new Interface();
    return instance;
}

GLFWwindow* Interface::initWindow() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(1280, 720, "MyDB", NULL, NULL);
    glfwMakeContextCurrent(window);

    ImGui::CreateContext();
    ImGui::GetIO().Fonts->AddFontFromFileTTF("fonts/font.ttf", 18.0f);
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    return window;
}

void Interface::Render(GLFWwindow* window) {
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

Interface* Interface::instance = nullptr;