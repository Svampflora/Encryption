#include "Decryption.h"
#include "Startscreen.h"
#include <fstream>
#include <iostream>


std::unique_ptr<State> Decryption::Update()
{


	if (IsKeyReleased(KEY_ENTER))
	{
		return std::make_unique<Start_screen>();
	}
	return nullptr;
}

void Decryption::Render() const noexcept
{

}

