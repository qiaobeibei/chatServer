#include "TaskThreadPool.h"

TaskThreadPool::TaskThreadPool(size_t threadCount)
{
	if (threadCount == 0) threadCount = 1;
	_workers.reserve(threadCount);
	for (size_t i = 0; i < threadCount; ++i) {
		_workers.emplace_back([this]() { workerLoop(); });
	}
}

TaskThreadPool::~TaskThreadPool()
{
	stop();
	for (auto &t : _workers) {
		if (t.joinable()) t.join();
	}
}

void TaskThreadPool::submit(std::function<void()> task)
{
	if (_stopping) return;
	{
		std::lock_guard<std::mutex> lock(_mtx);
		_tasks.push(std::move(task));
	}
	_cv.notify_one();
}

void TaskThreadPool::stop()
{
	bool expected = false;
	if (_stopping.compare_exchange_strong(expected, true)) {
		_cv.notify_all();
	}
}

void TaskThreadPool::workerLoop()
{
	for(;;) {
		std::function<void()> task;
		{
			std::unique_lock<std::mutex> lock(_mtx);
			_cv.wait(lock, [this]() { return _stopping || !_tasks.empty(); });
			if (_stopping && _tasks.empty()) return;
			task = std::move(_tasks.front());
			_tasks.pop();
		}
		if (task) task();
	}
} 