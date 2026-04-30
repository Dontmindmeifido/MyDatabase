#pragma once
#include <vector>
#include <string>

class IObserver {
public:
    virtual void update() = 0;
};

class Observer {
    std::vector<IObserver*> subscriptions;

public:
    void subscribe(IObserver* observer);
    void notify();
};

class VersionControl: public IObserver {
    static VersionControl* instance;

    VersionControl() {};
public: 
    static VersionControl* get_instance();
    void update() override;
};