#ifndef EVENT_H
#define EVENT_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <deque>
#include <queue>
#include <functional>

#include "debug.h"

using EventType = std::string;
using EventID = unsigned int;

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
        auto eventID_search = eventTypeToID.find(type);
        EventID eventID;
        if (eventID_search == eventTypeToID.end())
        {
            eventID = initID();
            eventTypeToID[type] = eventID;
        }
        else
        {
            eventID = eventID_search->second;
        }
        subscribers[id][eventID] = handler;
        eventSubscribers[eventID].push_back(id);
    }
    void unSubscribe(SubscriberID id, EventType type)
    {
        auto eventID_search = eventTypeToID.find(type);
        ASSERT(eventID_search != eventTypeToID.end(), "event type doesnt exist!");
        subscribers[id].erase(eventID_search->second);
    }
    template<typename T, typename... Args>
    void post_handle_immediate(Args&&... args)
    {
        T e(std::forward<Args>(args)...);
        EventType &type = e.type;
        auto eventID_search = eventTypeToID.find(type);
        ASSERT(eventID_search != eventTypeToID.end(), "event type doesnt exist!");
        EventID &eventID = eventID_search->second;
        auto subscriberList = eventSubscribers.find(eventID);
        if (subscriberList == eventSubscribers.end()) return;
        for (auto& sub: subscriberList->second)
        {
            auto eventCallback = subscribers[sub].find(eventID);
            if (eventCallback == subscribers[sub].end()) continue;
            eventCallback->second(e);
        }
    }
    template<typename T, typename... Args>
    void post_handle_queue(Args&&... args)
    {
        Event *e = new T(std::forward<Args>(args)...);
        eventQueue.push_back(e);
    }
    void dispatch()
    {
        for (auto e: eventQueue)
        {
            EventType &type = e->type;
            auto eventID_search = eventTypeToID.find(type);
            ASSERT(eventID_search != eventTypeToID.end(), "event type doesnt exist!");
            EventID &eventID = eventID_search->second;
            auto subscriberList = eventSubscribers.find(eventID);
            if (subscriberList == eventSubscribers.end()) continue;
            for (auto &sub: subscriberList->second)
            {
                auto eventCallback = subscribers[sub].find(eventID);
                if (eventCallback == subscribers[sub].end()) continue;
                eventCallback->second(*e);
            }
        }
    }

private:
    EventID initID()
    {
        static EventID s_id{};
        return s_id++;
    }

private:
    std::unordered_map<SubscriberID, std::unordered_map<EventID, std::function<void(const Event&)>>> subscribers;
    std::unordered_map<EventID, std::vector<SubscriberID>> eventSubscribers;

    std::unordered_map<EventType, EventID> eventTypeToID;

    std::deque<Event*> eventQueue;

    std::queue<SubscriberID> availableIDs;
    SubscriberID idhandedOut = 0;
};

#endif