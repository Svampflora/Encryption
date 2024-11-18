#pragma once
#include "Settings.h"
#include "Button.h"
#include "utf-8.h"

class Text_box
{
	std::wstring text;
	Font font;
	Vector2 position;
	float size;

public:
	Text_box(const Font& font, const Vector2& _position, const float& _size) noexcept:
		text(),
		font(font),
		position{ _position},
		size{_size}
	{}

	void write()
	{
		int key = GetCharPressed();

		while (key > 0)
		{
			if ((key >= 32) && (key <= 125))
			{
				text += narrow_cast<char>(key);
			}
			key = GetCharPressed();
		}

		if (IsKeyPressed(KEY_BACKSPACE) && !text.empty())
		{
			text.pop_back();
		}
	}

	std::wstring get_text() const noexcept
	{
		return text;
	}

	void draw() const noexcept
	{
		
		DrawRectanglePro(box(), { 0.0f, 0.0f }, 0.0f, GRAY);
		DrawTextPro(font, utf8_encode(text).c_str(), position, {0.0f, 0.0f}, 0.0f, size, 1, PINK);
		draw_caret();
	}

private:
	Rectangle box() const noexcept
	{
		std::wstring elongated = text + L"a";
		const Vector2 mesurements = MeasureTextEx(font, utf8_encode(elongated).c_str(), size, 1.0f);
		return { position.x - padding(), position.y - padding(), mesurements.x + 2 * padding(), mesurements.y + 2 * padding() };
	}

	bool blink() const noexcept
	{
		return narrow_cast<int>(GetTime()) % 2 == 0;
	}

	constexpr float padding() const noexcept
	{
		return 5.0f;
	} 

	void draw_caret() const noexcept
	{
		if (!blink()) return;

		std::string a = "a";
		const Vector2 mesurements = MeasureTextEx(font, a.data(), size, 1.0f);
		const Vector2 caret_position = { position.x + mesurements.x * text.size(), position.y};
		DrawRectangleRec({ caret_position.x, caret_position.y, mesurements.x, mesurements.y }, PINK);
	}
};