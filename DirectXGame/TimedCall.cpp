#include "TimedCall.h"

TimedCall::TimedCall(std::function<void()> func, uint32_t time) 
: func_(func), time_(time) {}

void TimedCall::Update() {
	if (isBullet_) {
		return;
	}

	time_--;
	if (time_ <= 0) {
		isBullet_ = true;
		// コールバック関数の呼び出し
		func_();
	}
}
