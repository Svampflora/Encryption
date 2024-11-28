#pragma once

#include <string>
#include "Utilities.h"
#include <string_view>
#include <vector>
#include <functional>


class Button
{
	using Action = std::function<void()>;
	Action action;
	std::string label;
	Vector2 position;
	float width;
	float height;

public:

	Button(const std::string& _label, const Rectangle& _rectangle, Action _action) :
		action(_action),
		label(_label),
		position({ _rectangle.x, _rectangle.y }),
		width(_rectangle.width),
		height(_rectangle.height)
	{}

	void click() const
	{
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && is_hovering())
		{
			if (action)
			{
				action();
			};
		}
	}

	void draw(const Font& _font) const noexcept
	{
		if (is_hovering())
		{
			DrawRectanglePro(hit_box(), { 0.0f,0.0f }, 0.0f, PINK);
			DrawTextPro(_font, label.data(), position, { 0.0f, 0.0f }, 0.0f, half_of(height), 1, WHITE);
		}
		else
		{
			DrawRectanglePro(hit_box(), { 0.0f,0.0f }, 0.0f, GRAY);
			DrawTextPro(_font, label.data(), position, { 0.0f, 0.0f }, 0.0f, half_of(height), 1, PINK);
		}
	}

private:
	Rectangle hit_box() const noexcept
	{
		return { position.x, position.y, width, height };
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
	Menu(const Font& _font) noexcept:
		font(_font)
	{
	}

	void update()
	{
		for (const Button& button : buttons)
		{
			button.click();
		}
	}

	void add_button(const Button& _button)
	{
		buttons.push_back(_button);
	}

	void draw() const noexcept
	{
		for (const Button& button : buttons)
		{
			button.draw(font);
		}
	}
};
