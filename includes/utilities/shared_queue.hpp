#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

/**
 * A shared queue is a queue that is thread safe.
 */
template <typename T>
class SharedQueue
{
    public:
        SharedQueue();
        ~SharedQueue();

        T& front();
        void pop();

        void push(const T& item);
        void push(T&& item);

        int size();
        bool empty();

    private:
        std::queue<T> queue;
        std::mutex mutex;
        std::condition_variable cond;
}; 

template <typename T>
SharedQueue<T>::SharedQueue() {}

template <typename T>
SharedQueue<T>::~SharedQueue() {}

template <typename T>
T& SharedQueue<T>::front()
{
    std::unique_lock<std::mutex> mlock(mutex);
    while (queue.empty())
    {
        cond.wait(mlock);
    }
    return queue.front();
}

template <typename T>
void SharedQueue<T>::pop()
{
    std::unique_lock<std::mutex> mlock(mutex);
    while (queue.empty())
    {
        cond.wait(mlock);
    }
    queue.pop();
}     

template <typename T>
void SharedQueue<T>::push(const T& item)
{
    std::unique_lock<std::mutex> mlock(mutex);
    queue.push(item);
    mlock.unlock();
    cond.notify_one();

}

template <typename T>
void SharedQueue<T>::push(T&& item)
{
    std::unique_lock<std::mutex> mlock(mutex);
    queue.push(std::move(item));
    mlock.unlock();
    cond.notify_one();

}

template <typename T>
int SharedQueue<T>::size()
{
    std::unique_lock<std::mutex> mlock(mutex);
    int size = queue.size();
    mlock.unlock();
    return size;
}

template <typename T>
bool SharedQueue<T>::empty()
{
    std::unique_lock<std::mutex> mlock(mutex);
    return queue.empty();
}