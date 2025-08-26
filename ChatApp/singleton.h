#ifndef SINGLETON_H
#define SINGLETON_H

#include <iostream>
#include <memory>
#include <mutex>
#include <atomic>

template <typename T>
class Singleton
{
protected:
    Singleton() = default;
    Singleton(const Singleton<T>&) = delete;
    Singleton& operator=(const Singleton<T>&) = delete;

public:
    ~Singleton(){
        std::cout << "this is auto safe template destruct" << std::endl;
    }

    static std::shared_ptr<T> GetInstance()
    {
        // First check without locking (acquire semantics for thread-safety)
        if (_b_init.load(std::memory_order_acquire))
        {
            return instance;
        }

        // Locking to ensure only one instance is created (lazy initialization)
        s_mutex.lock();

        // Double-check to ensure instance is not created by another thread
        if (_b_init.load(std::memory_order_relaxed))
        {
            s_mutex.unlock();
            return instance;
        }

        instance = std::shared_ptr<T>(new T());
        _b_init.store(true, std::memory_order_release);
        s_mutex.unlock();

        return instance;
    }

    void PrintAddress() {
        std::cout << instance.get() << std::endl;
    }

private:
    static std::shared_ptr<T> instance;
    static std::mutex s_mutex;
    static std::atomic<bool> _b_init;
};

// Static member definitions
template <typename T>
std::shared_ptr<T> Singleton<T>::instance = nullptr;

template <typename T>
std::mutex Singleton<T>::s_mutex;

template <typename T>
std::atomic<bool> Singleton<T>::_b_init = false;

#endif // SINGLETON_H
