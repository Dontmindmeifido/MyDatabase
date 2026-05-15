#pragma once
#include <vector>
#include <string>

class IObserver {
public:
    virtual void update(std::string database_file, std::string undo_key) = 0;
};

class Observer {
    std::vector<IObserver*> subscriptions;

public:
    void subscribe(IObserver* observer);
    void notify(std::string database_file, std::string undo_key);
};

class VersionControl: public IObserver {
    static VersionControl* instance;

    VersionControl() {};
    
public: 
    static VersionControl* get_instance();
    void update(std::string database_file, std::string undo_key) override;
};