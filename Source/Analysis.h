#pragma once
#include"State.h"
#include "Cipher_analyser.h"
#include "File_manager.h"
#include <vector>


class Analysis : public State
{
	Cipher_analyser decryptor;
	File_manager file_manager;
public:
	Analysis() noexcept;
	std::unique_ptr<State> Update() override;
	void Render() const noexcept override;
};