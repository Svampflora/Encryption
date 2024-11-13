#pragma once
#include "Settings.h"
#include "Window.h"
#include "Startscreen.h"
#include "Encryption.h"
#include "Endscreen.h"

class Game
{
	Window window{ "Lönnruna", SCREEN_WIDTH, SCREEN_HEIGHT };
	std::unique_ptr<State> current_state = std::make_unique<Start_screen>();
	Score_data scoreData{};

public:
	void Update();
	void Render() const noexcept;
};