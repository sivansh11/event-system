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
    Dispatcher dispatcher;  // manages all the events and subscibers 

    int window_width = 500, window_height = 500;  // demo data 

    auto windowEvents = dispatcher.createSubscriber();  // creats a new handle/subscriber
    dispatcher.subscribe(windowEvents, "WINDOW::RESIZE", [&](const Event& e)
    {
        WindowResizeEvent &event = (WindowResizeEvent&)(e);
        window_width = event.new_width;
        window_height = event.new_height;
    });  // a subscriber subscribes to an event and gives a callback 

    std::cout << window_width << ' ' << window_height << '\n';

    // dispatcher.unSubscribe(windowEvents, "WINDOW::RESIZE");  // unsubscribes subscriber to an event

    // dispatcher.deleteSubscriber(windowEvents);  // removes the subscriber

    dispatcher.post<WindowResizeEvent>(600, 400);  // posts event to an event queue, takes in templated Event and its args as the parameters

    std::cout << window_width << ' ' << window_height << '\n';

    return 0;
}