sigslot11
=========

signals and slots using c++11 magic

example
-------

```c++
#include "sigslot11.h"
#include <iostream>

class Button {
public:
    void push() {
        std::cout << "The button was pushed!" << std::endl;
    }
};

class Window {
public:
    sigslot11::Signal<void()> push_button;
};

int main()
{
    Window window;
    Button button;

    window.push_button.connect<Button, &Button::push>(&button);
    
    window.push_button();

    return 0;
}
```
