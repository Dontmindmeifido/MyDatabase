#include "Connection.h"

Connection::Connection() {
    this->database = Database::getInstance();
}

Connection::~Connection() {
    delete database;
}

const Database* Connection::getConnection() {
    return this->database;
}