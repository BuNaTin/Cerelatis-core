#include "utils/ThreadPool.h"

namespace utils {

ThreadPool::ThreadPool(uint32_t num_threads) {
    threads.reserve(num_threads);
    for (uint32_t i = 0; i < num_threads; ++i) {
        threads.emplace_back(&ThreadPool::run, this);
    }
}

void ThreadPool::wait(int64_t task_id) {
    std::unique_lock<std::mutex> lock(completed_task_ids_mtx);
    completed_task_ids_cv.wait(lock, [this, task_id]() -> bool {
        return completed_task_ids.find(task_id) !=
               completed_task_ids.end();
    });
}

void ThreadPool::wait_all() {
    std::unique_lock<std::mutex> lock(task_q_mtx);
    completed_task_ids_cv.wait(lock, [this]() -> bool {
        std::lock_guard<std::mutex> task_lock(completed_task_ids_mtx);
        return task_queue.empty() &&
               last_idx == completed_task_ids.size();
    });
}

bool ThreadPool::calculated(int64_t task_id) {
    std::lock_guard<std::mutex> lock(completed_task_ids_mtx);
    if (completed_task_ids.find(task_id) != completed_task_ids.end()) {
        return true;
    }
    return false;
}

size_t ThreadPool::size() const noexcept { return threads.size(); }

ThreadPool::~ThreadPool() {
    quite = true;
    for (uint32_t i = 0; i < threads.size(); ++i) {
        task_q_cv.notify_all();
        threads[i].join();
    }
}

void ThreadPool::run() {
    while (!quite) {
        std::unique_lock<std::mutex> lock(task_q_mtx);
        task_q_cv.wait(lock, [this]() -> bool {
            return !task_queue.empty() || quite;
        });

        if (!task_queue.empty()) {
            auto elem = std::move(task_queue.front());
            task_queue.pop();
            lock.unlock();

            elem.first.get();

            std::lock_guard<std::mutex> lock(completed_task_ids_mtx);
            completed_task_ids.insert(elem.second);

            completed_task_ids_cv.notify_all();
        }
    }
}
} // namespace utils