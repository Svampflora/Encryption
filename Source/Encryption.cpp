#include "Encryption.h"
#include "Analysis.h"



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
	if (IsKeyReleased(KEY_ENTER))
	{
		return std::make_unique<Analysis>();
	}
	
	menu.update();
	text_box.write();

	return nullptr;
}

Encryption::Encryption() : //TODO: throws?
	encryptor{},
	text_box(LoadFont("assets/pixelmix/pixelmix.TTF"), { middle_of_screen().x - GetScreenWidthF() * 0.1f, middle_of_screen().y }, GetScreenHeightF() * 0.05f ),
	menu(LoadFont("assets/pixelmix/pixelmix.TTF")),
	file_manager("assets/message.txt")
{
	const Rectangle menu_area{ GetScreenWidthF() * 0.1f, GetScreenHeightF() * 0.1f, GetScreenWidthF() * 0.2f, GetScreenHeightF() * 0.8f };
	const float button_height = menu_area.height / 8;

	Rectangle button_area{ menu_area.x, menu_area.y, menu_area.width, button_height };
	Button button_1("Addative (Shift 3)", button_area, [this]() {
		this->encryptor.add_cipher([](const std::wstring& message) noexcept {
			return Encryptor::addative_cipher(message, 3);
			});
		});
	menu.add_button(button_1); 

	button_area.y += button_height;
	Button button_2("Multiplicative (Shift 3)", button_area, [this]() {
		this->encryptor.add_cipher([](const std::wstring& message) {
			return Encryptor::multiplicative_cipher(message, 3);
			});
		});
	menu.add_button(button_2);

	button_area.y += button_height;

	Button button_3("Key Word", button_area, [this]() {
		this->encryptor.add_cipher([this](const std::wstring& message) {
			return Encryptor::keyword_cipher(message, this->text_box.get_text(), 'k');
			});
		});
	menu.add_button(button_3);

	button_area.y += button_height;
	Button button_4("vigenère", button_area, [this]() {
		this->encryptor.add_cipher([this](const std::wstring& message) {
			return Encryptor::vigenere(message, this->text_box.get_text());
			});
		});
	menu.add_button(button_4);

	button_area.y += button_height;
	Button button_5("Rövarspråk", button_area, [this]() {
		this->encryptor.add_cipher([](const std::wstring& message) {
			return Encryptor::rövarspråk(message);
			});
		});
	menu.add_button(button_5);

	button_area.y += button_height;
	Button button_6("Encrypt", button_area, [this]() {
		this->file_manager.write(this->encryptor.encrypt(this->file_manager.read()));
		});
	menu.add_button(button_6);

}

void Encryption::Render() const noexcept
{
	text_box.draw();
	menu.draw();
}
