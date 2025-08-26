#pragma once
#include <functional>
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>

class TaskThreadPool {
public:
	explicit TaskThreadPool(size_t threadCount = std::thread::hardware_concurrency());
	~TaskThreadPool();

	void submit(std::function<void()> task);
	void stop();

private:
	void workerLoop();

	std::vector<std::thread> _workers;
	std::queue<std::function<void()>> _tasks;
	std::mutex _mtx;
	std::condition_variable _cv;
	std::atomic<bool> _stopping{ false };
}; 