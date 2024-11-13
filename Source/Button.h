#pragma once
#include <codeanalysis\warnings.h>
#pragma warning(push)
#pragma warning(disable:ALL_CODE_ANALYSIS_WARNINGS)
#include "raylib.h"
#pragma warning(pop)
#include <string>
#include "Utilities.h"


class Button
{
	std::string text;
	Vector2 position;
	float width;
	float height;

public:

	Button(const std::string& _text, const Vector2& _position, const float& _width, const float& _height) :
		text(_text),
		position(_position),
		width(_width),
		height(_height)
	{}

	bool is_clicked() const noexcept
	{
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && is_hovering())
		{
			return true;
		}
		return false;
	}

	void draw()
	{
		if (is_hovering())
		{
			DrawRectanglePro(hit_box(), { 0.0f,0.0f }, 0.0f, GRAY);
		}
		else
		{
			DrawRectanglePro(hit_box(), { 0.0f,0.0f }, 0.0f, GRAY);
		}
	}

private:
	Rectangle hit_box() const noexcept
	{
		return { position.x - half_of(width), position.y - half_of(height), width, height };
	}

	bool is_hovering() const noexcept
	{
		return CheckCollisionPointRec(GetMousePosition(), hit_box());
	}
};

class Menu
{
	Font font;
	std::vector<Button> buttons;

public:
	Menu(const Font& _font, const Rectangle& _rectangle) :
		font(_font)
	{
	}

	void draw()
	{
		for (Button& button : buttons)
		{
			button.draw();
		}
	}
};