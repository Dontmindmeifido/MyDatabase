#include "version_control.h"
#include "../database/database.h"
#include "manager.h"

void Observer::subscribe(IObserver* observer) {
    subscriptions.push_back(observer);
}

void Observer::notify(std::string database_file, std::string undo_key) {
    for (IObserver* observer: subscriptions) {
        observer->update(database_file, undo_key);
    }
}

VersionControl* VersionControl::get_instance() {
    if (instance != nullptr) return instance;

    instance = new VersionControl();
    return instance;
}

void VersionControl::update(std::string database_file, std::string undo_key) {
    Manager::get_instance()->save_database(Database::get_instance(), database_file, undo_key);
}

VersionControl* VersionControl::instance = nullptr;

