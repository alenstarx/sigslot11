#include "sigslot11.h"
#include <iostream>

class Light
{
public:
    void toggle()
    {
        isLit = !isLit;
    }
    
    void print()
    {
        std::cout << "The light is " << (isLit ? "on" : "off") << std::endl;
    }
   
private:
    bool isLit = false;
};

class Switch
{
public:
    sigslot11::Signal<void()> click;
};

int main()
{
    Light l;
    Switch s;
    s.click.connect<Light, &Light::toggle>(&l);
    
    l.print();
    s.click();
    l.print();
    s.click.disconnectAll();
    s.click();
    l.print();
    
    return 0;
}
