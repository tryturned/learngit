/*
 * @Author: taobo
 * @Date: 2020-11-05 16:58:09
 * @LastEditTime: 2020-11-06 17:10:16
 */
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <thread>
#include <vector>

using namespace std;

class ThreadDemo {
private:
  deque<int> m_data;
  mutex m_mtx;
  condition_variable m_cv;
  unsigned int m_N;
public:
  void ProduceThread() {
    while (true) {
      {
        unique_lock<mutex> lck(m_mtx);
        m_N = ++m_N % 1000;
        m_data.push_back(m_N);
      }
      m_cv.notify_all();
    }
  }

  void ConsumeThread() {
    while (true)
    {
      {
        unique_lock<mutex> lck(m_mtx);
        while(m_data.empty()) {
          m_cv.wait(lck);
        }
        m_data.pop_front();
      }
    }
  }
public:
  ThreadDemo() {
    m_data.clear();
    m_N = 0;
  }
  void start() {
    vector<thread> threads;
    for (int i = 0; i < 5; ++i) {
      threads.emplace_back(&ThreadDemo::ProduceThread, this);
    }
    for (int i = 0; i < 5; ++i) {
      threads.emplace_back(&ThreadDemo::ConsumeThread, this);
    }
    for (int i = 0; i < 10; ++i) {
      threads[i].join();
    }
  }
};



int main() {
  ThreadDemo objs;
  objs.start();
  return 0;
}