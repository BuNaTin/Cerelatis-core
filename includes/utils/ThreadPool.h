#pragma once

#include <atomic>
#include <chrono>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <unordered_set>

namespace utils {

class ThreadPool {
    // interface
public:
    template<typename Func, typename... Args>
    int64_t addTask(const Func &task_func, Args &&...args);

    void wait(int64_t task_id);
    void wait_all();

    bool calculated(int64_t task_id);
    size_t size() const noexcept;

    // constructors
public:
    ThreadPool(uint32_t num_threads);
    ~ThreadPool();

private:
    void run();

    // data
private:
    std::queue<std::pair<std::future<void>, int64_t>> task_queue;
    std::mutex task_q_mtx;
    std::condition_variable task_q_cv;

    std::unordered_set<int64_t> completed_task_ids;
    std::condition_variable completed_task_ids_cv;
    std::mutex completed_task_ids_mtx;

    std::vector<std::thread> threads;

    std::atomic<bool> quite{false};
    std::atomic<int64_t> last_idx;
};

template<typename Func, typename... Args>
int64_t ThreadPool::addTask(const Func &task_func, Args &&...args) {
    int64_t task_idx = last_idx++;

    std::lock_guard<std::mutex> q_lock(task_q_mtx);
    task_queue.emplace(
            std::async(std::launch::deferred, task_func, args...),
            task_idx);
    task_q_cv.notify_one();
    return task_idx;
}

} // namespace utils