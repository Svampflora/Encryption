#include "Analysis.h"
#include "Startscreen.h"
#include <fstream>
#include <iostream>


std::unique_ptr<State> Analysis::Update()
{


	if (IsKeyReleased(KEY_ENTER))
	{
		return std::make_unique<Start_screen>();
	}
	return nullptr;
}

void Analysis::Render() const noexcept
{

}

