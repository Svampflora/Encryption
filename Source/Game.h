#pragma once
#include "Window.h"
#include "Startscreen.h"
#include "Encryption.h"
#include "Edit.h"

class Game
{
	Window window{ "Lönnruna", SCREEN_WIDTH, SCREEN_HEIGHT };
	std::unique_ptr<State> current_state = std::make_unique<Start_screen>();

public:
	void Update();
	void Render() const noexcept;
};