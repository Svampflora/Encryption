#include "Startscreen.h"
#include "Encryption.h"

Start_screen::Start_screen() noexcept{}


std::unique_ptr<State> Start_screen::Update()
{
	if (IsKeyReleased(KEY_SPACE))
	{
		return std::make_unique<Encryption>();
	}
	return nullptr;
}

void Start_screen::Render() const noexcept
{
	Draw_title();
}

void Start_screen::Draw_title() const noexcept
{
	DrawText("SPACE INVADERS", 200, 100, 160, YELLOW);
	DrawText("PRESS SPACE TO BEGIN", 200, 350, 40, YELLOW);
}