/*
	MIT License

	Copyright(c) 2024 George Fotopoulos

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files(the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions :

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#pragma once

#include <chrono>
#include <fstream>
#include <mutex>
#include <string>

#include <nlohmann/json.hpp>

#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>
#else
#include <unistd.h>
#endif

#define TRACER_CONCAT_IMPL(a, b) a##b
#define TRACER_CONCAT(a, b) TRACER_CONCAT_IMPL(a, b)

#if defined(__GNUC__) || defined(__clang__)
#define TRACER_FUNCTION(categories, ...) TracerScope TRACER_CONCAT(_tracer_scope_, __LINE__)(__PRETTY_FUNCTION__, categories, #__VA_ARGS__)
#elif defined(_MSC_VER)
#define TRACER_FUNCTION(categories, ...) TracerScope TRACER_CONCAT(_tracer_scope_, __LINE__)(__FUNCSIG__, categories, #__VA_ARGS__)
#else
#define TRACER_FUNCTION(categories, ...) TracerScope TRACER_CONCAT(_tracer_scope_, __LINE__)(__func__, categories, #__VA_ARGS__)
#endif

#define TRACER_SCOPE(name, categories, ...) TracerScope TRACER_CONCAT(_tracer_scope_, __LINE__)(name, categories, #__VA_ARGS__)

#define TRACER_DURATION_EVENT_BEGIN(name, categories, ...) Tracer::getInstance().traceEvent(name, categories, "B", Tracer::getProcessId(), Tracer::getThreadId(), Tracer::getTimestamp(), #__VA_ARGS__)
#define TRACER_DURATION_EVENT_END(name, categories, ...) Tracer::getInstance().traceEvent(name, categories, "E", Tracer::getProcessId(), Tracer::getThreadId(), Tracer::getTimestamp(), #__VA_ARGS__)
#define TRACER_INSTANT_EVENT(name, categories, ...) Tracer::getInstance().traceEvent(name, categories, "i", Tracer::getProcessId(), Tracer::getThreadId(), Tracer::getTimestamp(), #__VA_ARGS__)
#define TRACER_COUNTER_EVENT(name, categories, ...) Tracer::getInstance().traceEvent(name, categories, "C", Tracer::getProcessId(), Tracer::getThreadId(), Tracer::getTimestamp(), #__VA_ARGS__)
#define TRACER_ASYNC_EVENT_BEGIN(name, categories, ...) Tracer::getInstance().traceEvent(name, categories, "b", Tracer::getProcessId(), Tracer::getThreadId(), Tracer::getTimestamp(), #__VA_ARGS__)
#define TRACER_ASYNC_EVENT_INSTANT(name, categories, ...) Tracer::getInstance().traceEvent(name, categories, "n", Tracer::getProcessId(), Tracer::getThreadId(), Tracer::getTimestamp(), #__VA_ARGS__)
#define TRACER_ASYNC_EVENT_END(name, categories, ...) Tracer::getInstance().traceEvent(name, categories, "e", Tracer::getProcessId(), Tracer::getThreadId(), Tracer::getTimestamp(), #__VA_ARGS__)
#define TRACER_FLOW_EVENT_BEGIN(name, categories, ...) Tracer::getInstance().traceEvent(name, categories, "s", Tracer::getProcessId(), Tracer::getThreadId(), Tracer::getTimestamp(), #__VA_ARGS__)
#define TRACER_FLOW_EVENT_INSTANT(name, categories, ...) Tracer::getInstance().traceEvent(name, categories, "t", Tracer::getProcessId(), Tracer::getThreadId(), Tracer::getTimestamp(), #__VA_ARGS__)
#define TRACER_FLOW_EVENT_END(name, categories, ...) Tracer::getInstance().traceEvent(name, categories, "f", Tracer::getProcessId(), Tracer::getThreadId(), Tracer::getTimestamp(), #__VA_ARGS__)

#define TRACER_DUMP(filename, indentation) Tracer::getInstance().dump(filename, indentation)

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

	void traceEvent(const char *name, const char *categories, const char *phase, std::uint64_t pid, std::uint64_t tid, std::uint64_t timestamp, const nlohmann::json& args = nlohmann::json::object()) {
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
	TracerScope(const char* name, const char* categories, const nlohmann::json& args) : mName(name), mCategories(categories), mArgs(args) {
		Tracer::getInstance().traceEvent(name, categories, "B", Tracer::getProcessId(), Tracer::getThreadId(), Tracer::getTimestamp(), args);
	}

	TracerScope(const char* name, const char* categories) : mName(name), mCategories(categories), mArgs(nlohmann::json::object()) {
		Tracer::getInstance().traceEvent(name, categories, "B", Tracer::getProcessId(), Tracer::getThreadId(), Tracer::getTimestamp(), mArgs);
	}

	~TracerScope() {
		Tracer::getInstance().traceEvent(mName, mCategories, "E", Tracer::getProcessId(), Tracer::getThreadId(), Tracer::getTimestamp(), mArgs);
	}

private:
	const char* mName;
	const char* mCategories;
	const nlohmann::json mArgs;
};
