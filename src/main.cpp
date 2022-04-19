#include "event.h"

#include <iostream>

class AddEvent : public Event
{
public:
    AddEvent(int i1, int j2) : Event("ADD")
    {
        i = i1;
        j = j2;
    }
    int i, j;
};

int main()
{
    Dispatcher dispatcher;
    auto adder = dispatcher.createSubscriber();
    int i = 7;
    dispatcher.subscribe(adder, "ADD", [&](const Event& e)
    {
        AddEvent &event = (AddEvent&)e;
        std::cout << event.i + event.j + i << '\n';

    });
    AddEvent e(1, 1);
    dispatcher.post(e);

    dispatcher.dispatch();

    return 0;
}