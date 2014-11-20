#include "sigslot11.hpp"
#include <iostream>

class Light {
public:
    void toggle() {
        is_lit = !is_lit;
    }

    void print() {
        std::cout << "The light is " << (is_lit ? "on" : "off") << std::endl;
    }

private:
    bool is_lit = false;
};

class Switch {
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
    s.click.disconnect_all();
    s.click();
    l.print();

    return 0;
}
