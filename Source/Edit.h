#pragma once
#include"State.h"
#include "Editor.h"


class Edit : public State
{
	Editor editor;
public:
	Edit() noexcept;
	std::unique_ptr<State> Update() override;
	void Render() const noexcept override;
};