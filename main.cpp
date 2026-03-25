#include "05Interface.cpp"

int main() {
    Lexer* lexer = Lexer::getInstance();
    Compiler* compiler = Compiler::getInstance();
    Database* db = Database::getInstance();
    Crud* crud = Crud::getInstance();

    crud->createTable("EMPLOYEES", vector<string> {"NAME:VARCHAR", "SURNAME:VARCHAR", "SALARY:NUMBER"});
    crud->createTable("HR", vector<string> {"NAME:VARCHAR", "SURNAME:VARCHAR", "SALARY:NUMBER", "HEIGHT:VARCHAR"});
    crud->addRow("EMPLOYEES", vector<string> {"John", "Doe", "300"});
    crud->addRow("EMPLOYEES", vector<string> {"John", "Tyrell", "500"});
    crud->addRow("EMPLOYEES", vector<string> {"Park", "Lee", "257"});
    crud->addRow("EMPLOYEES", vector<string> {"Frederick", "Doe", "300"});
    crud->addRow("EMPLOYEES", vector<string> {"Stevenson", "Stevens", "310"});
    crud->addRow("EMPLOYEES", vector<string> {"Brian", "Walter", "410"});
    crud->addRow("EMPLOYEES", vector<string> {"Trisha", "Goldomore", "600"});
    crud->addRow("EMPLOYEES", vector<string> {"John", "Doe", "300"});
    crud->addRow("EMPLOYEES", vector<string> {"Frederickson", "Gustavo", "300"});
    crud->addRow("EMPLOYEES", vector<string> {"Summer", "Tyrell", "500"});
    crud->addRow("EMPLOYEES", vector<string> {"Kim", "Lee", "257"});
    crud->addRow("EMPLOYEES", vector<string> {"Frederick", "James", "300"});
    crud->addRow("EMPLOYEES", vector<string> {"Jackson", "Stevens", "310"});
    crud->addRow("EMPLOYEES", vector<string> {"Brian", "Walter", "410"});
    crud->addRow("EMPLOYEES", vector<string> {"Cooper", "Goldomore", "600"});
    crud->addRow("EMPLOYEES", vector<string> {"Britney", "Johnson", "300"});

    crud->addRow("HR", vector<string> {"Sabrina", "Park", "520", "5'4"});
    crud->addRow("HR", vector<string> {"Ashley", "Johnson", "600", "5'8"});
    crud->addRow("HR", vector<string> {"Lia", "Xiu", "550", "5'6"});
    crud->addRow("HR", vector<string> {"Brian", "Walter", "410", "6'1"});
    crud->addRow("HR", vector<string> {"Trisha", "Goldomore", "600", "5'5"});
    crud->addRow("HR", vector<string> {"John", "Doe", "300", "6'2"});
    crud->addRow("HR", vector<string> {"John", "Tyrell", "500", "5'11"});
    crud->addRow("HR", vector<string> {"Park", "Lee", "257", "5'7"});
    crud->addRow("HR", vector<string> {"Frederick", "Doe", "300", "5'8"});
    crud->addRow("HR", vector<string> {"Stevenson", "Stevens", "310", "6'3"});

    Interface* interface = Interface::getInstance(db, lexer, compiler);
    GLFWwindow* window = interface->initWindow();
    interface->Render(window);

    return 0;
}