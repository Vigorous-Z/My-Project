#pragma once
#include "scene.h"

struct SelectScene : public Scene{
public:
	SelectScene() = default;
	~SelectScene() = default;

	virtual void on_enter() {}
	virtual void on_update(clock_t delta) {}
	virtual void on_draw() {}
	virtual void on_input(const ExMessage& msg) {}
	virtual void on_exit() {}

private:

};