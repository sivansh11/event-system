#include "event.h"

#include <iostream>

// event definition.  
class WindowResizeEvent : public Event
{
public:
    WindowResizeEvent(int new_width, int new_height) : Event("WINDOW::RESIZE"), new_width(new_width), new_height(new_height) {}
    int new_width, new_height;
};

int main()
{
    Dispatcher dispatcher;
    int window_width = 500, window_height = 500;
    auto windowEvents = dispatcher.createSubscriber();
    dispatcher.subscribe(windowEvents, "WINDOW::RESIZE", [&](const Event& e)
    {
        WindowResizeEvent &event = (WindowResizeEvent&)(e);
        window_width = event.new_width;
        window_height = event.new_height;
    });

    dispatcher.post(WindowResizeEvent(600, 400));

    // dispatcher.unSubscribe(windowEvents, "WINDOW::RESIZE");

    dispatcher.deleteSubscriber(windowEvents);

    dispatcher.dispatch();

    std::cout << window_width << ' ' << window_height << '\n';


    return 0;
}