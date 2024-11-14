#pragma once
#include"State.h"
#include "Decryptor.h"
#include <vector>


class Decryption : public State
{
	Decryptor decryptor;
public:
	std::unique_ptr<State> Update() override;
	void Render() const noexcept override;
};