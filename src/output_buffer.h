#ifndef _OUTPUT_BUFFER_H
#define _OUTPUT_BUFFER_H

#include <vector>
#include <queue>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/shared_ptr.hpp>

template <typename T>
class OutputBuffer {
public:
    void push(T t) {
        boost::lock_guard<boost::mutex> lock(this->mutex_);
        Q_.push(t);
    }
    void pushMany(std::vector<T> &t) {
        boost::lock_guard<boost::mutex> lock(this->mutex_);
        for (int i = 0; i < t.size(); ++i) {
            Q_.push(t[i]);
        }
    }
    T pop() {
        boost::unique_lock<boost::mutex> lock(this->mutex_);

        while (Q_.empty()) {
            this->cond_.wait(lock);
        }

        T t = Q_.front(); Q_.pop();
        return t;
    }
    void popMany(std::vector <T> &t, int maxSize) {
        boost::unique_lock<boost::mutex> lock(this->mutex_);

        while (Q_.empty()) {
            this->cond_.wait(lock);
        }

        t.clear();
        while (!Q_.empty() && t.size() < maxSize) {
            t.push_back(Q_.front()); Q_.pop();
        }
    }


private:
    std::queue <T> Q_;
    boost::mutex mutex_;
    boost::condition_variable cond_;
};

#endif
