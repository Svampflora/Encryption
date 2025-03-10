#include "Piece_table.h"

Piece_table::Piece_table(const std::wstring& text) : original_text(text)
{
    pieces.push_back({ 0, text.size(), BLACK, 12, true });
}

void Piece_table::insert(size_t pos, const std::wstring& new_text, const Color& color, int size)
{
    if (pos > total_size()) return;

    const size_t add_start = add_buffer.size();
    add_buffer += new_text;

    std::vector<Piece> new_pieces;
    size_t current_pos = 0;

    for (const auto& piece : pieces)
    {
        if (current_pos + piece.length > pos)
        {
            // Split piece
            const size_t offset = pos - current_pos;
            if (offset > 0) new_pieces.push_back({ piece.start, offset, color, size, piece.is_original });

            // Insert new text
            new_pieces.push_back({ add_start, new_text.size(), color, size, false });

            // Add the remainder of the split piece
            const size_t remaining = piece.length - offset;
            if (remaining > 0) new_pieces.push_back({ piece.start + offset, remaining, piece.color, piece.size, piece.is_original });
        }
        else
        {
            new_pieces.push_back(piece);
        }
        current_pos += piece.length;
    }

    pieces = std::move(new_pieces);
}

const std::wstring& Piece_table::get_original_text() const noexcept
{
    return original_text;
}

const std::wstring& Piece_table::get_add_buffer() const noexcept
{
    return add_buffer;
}

const std::vector<Piece>& Piece_table::get_pieces() const noexcept
{
    return pieces;
}

void Piece_table::print()
{
    for (const auto& piece : pieces)
    {
        const std::wstring& buffer = add_buffer; piece.is_original ? original_text : add_buffer;
        std::wcout << buffer.substr(piece.start, piece.length) << L" ";
    }
    std::wcout << std::endl;
}

size_t Piece_table::total_size() const noexcept
{
    size_t total = 0;
    for (const auto& piece : pieces) total += piece.length;
    return total;
}

wchar_t Piece_table::get_char_at(size_t index) const
{
    size_t current_pos = 0;

    for (const auto& piece : pieces) 
    {
        if (index < current_pos + piece.length) 
        {
            return add_buffer.at(index - current_pos);
        }

        current_pos += piece.length;
    }

    return L'\0'; 
}

