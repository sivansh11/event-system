# event-system

## in linux
cmake -S. -Bbuild 

### to run
cd build
make
./app



### How to use this library?
This library is based on event subscriber paradigm.
So the flow is, you create a subscriber(analogus to a user), the subscriber subscribes to an event and gives the dispatcher a callback.
When a event is posted to the dispatcher, the dispatcher looks at the subscribers that are subscribed to that particular event and calls their callbacks.

There are 2 ways of posting an event, post_handle_queue() and post_handle_immediate()
post_handle_immediate() immidiately calls all the respective callbacks, while post_handle_queue() requires a dispatch() call to go thru the queue of events

do look at example/main.cpp for further clarification 
