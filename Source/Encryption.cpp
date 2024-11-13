#include "Encryption.h"
#include "Endscreen.h"



template<typename T, typename Func>
void CheckConditionAndPerformAction(T value, Func action)
{
	if (value)
	{
		action();
	}
}

template <typename T>
void RenderObjects(const std::vector<T>& objects, const Texture& texture) noexcept
{
	for (const T& obj : objects)
	{
		obj.Render(texture);
	}
}

template <typename T>
void UpdateObjects(std::vector<T>& objects)  noexcept
{
	for (T& obj : objects)
	{
		obj.Update();
	}
}

template <typename Container, typename Predicate>
void remove_if(Container& container, Predicate predicate)
{
	container.erase(
		std::remove_if(container.begin(), container.end(), predicate),
		container.end());
}



std::unique_ptr<State> Encryption::Update()
{
	if (IsKeyReleased(KEY_Q))
	{
		return std::make_unique<End_screen>();
	}

	if (IsKeyPressed(KEY_ONE))
	{
		encryptor.add_cipher([](const std::string& message) noexcept
			{
			return Encryptor::Caesar_cipher(message, 3);
			});
	}

	if (IsKeyPressed(KEY_TWO))
	{
		encryptor.add_cipher([](const std::string& message) noexcept
			{
				return Encryptor::Rövarspråk(message);
			});
	}

	if (IsKeyPressed(KEY_ENTER))
	{
		std::string encrypted_text = encryptor.encrypt(text_box.get_text());
		text_box.set_text(encrypted_text);
	}

	text_box.write();

	return nullptr;
}

Encryption::Encryption() noexcept :
	text_box(LoadFont("assets/pixelmix/pixelmix.TTF"), { middle_of_screen().x - GetScreenWidthF() * 0.1f, middle_of_screen().y }, GetScreenHeightF() * 0.05f )
{}

void Encryption::Render() const noexcept
{
	text_box.draw();
}
