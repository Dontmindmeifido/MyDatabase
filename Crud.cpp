#include "Crud.h"

Crud::Crud() {
    
}

Crud* Crud::getInstance() {
    if (!instance) instance = new Crud();
    return instance;
}

Crud* Crud::instance = nullptr;

