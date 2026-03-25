#include "05Interface.h"

int main() {
    Crud* crud = Crud::getInstance();

    crud->createTable("EMPLOYEES", std::vector<std::string> {"NAME", "SURNAME", "SALARY"}, std::vector<std::string> {"VARCHAR", "VARCHAR", "NUMBER"});
    crud->createTable("HR", std::vector<std::string> {"NAME", "SURNAME", "SALARY", "HEIGHT"}, std::vector<std::string> {"VARCHAR", "VARCHAR", "NUMBER", "VARCHAR"});
    crud->addRow("EMPLOYEES", std::vector<std::string> {"John", "Doe", "300"});
    crud->addRow("EMPLOYEES", std::vector<std::string> {"John", "Tyrell", "500"});
    crud->addRow("EMPLOYEES", std::vector<std::string> {"Park", "Lee", "257"});
    crud->addRow("EMPLOYEES", std::vector<std::string> {"Frederick", "Doe", "300"});
    crud->addRow("EMPLOYEES", std::vector<std::string> {"Stevenson", "Stevens", "310"});
    crud->addRow("EMPLOYEES", std::vector<std::string> {"Brian", "Walter", "410"});
    crud->addRow("EMPLOYEES", std::vector<std::string> {"Trisha", "Goldomore", "600"});
    crud->addRow("EMPLOYEES", std::vector<std::string> {"John", "Doe", "300"});
    crud->addRow("EMPLOYEES", std::vector<std::string> {"Frederickson", "Gustavo", "300"});
    crud->addRow("EMPLOYEES", std::vector<std::string> {"Summer", "Tyrell", "500"});
    crud->addRow("EMPLOYEES", std::vector<std::string> {"Kim", "Lee", "257"});
    crud->addRow("EMPLOYEES", std::vector<std::string> {"Frederick", "James", "300"});
    crud->addRow("EMPLOYEES", std::vector<std::string> {"Jackson", "Stevens", "310"});
    crud->addRow("EMPLOYEES", std::vector<std::string> {"Brian", "Walter", "410"});
    crud->addRow("EMPLOYEES", std::vector<std::string> {"Cooper", "Goldomore", "600"});
    crud->addRow("EMPLOYEES", std::vector<std::string> {"Britney", "Johnson", "300"});

    crud->addRow("HR", std::vector<std::string> {"Sabrina", "Park", "520", "5'4"});
    crud->addRow("HR", std::vector<std::string> {"Ashley", "Johnson", "600", "5'8"});
    crud->addRow("HR", std::vector<std::string> {"Lia", "Xiu", "550", "5'6"});
    crud->addRow("HR", std::vector<std::string> {"Brian", "Walter", "410", "6'1"});
    crud->addRow("HR", std::vector<std::string> {"Trisha", "Goldomore", "600", "5'5"});
    crud->addRow("HR", std::vector<std::string> {"John", "Doe", "300", "6'2"});
    crud->addRow("HR", std::vector<std::string> {"John", "Tyrell", "500", "5'11"});
    crud->addRow("HR", std::vector<std::string> {"Park", "Lee", "257", "5'7"});
    crud->addRow("HR", std::vector<std::string> {"Frederick", "Doe", "300", "5'8"});
    crud->addRow("HR", std::vector<std::string> {"Stevenson", "Stevens", "310", "6'3"});

    Interface* interface = Interface::getInstance();
    GLFWwindow* window = interface->initWindow();
    interface->Render(window);

    return 0;
}