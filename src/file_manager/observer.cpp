#include "observer.h"
#include "../database/database.h"
#include "manager.h"

void Observer::subscribe(IObserver* observer) {
    subscriptions.push_back(observer);
}

void Observer::notify() {
    for (IObserver* observer: subscriptions) {
        observer->update();
    }
}

VersionControl* VersionControl::get_instance() {
    if (instance != nullptr) return instance;

    instance = new VersionControl();
    return instance;
}

void VersionControl::update() {
    Manager::get_instance()->save_database(Database::get_instance(), ".version.db", "password");
}

VersionControl* VersionControl::instance = nullptr;

