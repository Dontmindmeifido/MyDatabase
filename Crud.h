#pragma once
#include "Create.h"
#include "Read.h"
#include "Update.h"
#include "Delete.h"

class Crud: public Create, public Read, public Update, public Delete {
    static Crud* instance;

    Crud();

public:
    static Crud* getInstance();
};
