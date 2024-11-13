#pragma once
#include "State.h"
#include "Encryptor.h"


class Encryption : public State
{
	Encryptor encryptor{};
	Text_box text_box;


public:
	
	Encryption() noexcept;
	std::unique_ptr<State> Update() override;
	void Render() const noexcept override;
};