#include "CallManager.h"

CallManager &CallManager::Inst()
{
	static CallManager inst;
	return inst;
}

bool CallManager::canCall(int caller, int callee, const char** reason)
{
	std::lock_guard<std::mutex> g(_mtx);
	auto get = [&](int uid)->Entry& { return _map[uid]; };
	Entry &a = get(caller), &b = get(callee);
	if (a.state != CallState::Idle) { if (reason) *reason = "caller busy"; return false; }
	if (b.state != CallState::Idle) { if (reason) *reason = "callee busy"; return false; }
	return true;
}

void CallManager::startRinging(int caller, int callee)
{
	std::lock_guard<std::mutex> g(_mtx);
	_map[caller] = { callee, CallState::Ringing };
	_map[callee] = { caller, CallState::Ringing };
}

bool CallManager::accept(int callee, int caller, const char** reason)
{
	std::lock_guard<std::mutex> g(_mtx);
	auto itCallee = _map.find(callee);
	auto itCaller = _map.find(caller);
	if (itCallee == _map.end() || itCaller == _map.end()) { if (reason) *reason = "no ringing"; return false; }
	if (itCallee->second.peer != caller || itCaller->second.peer != callee) { if (reason) *reason = "peer mismatch"; return false; }
	if (itCallee->second.state != CallState::Ringing || itCaller->second.state != CallState::Ringing) { if (reason) *reason = "not ringing"; return false; }
	itCallee->second.state = CallState::InCall;
	itCaller->second.state = CallState::InCall;
	return true;
}

int CallManager::hangup(int uid, const char** reason)
{
	std::lock_guard<std::mutex> g(_mtx);
	auto it = _map.find(uid);
	if (it == _map.end()) { if (reason) *reason = "no session"; return -1; }
	int peer = it->second.peer;
	_map.erase(uid);
	if (peer != -1) _map.erase(peer);
	return peer;
}

int CallManager::peerOf(int uid) const
{
	std::lock_guard<std::mutex> g(_mtx);
	auto it = _map.find(uid);
	if (it == _map.end()) return -1;
	return it->second.peer;
}

CallManager::CallState CallManager::stateOf(int uid) const
{
	std::lock_guard<std::mutex> g(_mtx);
	auto it = _map.find(uid);
	if (it == _map.end()) return CallState::Idle;
	return it->second.state;
} 