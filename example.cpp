#include "sigslot11.h"
#include <iostream>

class Light
{
public:
    Light()
    {
        id = ++UID;
    }

    void toggleState()
    {
        isLit = !isLit;
    }
    
    void turnOn()
    {
        isLit = true;
    }
    
    void turnOff()
    {
        isLit = false;
    }
    
    void print()
    {
        std::cout << "The light #" << id << " is " << (isLit ? "on" : "off") << std::endl;
    }
   
private:
    bool isLit = false;
    int id;
    static int UID;
};

int Light::UID = 0;

class Switch
{
public:
    sigslot11::Signal<void()> click;
};

int main()
{
    Light l;
    Switch s;
    s.click.connect<Light, &Light::turnOn>(&l);
    
    l.print();
    s.click();
    l.print();
    s.click.disconnect();
    s.click();
    l.print();
    
    return 0;
}
