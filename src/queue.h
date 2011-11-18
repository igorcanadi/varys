#ifndef _QUEUE_H
#define _QUEUE_H

#include <vector>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/shared_ptr.hpp>
#include <list>

template <typename T>
class Queue {
public:
    void push(T t) {
        boost::lock_guard<boost::mutex> lock(this->mutex_);
        Q_.push_back(t);
        this->cond_.notify_all();
    }
    void pushMany(std::vector<T> &t) {
        boost::lock_guard<boost::mutex> lock(this->mutex_);
        for (int i = 0; i < t.size(); ++i) {
            Q_.push_back(t[i]);
        }
        this->cond_.notify_all();
    }
    T pop() {
        boost::unique_lock<boost::mutex> lock(this->mutex_);

        while (Q_.empty()) {
            this->cond_.wait(lock);
        }

        T t = Q_.front(); Q_.pop_front();
        return t;
    }
    // if maxSize == 1, pop all
    void popMany(std::vector <T> &t, int maxSize, bool block = true) {
        boost::unique_lock<boost::mutex> lock(this->mutex_);

        if (block) {
            while (Q_.empty()) {
                this->cond_.wait(lock);
            }
        }

        t.clear();
        while (!Q_.empty() && (maxSize == -1 || t.size() < maxSize)) {
            t.push_back(Q_.front()); Q_.pop_front();
        }
    }

    bool empty() {
        boost::lock_guard<boost::mutex> lock(this->mutex_);
        return Q_.empty();
    }

private:
    // TODO change this to queue
    std::list <T> Q_;
    boost::mutex mutex_;
    boost::condition_variable cond_;
};

#endif
