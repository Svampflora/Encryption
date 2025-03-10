#include "Edit.h"
#include "Encryption.h"
#include <fstream>
#include <iostream>


Edit::Edit() noexcept
{
	
}

std::unique_ptr<State> Edit::Update()
{
	
	if (IsKeyPressed(KEY_LEFT_SHIFT) && IsKeyReleased(KEY_K))
	{
		return std::make_unique<Encryption>();
	}

	editor.update();


	return nullptr;
}

void Edit::Render() const noexcept
{
	editor.render();
}

