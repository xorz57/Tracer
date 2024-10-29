#pragma once

#include <chrono>
#include <fstream>
#include <mutex>
#include <string>

#include <nlohmann/json.hpp>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#define TRACER_SCOPE(name, categories, ...) TracerScope _tracer_scope_##__LINE__(name, categories, __VA_ARGS__)

#define TRACER_DURATION_EVENT_BEGIN(name, categories, ...) Tracer::getInstance().traceEvent(name, categories, "B", Tracer::getProcessId(), Tracer::getThreadId(), Tracer::getTimestamp(), __VA_ARGS__)
#define TRACER_DURATION_EVENT_END(name, categories, ...) Tracer::getInstance().traceEvent(name, categories, "E", Tracer::getProcessId(), Tracer::getThreadId(), Tracer::getTimestamp(), __VA_ARGS__)
#define TRACER_INSTANT_EVENT(name, categories, ...) Tracer::getInstance().traceEvent(name, categories, "i", Tracer::getProcessId(), Tracer::getThreadId(), Tracer::getTimestamp(), __VA_ARGS__)
#define TRACER_COUNTER_EVENT(name, categories, ...) Tracer::getInstance().traceEvent(name, categories, "C", Tracer::getProcessId(), Tracer::getThreadId(), Tracer::getTimestamp(), __VA_ARGS__)
#define TRACER_ASYNC_EVENT_BEGIN(name, categories, ...) Tracer::getInstance().traceEvent(name, categories, "b", Tracer::getProcessId(), Tracer::getThreadId(), Tracer::getTimestamp(), __VA_ARGS__)
#define TRACER_ASYNC_EVENT_INSTANT(name, categories, ...) Tracer::getInstance().traceEvent(name, categories, "n", Tracer::getProcessId(), Tracer::getThreadId(), Tracer::getTimestamp(), __VA_ARGS__)
#define TRACER_ASYNC_EVENT_END(name, categories, ...) Tracer::getInstance().traceEvent(name, categories, "e", Tracer::getProcessId(), Tracer::getThreadId(), Tracer::getTimestamp(), __VA_ARGS__)
#define TRACER_FLOW_EVENT_BEGIN(name, categories, ...) Tracer::getInstance().traceEvent(name, categories, "s", Tracer::getProcessId(), Tracer::getThreadId(), Tracer::getTimestamp(), __VA_ARGS__)
#define TRACER_FLOW_EVENT_INSTANT(name, categories, ...) Tracer::getInstance().traceEvent(name, categories, "t", Tracer::getProcessId(), Tracer::getThreadId(), Tracer::getTimestamp(), __VA_ARGS__)
#define TRACER_FLOW_EVENT_END(name, categories, ...) Tracer::getInstance().traceEvent(name, categories, "f", Tracer::getProcessId(), Tracer::getThreadId(), Tracer::getTimestamp(), __VA_ARGS__)

class Tracer {
public:
	Tracer(const Tracer&) = delete;
	Tracer(Tracer&&) = delete;

	static Tracer& getInstance() {
		static Tracer instance;
		return instance;
	}

	static std::uint64_t getTimestamp() {
		const auto now = std::chrono::high_resolution_clock::now();
		const auto duration = now.time_since_epoch();
		const auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration);
		return static_cast<std::uint64_t>(microseconds.count());
	}

	static std::uint64_t getProcessId() {
#ifdef _WIN32
		return GetCurrentProcessId();
#else
		return static_cast<std::uint64_t>(getpid());
#endif
	}

	static std::uint64_t getThreadId() {
#ifdef _WIN32
		return GetCurrentThreadId();
#else
		return static_cast<std::uint64_t>(pthread_self());
#endif
	}

	void traceEvent(const std::string &name, const std::string &categories, const std::string &phase, std::uint64_t pid, std::uint64_t tid, std::uint64_t timestamp, const nlohmann::json& args = nlohmann::json::object()) {
		std::lock_guard<std::mutex> lock(mDataMutex);
		nlohmann::json traceEvent;
		traceEvent["name"] = name;
		traceEvent["cat"] = categories;
		traceEvent["ph"] = phase;
		traceEvent["pid"] = pid;
		traceEvent["tid"] = tid;
		traceEvent["ts"] = timestamp;
		traceEvent["args"] = args;
		mData["traceEvents"].emplace_back(traceEvent);
	}

	void dump(const char* filename, int indent = 4) {
		std::lock_guard<std::mutex> lock(mDataMutex);
		std::ofstream ofs(filename);
		ofs << mData.dump(indent);
	}

private:
	Tracer() = default;
	~Tracer() = default;

	nlohmann::json mData;
	std::mutex mDataMutex;
};

class TracerScope {
public:
	TracerScope(const char* name, const char* categories, const nlohmann::json& args) : name(name), categories(categories), args(args) {
		Tracer::getInstance().traceEvent(name, categories, "B", Tracer::getProcessId(), Tracer::getThreadId(), Tracer::getTimestamp(), args);
	}

	TracerScope(const char* name, const char* categories) : name(name), categories(categories), args(nlohmann::json::object()) {
		Tracer::getInstance().traceEvent(name, categories, "B", Tracer::getProcessId(), Tracer::getThreadId(), Tracer::getTimestamp(), args);
	}

	~TracerScope() {
		Tracer::getInstance().traceEvent(name, categories, "E", Tracer::getProcessId(), Tracer::getThreadId(), Tracer::getTimestamp(), args);
	}

private:
	const std::string name;
	const std::string categories;
	const nlohmann::json args;
};
