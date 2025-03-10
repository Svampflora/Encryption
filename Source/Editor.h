#pragma once
#include "Cipher_analyser.h"
#include "Piece_table.h"
#include "File_manager.h"
#include <vector>

class Editor
{
	Piece_table piece_table;
	Page page;
	Caret caret;
	Color text_color;
	unsigned int text_size;

public:
	Editor();
	void update();
	void render() const noexcept;

private:
	void write();
};