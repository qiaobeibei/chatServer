#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <memory>
#include <fstream>
#include "TaskThreadPool.h"

struct FileChunkMeta {
	std::string fileId; // 客户端侧生成的唯一文件ID
	std::string fileName;
	std::string targetDir;
	uint64_t totalSize{0};
	uint32_t totalChunks{0};
	uint32_t index{0}; // 当前分片序号，从 0 开始
	std::string data;  // 二进制数据
};

class FileTask {
public:
	FileTask(const std::string &fileId,
		const std::string &fileName,
		const std::string &dir,
		uint64_t totalSize,
		uint32_t totalChunks);
	~FileTask();

	// 返回是否已经全部完成
	bool appendChunk(uint32_t index, const std::string &data, std::string &err);
	std::string finalPath() const { return _finalPath; }

private:
	bool openIfNeed(std::string &err);

	std::string _fileId;
	std::string _fileName;
	std::string _dir;
	std::string _tmpPath;
	std::string _finalPath;
	uint64_t _totalSize;
	uint32_t _totalChunks;
	uint32_t _received{0};
	std::ofstream _ofs;
	std::mutex _mtx;
};

class FileUploadManager {
public:
	static FileUploadManager &Inst();

	// 初始化任务
	bool initTask(const std::string &fileId,
		const std::string &fileName,
		const std::string &dir,
		uint64_t totalSize,
		uint32_t totalChunks,
		std::string &err);
	// 提交分片（异步执行写磁盘）
	void submitChunk(const FileChunkMeta &meta);

private:
	FileUploadManager();
	std::shared_ptr<FileTask> getTaskNoLock(const std::string &fileId);

	std::unordered_map<std::string, std::shared_ptr<FileTask>> _tasks; // fileId -> task
	std::mutex _tasksMtx;
	TaskThreadPool _pool;
}; 