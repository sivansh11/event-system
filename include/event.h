#ifndef EVENT_H
#define EVENT_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <deque>
#include <queue>
#include <functional>

using EventType = std::string;

class Event  // base event class that all events will inherit from
{
public:
    Event(const char *type) : type(type) {}
    EventType type;
private:

};

using SubscriberID = long long;

class Dispatcher
{
    struct EventData
    {
        EventType type;
        std::function<void(const Event&)> &callBack;
    };
public:
    SubscriberID createSubscriber()
    {
        if (availableIDs.size() == 0)
        {
            return idhandedOut++;
        }
        SubscriberID id = availableIDs.front();
        availableIDs.pop();
        return id;
    }
    void deleteSubscriber(SubscriberID id)
    {
        subscribers[id].clear();
        subscribers.erase(id);
        availableIDs.push(id);
    }
    void subscribe(SubscriberID id, EventType type, std::function<void(const Event&)> &&handler)
    {
        subscribers[id][type] = handler;
        eventSubscribers[type].push_back(id);
    }
    void unSubscribe(SubscriberID id, EventType type)
    {
        subscribers[id].erase(type);
    }
    void post(Event &&e)
    {
        eventQueue.push_back(&e);
    }
    void dispatch()
    {
        for (auto& e: eventQueue)
        {
            EventType &type = e->type;
            auto subscriberList = eventSubscribers.find(type);
            if (subscriberList == eventSubscribers.end()) continue;
            for (auto& sub: subscriberList->second)
            {
                auto eventCallback = subscribers[sub].find(type);
                if (eventCallback == subscribers[sub].end()) continue;
                eventCallback->second(*e);
            }
        }
    }
private:
    std::unordered_map<SubscriberID, std::unordered_map<EventType, std::function<void(const Event&)>>> subscribers;
    std::unordered_map<EventType, std::vector<SubscriberID>> eventSubscribers;

    std::deque<Event*> eventQueue;

    std::queue<SubscriberID> availableIDs;
    SubscriberID idhandedOut = 0;
};

#endif