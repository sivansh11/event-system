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

class Event
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
    void post(Event &e)
    {
        eventQueue.push_back(&e);
    }
    void dispatch()
    {
        for (auto& e: eventQueue)
        {
            EventType &type = e->type;
            for (auto& kv: eventSubscribers)
            {
                auto& subs = kv.second;
                for (auto& sub: subs)
                {
                    subscribers[sub][type](*e);
                }
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