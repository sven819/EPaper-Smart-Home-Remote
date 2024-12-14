#pragma once
#include <map>

template <typename T>
class PubSub
{
private:
    struct Subscriber
    {
        QueueHandle_t queue;
    };

    std::map<int, Subscriber> subscribers;
    int nextSubscriberId = 0;

public:
    PubSub() {}

    ~PubSub()
    {
    }

    int subscribe()
    {
        Subscriber newSubscriber;
        newSubscriber.queue = xQueueCreate(10, sizeof(T));

        int subscriberId = nextSubscriberId++;
        subscribers[subscriberId] = newSubscriber;
        return subscriberId;
    }

    void publish(const T &msg)
    {
        for (const auto &[id, subscriber] : subscribers)
        {
            xQueueSend(subscriber.queue, &msg, 0);
        }
    }

    bool receive(int subscriberId, T &msg, TickType_t wait = 0)
    {
        auto it = subscribers.find(subscriberId);
        if (it != subscribers.end())
        {
            if (xQueueReceive(it->second.queue, &msg, wait) == pdTRUE)
            {
                return true;
            }
        }
        return false;
    }
};
