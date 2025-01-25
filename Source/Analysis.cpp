#include "Analysis.h"
#include "Startscreen.h"
#include <fstream>
#include <iostream>


Analysis::Analysis() noexcept:
decryptor{},
file_manager("assets/message.txt")
{
}

std::unique_ptr<State> Analysis::Update()
{
	if (IsKeyReleased(KEY_ENTER))
	{
		return std::make_unique<Start_screen>();
	}

	if (IsKeyReleased(KEY_SPACE))
	{
		std::wstring message = file_manager.read();
		//message = decryptor.monoalphabetic_attack(message);
		file_manager.write(message);
	}


	return nullptr;
}

void Analysis::Render() const noexcept
{

}

