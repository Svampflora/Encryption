#pragma once
#include"State.h"
#include "Cipher_analyser.h"
#include <vector>


class Analysis : public State
{
	Cipher_analyser decryptor;
public:
	std::unique_ptr<State> Update() override;
	void Render() const noexcept override;
};