/** ******************************************************************************
 * @file    ThreadSafeQueue.hpp
 * @company Orbit Research
 * @version v1.0
 * @date    08-12-25
 * @brief   Thread-safe queue template header.
 *
 * @details
 * Provides a simple `ThreadSafeQueue<T>` wrapper around `std::queue` with
 * mutex protection for concurrent push/pop operations. Intended for use by
 * Graphiti event/response queueing within the library.
 *
 ******************************************************************************
 * @attention
 * Copyright (c) 2026 Orbit Research LLC
 * Distributed under GNU General Public License V3.0
 *******************************************************************************/

#ifndef THREAD_SAFE_QUEUE_H
#define THREAD_SAFE_QUEUE_H

#include <queue>
#include <mutex>

/**
 * @class ThreadSafeQueue
 * @brief Thread-safe FIFO queue implementation
 *
 * Provides mutual-exclusion–protected access to an
 * underlying std::queue for concurrent environments.
 *
 * @tparam T Type of elements stored in the queue
 */
template<typename T>
class ThreadSafeQueue {
private:
    /** Internal FIFO queue */
    std::queue<T> queue_;

    /** Mutex protecting queue access */
    std::mutex mutex_;

public:
    /**
     * @function ThreadSafeQueue
     * @brief Default constructor
     */
    ThreadSafeQueue() = default;

    /**
     * @function ~ThreadSafeQueue
     * @brief Default destructor
     * @return void
     */
    ~ThreadSafeQueue() = default;

    /**
     * @function ThreadSafeQueue
     * @brief Copy constructor (deleted)
     * @param other Source queue
     */
    ThreadSafeQueue(const ThreadSafeQueue& other) = delete;

    /**
     * @function operator=
     * @brief Copy assignment operator (deleted)
     * @param other Source queue
     * @return ThreadSafeQueue reference
     */
    ThreadSafeQueue& operator=(const ThreadSafeQueue& other) = delete;

    /**
     * @function push
     * @brief Pushes an element into the queue
     * @param value Element to be inserted
     * @return void
     */
    void push(const T& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(value);
    }

    /**
     * @function pop
     * @brief Pops the front element from the queue
     * @param value Reference to store the popped element
     * @return true if an element was popped, false if the queue was empty
     */
    bool pop(T& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) return false;
        value = queue_.front();
        queue_.pop();
        return true;
    }

    /**
     * @function empty
     * @brief Checks whether the queue is empty
     * @return true if the queue is empty, false otherwise
     */
    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }

    /**
     * @function size
     * @brief @return the number of elements in the queue
     * @return Number of elements currently stored
     */
    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }
};

#endif // THREAD_SAFE_QUEUE_H

