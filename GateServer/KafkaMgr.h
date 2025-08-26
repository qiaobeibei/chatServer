#pragma once
#include <string>
#include <functional>
#include <memory>
#include <thread>
#include <atomic>

namespace RdKafka { class KafkaConsumer; class Producer; class Message; class Conf; }

class KafkaMgr {
public:
	static KafkaMgr& Inst();
	bool init(const std::string& brokers, const std::string& group);
	bool enable() const { return _enabled; }
	void setEnabled(bool e) { _enabled = e; }
	bool produce(const std::string& topic, const std::string& key, const std::string& payload);
	bool consume(const std::string& topic, std::function<void(const std::string& key, const std::string& payload)> cb);
	void close();
private:
	KafkaMgr() = default;
	bool startConsumerLoop();

	std::unique_ptr<RdKafka::Producer> _producer;
	std::unique_ptr<RdKafka::KafkaConsumer> _consumer;
	std::unique_ptr<RdKafka::Conf> _confGlobal;
	std::unique_ptr<RdKafka::Conf> _confTopic;
	std::thread _consumerThread;
	std::function<void(const std::string&, const std::string&)> _onMessage;
	std::string _consumeTopic;
	std::atomic<bool> _running{ false };
	bool _enabled{ false };
}; 