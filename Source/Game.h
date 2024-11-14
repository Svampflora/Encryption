#pragma once
#include "Settings.h"
#include "Window.h"
#include "Startscreen.h"
#include "Encryption.h"
#include "Decryption.h"

class Game
{
	Window window{ "L�nnruna", SCREEN_WIDTH, SCREEN_HEIGHT };
	std::unique_ptr<State> current_state = std::make_unique<Start_screen>();

public:
	void Update();
	void Render() const noexcept;
};