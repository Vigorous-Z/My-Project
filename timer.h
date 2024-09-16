#pragma once

#include <functional>
#include <time.h>
using namespace std;

struct Timer {
public:
	Timer() {}
	~Timer() {}

	void Restart() {
		pass_time = 0;
		shotted = false;
	}
	//设置定时器
	//ms 定时器触发间隔
	//function 触发函数
	//flag 是否单次触发
	void SetTimer(clock_t ms, function<void()> callback, bool flag) {
		wait_time = ms;
		one_shot = flag;
		this->callback = callback;
	}
	void SetWaitTime(clock_t ms) {
		wait_time = ms;
	}
	void SetOneShot(bool flag) {
		one_shot = flag;
	}
	void Pause(bool flag) {
		paused = flag;
	}
	void OnUpdate(clock_t delta) {
		if (paused)return;
		pass_time += delta;
		if (pass_time >= wait_time) {
			if ((!one_shot || (one_shot && !shotted)) && callback)
				callback();
			shotted = true;
			pass_time = 0;
		}
	}
	void RestartPause(bool flag) {
		paused = flag;
		pass_time = 0;
	}
	void Shot() {
		if ((!one_shot || (one_shot && !shotted)) && callback)
			callback();
		shotted = true;
		pass_time = 0;
	}
private:
	clock_t pass_time = 0;
	clock_t wait_time = 0;
	bool paused = false;
	bool shotted = false;
	bool one_shot = false;
	function<void()> callback;
};