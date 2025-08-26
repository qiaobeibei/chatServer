#pragma once
#include <unordered_map>
#include <mutex>

class CallManager {
public:
	static CallManager &Inst();

	enum class CallState { Idle = 0, Ringing, InCall };

	// 检查是否可发起通话（双方必须空闲）
	bool canCall(int caller, int callee, const char** reason = nullptr);
	// 进入振铃（呼叫中）
	void startRinging(int caller, int callee);
	// 被叫接听，进入通话中
	bool accept(int callee, int caller, const char** reason = nullptr);
	// 挂断（任意一方），返回对端uid，若无会话返回 -1
	int hangup(int uid, const char** reason = nullptr);
	// 查询对端
	int peerOf(int uid) const;
	CallState stateOf(int uid) const;

private:
	CallManager() = default;
	struct Entry { int peer{ -1 }; CallState state{ CallState::Idle }; };
	mutable std::mutex _mtx;
	std::unordered_map<int, Entry> _map; // uid -> {peer, state}
}; 