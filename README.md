sigslot11
=========

signals and slots using c++11 magic

example
-------

```
#include "sigslot11.h"
#include <iostream>

class Button
{
public:
    void push()
    {
        std::cout << "The button was pushed!" << std::endl;
    }
};

class Window
{
public:
    sigslot11::Signal<void()> pushButton;
};

int main()
{
    Window window;
    Button button;
    
    window.pushButton.connect<Button, &Button::push>(&button);
    
    window.pushButton();
    
    return 0;
}
```
