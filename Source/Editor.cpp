#include "Editor.h"

Editor::Editor() :
	//piece_table(L"new document"),
	page(),
    caret(),
    text_color{BLACK},
    text_size{12}

{
}

void Editor::update()
{
    page.update();
	write();
}

void Editor::render() const 
{
    page.draw();
    page.draw_settings();
	//page.draw(piece_table, caret);
	
}

void Editor::write()
{
    const int key = GetCharPressed();

    if (key >= 32 && key <= 126) 
    {
        //piece_table.insert(caret.get_index(), std::wstring(1, key), text_color, text_size );
        //caret.move_right(piece_table.get_line_length(caret.get_index()));
    }

    if (IsKeyPressed(KEY_BACKSPACE))
    {
        //piece_table.remove(caret.get_index());
    }

    if (IsKeyPressed(KEY_LEFT))
    {
        caret.move_left();
    }
    if (IsKeyPressed(KEY_RIGHT))
    {
        //caret.move_right();
    }
    if (IsKeyPressed(KEY_UP))
    {
        //caret.move_up();
    }
    if (IsKeyPressed(KEY_DOWN))
    {
        //caret.move_down();
    }

}
