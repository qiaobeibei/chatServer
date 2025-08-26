#include "KafkaMgr.h"
#include <rdkafka.h>
#include <librdkafka/rdkafkacpp.h>
#include <iostream>

KafkaMgr& KafkaMgr::Inst()
{
	static KafkaMgr inst;
	return inst;
}

bool KafkaMgr::init(const std::string& brokers, const std::string& group)
{
	std::string errstr;
	_confGlobal.reset(RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL));
	_confTopic.reset(RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC));
	if (!_confGlobal || !_confTopic) {
		std::cerr << "kafka conf create failed" << std::endl;
		return false;
	}
	_confGlobal->set("bootstrap.servers", brokers, errstr);
	_confGlobal->set("group.id", group, errstr);
	_confGlobal->set("enable.partition.eof", "false", errstr);
	_confGlobal->set("auto.offset.reset", "earliest", errstr);

	_producer.reset(RdKafka::Producer::create(_confGlobal.get(), errstr));
	if (!_producer) {
		std::cerr << "create producer failed: " << errstr << std::endl;
		return false;
	}
	return true;
}

bool KafkaMgr::produce(const std::string& topic, const std::string& key, const std::string& payload)
{
	if (!_enabled || !_producer) return false;
	RdKafka::ErrorCode resp = _producer->produce(
		topic, RdKafka::Topic::PARTITION_UA,
		RdKafka::Producer::RK_MSG_COPY,
		(void*)payload.data(), payload.size(),
		&key, nullptr);
	if (resp != RdKafka::ERR_NO_ERROR) {
		std::cerr << "produce failed: " << RdKafka::err2str(resp) << std::endl;
		return false;
	}
	_producer->poll(0);
	return true;
}

bool KafkaMgr::consume(const std::string& topic, std::function<void(const std::string& key, const std::string& payload)> cb)
{
	if (!_enabled) return false;
	std::string errstr;
	_consumer.reset(RdKafka::KafkaConsumer::create(_confGlobal.get(), errstr));
	if (!_consumer) {
		std::cerr << "create consumer failed: " << errstr << std::endl;
		return false;
	}
	auto err = _consumer->subscribe({ topic });
	if (err) {
		std::cerr << "subscribe failed: " << RdKafka::err2str(err) << std::endl;
		return false;
	}
	_onMessage = std::move(cb);
	_consumeTopic = topic;
	_running = true;
	_consumerThread = std::thread([this]() { startConsumerLoop(); });
	return true;
}

bool KafkaMgr::startConsumerLoop()
{
	while (_running.load()) {
		std::unique_ptr<RdKafka::Message> msg(_consumer->consume(100));
		if (!msg) continue;
		switch (msg->err()) {
		case RdKafka::ERR_NO_ERROR: {
			std::string key;
			if (msg->key()) key.assign(*msg->key());
			std::string payload((const char*)msg->payload(), msg->len());
			if (_onMessage) _onMessage(key, payload);
			break; }
		case RdKafka::ERR__TIMED_OUT:
			break;
		default:
			std::cerr << "consume error: " << msg->errstr() << std::endl;
			break;
		}
	}
	return true;
}

void KafkaMgr::close()
{
	_running = false;
	if (_consumer) {
		_consumer->close();
	}
	if (_consumerThread.joinable()) _consumerThread.join();
	_producer.reset();
	_consumer.reset();
	_confGlobal.reset();
	_confTopic.reset();
} 