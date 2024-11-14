#include "Encryption.h"
#include "Decryption.h"



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
		return std::make_unique<Decryption>();
	}
	

	menu.update();
	text_box.write();

	return nullptr;
}

Encryption::Encryption() : //TODO: throws?
	encryptor{},
	text_box(LoadFont("assets/pixelmix/pixelmix.TTF"), { middle_of_screen().x - GetScreenWidthF() * 0.1f, middle_of_screen().y }, GetScreenHeightF() * 0.05f ),
	menu(LoadFont("assets/pixelmix/pixelmix.TTF"))
{
	const Rectangle menu_area{ GetScreenWidthF() * 0.7f, GetScreenHeightF() * 0.1f, GetScreenWidthF() * 0.2f, GetScreenHeightF() * 0.8f };
	const float button_height = menu_area.height / 8;

	Rectangle button_area{ menu_area.x, menu_area.y, menu_area.width, button_height };
	Button button_1("Addative (Shift 3)", button_area, [this]() {
		this->encryptor.add_cipher([](const std::string& message) noexcept {
			return Encryptor::addative_cipher(message, 3);
			});
		});
	menu.add_button(button_1);

	button_area.y += button_height;
	Button button_2("Rövarspråk", button_area, [this]() {
		this->encryptor.add_cipher(Encryptor::Rövarspråk);
		});
	menu.add_button(button_2);

	button_area.y += button_height;
	Button button_3("Multiplicative (Shift 3)", button_area, [this]() {
		this->encryptor.add_cipher([](const std::string& message) noexcept {
			return Encryptor::multiplicative_cipher(message, 3);
			});
		});
	menu.add_button(button_3);

	button_area.y += button_height;
	Button button_4("Encrypt", button_area, [this]() {
		this->text_box.set_text(this->encryptor.encrypt(this->text_box.get_text()));
		});
	menu.add_button(button_4);

}

void Encryption::Render() const noexcept
{
	text_box.draw();
	menu.draw();
}
