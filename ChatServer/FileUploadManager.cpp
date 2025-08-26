#include "FileUploadManager.h"
#include <sstream>
#include <iomanip>
#include <cstdio>

static std::string joinPath(const std::string &a, const std::string &b) {
	if (a.empty()) return b;
	if (a.back() == '/' || a.back() == '\\') return a + b;
	return a + "/" + b;
}

FileTask::FileTask(const std::string &fileId,
	const std::string &fileName,
	const std::string &dir,
	uint64_t totalSize,
	uint32_t totalChunks)
	: _fileId(fileId), _fileName(fileName), _dir(dir), _totalSize(totalSize), _totalChunks(totalChunks)
{
	_tmpPath = joinPath(_dir, _fileId + ".part");
	_finalPath = joinPath(_dir, _fileName);
}

FileTask::~FileTask()
{
	if (_ofs.is_open()) _ofs.close();
}

bool FileTask::openIfNeed(std::string &err)
{
	if (_ofs.is_open()) return true;
	_ofs.open(_tmpPath, std::ios::binary | std::ios::out | std::ios::trunc);
	if (!_ofs.is_open()) { err = "open tmp file failed"; return false; }
	return true;
}

bool FileTask::appendChunk(uint32_t index, const std::string &data, std::string &err)
{
	std::lock_guard<std::mutex> lock(_mtx);
	if (!openIfNeed(err)) return false;
	// 简化：假设客户端保证 index 顺序到达；若需要乱序支持，可记录位图并 seek
	_ofs.write(data.data(), static_cast<std::streamsize>(data.size()));
	if (!_ofs.good()) { err = "write failed"; return false; }
	_received++;
	if (_received == _totalChunks) {
		_ofs.flush();
		_ofs.close();
		if (std::rename(_tmpPath.c_str(), _finalPath.c_str()) != 0) { err = "rename failed"; return false; }
		return true; // complete
	}
	return false; // not complete
}

FileUploadManager &FileUploadManager::Inst()
{
	static FileUploadManager inst;
	return inst;
}

FileUploadManager::FileUploadManager()
	: _pool(4)
{
}

std::shared_ptr<FileTask> FileUploadManager::getTaskNoLock(const std::string &fileId)
{
	auto it = _tasks.find(fileId);
	if (it != _tasks.end()) return it->second;
	return nullptr;
}

bool FileUploadManager::initTask(const std::string &fileId,
	const std::string &fileName,
	const std::string &dir,
	uint64_t totalSize,
	uint32_t totalChunks,
	std::string &err)
{
	std::lock_guard<std::mutex> g(_tasksMtx);
	if (_tasks.find(fileId) != _tasks.end()) return true; // 幂等
	auto task = std::make_shared<FileTask>(fileId, fileName, dir, totalSize, totalChunks);
	_tasks[fileId] = task;
	return true;
}

void FileUploadManager::submitChunk(const FileChunkMeta &meta)
{
	auto self = this;
	_pool.submit([self, meta]() {
		std::shared_ptr<FileTask> task;
		{
			std::lock_guard<std::mutex> g(self->_tasksMtx);
			task = self->getTaskNoLock(meta.fileId);
		}
		if (!task) {
			return; // 未 init，直接丢弃或记录错误
		}
		std::string err;
		bool done = task->appendChunk(meta.index, meta.data, err);
		if (done) {
			// 完成后可从 map 清理
			std::lock_guard<std::mutex> g(self->_tasksMtx);
			self->_tasks.erase(meta.fileId);
		}
	});
} 