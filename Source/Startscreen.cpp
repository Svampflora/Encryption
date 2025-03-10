#include "Startscreen.h"
#include "Edit.h"


Start_screen::Start_screen() noexcept{}


std::unique_ptr<State> Start_screen::Update()
{
	if (IsKeyReleased(KEY_SPACE))
	{
		return std::make_unique<Edit>();
	}
	return nullptr;
}

void Start_screen::Render() const noexcept
{
	Draw_title();
}

void Start_screen::Draw_title() const noexcept
{
	DrawText("PRESS SPACE TO BEGIN", 200, 350, 40, PINK);
}