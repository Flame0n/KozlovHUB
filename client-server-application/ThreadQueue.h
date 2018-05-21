#ifndef THREADQUEUE_H
#define THREADQUEUE_H

#include <future> //threads v cpp
#include <queue>
//#include <vector>

using namespace std;

class ThreadQueue
{
    public:
        ThreadQueue(size_t threadsCount);
        template<typename T, typename... Args> auto addThread(T&& t, Args&&... args) -> future<typename result_of<T(Args...)>::type>;
        virtual ~ThreadQueue()
        {
            done.store(1);
            cond.notify_all();
            for(thread& thread : threads)
            thread.join();
        };
    protected:
    private:
        queue<function<void()>> tasks{}; //массив функций потоков
        vector<thread> threads{}; //массив потоков
        mutex mtx{};
        condition_variable cond{};
        atomic<bool> done{};
};

template<typename T, typename... Args> auto ThreadQueue::addThread(T&& t, Args&&... args) -> future<typename std::result_of<T(Args...)>::type>
{
    typedef typename result_of<T(Args...)>::type ResultType;
    auto task = make_shared<packaged_task<ResultType()>>
        ([&t, &args...](){return t(args...);});
    auto result = task->get_future();
    {
        unique_lock<mutex> lock(mtx);
        if(done.load())
            throw runtime_error("Queue is full");
        tasks.emplace([task](){ (*task)();});
    }
    cond.notify_one();
    return result;
}

ThreadQueue::ThreadQueue(size_t threadsCount)
{
    done.store(false);
    for(size_t i = 0; i < threadsCount; i++)
    {
        threads.emplace_back([this]() {
            while(1)
            {
                function<void()> task;
                {
                    unique_lock<mutex> lock(mtx);
                    cond.wait(lock, [this](){ return !tasks.empty() || done;});
                    if(done && tasks.empty())
                        return;
                    task = move(tasks.front());
                    tasks.pop();
                }
                task();
            }
        });
    }
}


#endif // THREADQUEUE_H
