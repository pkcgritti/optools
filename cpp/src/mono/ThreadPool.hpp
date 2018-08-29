#pragma once

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

namespace mono {
    using std::vector;
    using std::queue;
    class ThreadPool {
        static ThreadPool* instance;
        vector<std::thread> workers;
        queue<std::function<void()>> tasks;
        std::mutex mutex;
        std::condition_variable condition;
        bool stop;

        ThreadPool(size_t nThreads);
        ~ThreadPool();
    public:
        size_t size() const { return workers.size(); }
        static ThreadPool& getInstance();
        static void shutdown();
        template <class F, class... Args>
        auto enqueue(F&& f, Args&&... args)
            -> std::future<typename std::result_of<F(Args...)>::type>;
    };

    ThreadPool* ThreadPool::instance = 0;

    ThreadPool& ThreadPool::getInstance() {
        if (ThreadPool::instance == 0) {
            ThreadPool::instance = new ThreadPool(2);
        }
        return *ThreadPool::instance;
    }

    void ThreadPool::shutdown() {
        if (ThreadPool::instance != 0) {
            delete ThreadPool::instance;
            ThreadPool::instance = 0;
        }
    }

    inline ThreadPool::ThreadPool(size_t nThreads) : stop(false) {
        for (size_t i = 0; i < nThreads; i++) {
            workers.emplace_back(
                [this]
                {
                    for(;;) {
                        std::function<void()> task;

                        {
                            std::unique_lock<std::mutex> lock(this->mutex);
                            this->condition.wait(lock, 
                                [this] { return this->stop || !this->tasks.empty(); });
                            if (this->stop && this->tasks.empty())
                                return;
                            task = std::move(this->tasks.front());
                            this->tasks.pop();
                        }
                        task();
                    }
                }
            );
        }
    }

    template <class F, class... Args>
    auto ThreadPool::enqueue(F&& f, Args&&... args)
        -> std::future<typename std::result_of<F(Args...)>::type> {
        using return_type = typename std::result_of<F(Args...)>::type;
        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(mutex);
            if (stop) throw std::runtime_error("Enqueue on stopped threadpool");
            tasks.emplace([task](){ (*task)(); });
        }
        condition.notify_one();
        return res;
    }

    inline ThreadPool::~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(mutex);
            stop = true;
        }
        condition.notify_all();
        for (std::thread& worker: workers)
            worker.join();
    }
};

