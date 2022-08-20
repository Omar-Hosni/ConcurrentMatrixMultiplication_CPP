#pragma once
#include <vector>
#include <thread>

using namespace std;

class thread_guard {
	thread& t;

public:
	explicit thread_guard(thread& _t) : t(_t) {}

	~thread_guard()
	{
		if (t.joinable()) {
			t.join();
		}
	}

	thread_guard(thread_guard& const) = delete;
	thread_guard& operator=(thread_guard& const) = delete;
};

class join_threads {
	vector<thread>& threads;

public:
	explicit join_threads(vector<thread>& _threads) : threads(_threads) {}


	~join_threads() {
		for (long i = 0; i < threads.size(); i++) {
			if (threads[i].joinable()) {
				threads[i].join();
			}
		}
	}
};

class function_wrapper {
	struct impl_base {
		virtual void call() = 0;
		virtual ~impl_base() {}
	};

	template<typename F>
	struct impl_type : impl_base
	{
		F f;
		impl_type(F&& f_) : f(move(f_)) {}
		void call() { f(); }
	};

	unique_ptr<impl_base> impl;

public:
	template<typename F>
	function_wrapper(F&& f) :impl(new impl_type<F>(move(f))) {}
	void operator()() { impl->call(); }

	function_wrapper() {}

	function_wrapper(function_wrapper&& other) :
		impl(std::move(other.impl))
	{}

	function_wrapper& operator=(function_wrapper&& other)
	{
		impl = std::move(other.impl);
		return *this;
	}

	function_wrapper(const function_wrapper&) = delete;
	function_wrapper(function_wrapper&) = delete;


};

