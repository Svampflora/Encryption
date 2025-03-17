#pragma once
#include <iostream>
#include <span>
#include "Utilities.h"
#include "Settings.h"
#include "Button.h"
#include "utf-8.h"

struct Piece 
{
    size_t start, length;
    Color color;
    int size;
    bool is_original;
};

class Piece_table 
{
    std::wstring original_text;
    std::wstring add_buffer;
    std::vector<Piece> pieces;

public:

    Piece_table(const std::wstring& text);
    void insert(size_t pos, const std::wstring& new_text, const Color& color, int size);
    const std::wstring& get_original_text() const noexcept;
    const std::wstring& get_add_buffer() const noexcept;
    const std::vector<Piece>& get_pieces() const noexcept;
    size_t total_size() const noexcept;
    wchar_t get_char_at(size_t index) const;
    void print();
    std::wstring get_text_range(size_t start_index, size_t length) const;

private:
};

class Caret
{
    size_t index;

public:
    size_t get_index() const noexcept
    {
        return index;
    }

    void move_left()
    { 
        if (index > 0) index--;
    }
    void move_right(size_t line_length) 
    { 
        if (index < line_length) index++;
    }
    //void move_up(const Piece_table& piece_table)
    //{
    //    size_t newIndex = index;
    //    while (newIndex > 0 && piece_table.get_char_at(newIndex) != L'\n') newIndex--;
    //    if (newIndex > 0) index = newIndex - 1;
    //}

    //void move_down(const Piece_table& piece_table) 
    //{
    //    size_t newIndex = index;
    //    while (newIndex < piece_table.total_size() && piece_table.get_char_at(newIndex) != L'\n') newIndex++;
    //    if (newIndex < piece_table.total_size()) index = newIndex + 1;
    //}
    bool blink() const noexcept
    {
        return narrow_cast<int>(GetTime()) % 2 == 0;
    }
private:


};


namespace Format
{

    static inline vec2 A() noexcept
    {
        return { 1.0, sqrtf(2) };
    }

    static inline vec2 A( int format) noexcept
    {
        constexpr float A0_WIDTH = 1000.0f;
        const float A0_HEIGHT = A0_WIDTH * sqrtf(2);

        float width = A0_WIDTH;
        float height = A0_HEIGHT;

        for (int i = 0; i < format; i++) 
        {
            std::swap(width, height);
            height *= 0.5f;
        }

        return { width, height };
    }


}

struct Line_info
{
    size_t start, length;
    vec2 position;
};

class Page
{
    vec2 format;
    size_t character_spacing;
    float zoom;
    float scroll;
    float margin;
    float line_spacing;
    
public:

    Page() noexcept :
        format{ Format::A(4) },
        character_spacing{ 1 },
        zoom{ 0.5f },
        scroll{0.0f},
        margin{ 20 },
        line_spacing{ 1.0f }
    {
    }

    //std::vector<Line_info> compute_lines(const Piece_table& piece_table)
    //{
    //    std::vector<Line_info> lines;
    //    float y = margin;
    //    const float max_width = format.x - margin * 2;

    //    size_t line_start = 0;
    //    const size_t total_length = piece_table.total_size();

    //    while (line_start < total_length) {
    //        size_t count = 0;
    //        float text_width = 0;
    //        const float current_font_size = piece_table.get_pieces().at(line_start).size;

    //        while (line_start + count < total_length) 
    //        {
    //            const Piece& piece = piece_table.get_pieces().at(line_start + count);

    //            if (piece.size != current_font_size) 
    //            {
    //                break; 
    //            }

    //            std::wstring current_text = piece_table.get_text_range(line_start, count + 1);
    //            text_width = MeasureText(TextFormat("%ls", current_text.c_str()), current_font_size);

    //            if (text_width > max_width) 
    //            {
    //                break;
    //            }

    //            count++;
    //        }

    //        // Handle explicit newlines
    //        std::wstring line_text = piece_table.get_text_range(line_start, count);
    //        size_t newline_pos = line_text.find(L'\n');
    //        if (newline_pos != std::wstring::npos) {
    //            count = newline_pos + 1;  // Include newline in the count
    //        }

    //        lines.push_back({ line_start, y });
    //        line_start += count;  // Move to the next line

    //        // Determine line height dynamically based on max font size in the current line
    //        float max_line_height = 0;
    //        for (size_t i = 0; i < count; i++) {
    //            max_line_height = std::max(max_line_height, piece_table.get_pieces().at(line_start + i).size * zoom);
    //        }

    //        y += max_line_height * 1.2f;  // Move down by the tallest font size in the line
    //    }
    //}

    //void draw(const Piece_table& piece_table, const Caret& caret) const noexcept
    //{
    //    draw_blank_page();
    //    draw_text(piece_table);
    //    draw_caret(piece_table, caret);
    //}
    void update()
    {
        const float scroll_input = GetMouseWheelMove();
        if (is_hovering())
        {
            zoom += scroll_input * GetFrameTime();
            zoom = clamp(zoom, 0.1f, 1.0f);
        }
    }

    void draw() const noexcept
    {
        draw_blank_page();
    }

    void draw_settings() const
    {
        std::string zoom_text = "ZOOM: " + std::format("{:.2f}", zoom * 100) + " %";
        const vec2 zoom_text_pos{ 5.0f, GetScreenHeight() - (0.1f * GetScreenHeight()) };
        DrawTextF(zoom_text.data(), zoom_text_pos.x, zoom_text_pos.y, 12, PINK);
    }

private:

    void draw_blank_page() const noexcept
    {
        DrawRectangleV(position(), size(), WHITE);
        DrawRectangleLinesEx(rectangle(), 5, DARKGRAY);
    }

    Rectangle rectangle() const noexcept
    {
        const vec2 page_position = position();
        const vec2 page_size = size();
        return { page_position.x, page_position.y, page_size.x, page_size.y };
    }

    vec2 first_line() const noexcept
    {
        const vec2 page_position = position();
        return { page_position.x + margin, page_position.y + margin };
    }

    vec2 position() const noexcept
    {
        const vec2 page_size = size();
        const vec2 screen_center{ half_of(screen_width()), half_of(screen_height()) };
        return { screen_center.x - half_of(page_size.x), screen_center.y - half_of(page_size.y) };
    }

    vec2 size() const noexcept
    {
        const float relative_zoom = zoom * screen_width();
        return{ Format::A().x * relative_zoom, Format::A().y * relative_zoom };
    }


    bool is_hovering() const noexcept
    {
        return CheckCollisionPointRec(GetMousePosition(), rectangle());
    }

    void draw_text(const Piece_table& piece_table) const noexcept
    {
        const float relative_zoom = zoom * screen_width();
        vec2 draw_position = { first_line().x * relative_zoom, first_line().y * relative_zoom};
        for (const auto& piece : piece_table.get_pieces()) 
        {
            const std::wstring& source = piece.is_original ? piece_table.get_original_text() : piece_table.get_add_buffer();
            std::wstring text = source.substr(piece.start, piece.length);
            DrawTextEx(GetFontDefault(), TextFormat("%ls", text.c_str()), draw_position, piece.size, character_spacing, piece.color);
            draw_position.y += piece.size + line_spacing;
        }
    }

    //void draw_caret(const Piece_table& piece_table, const Caret& caret) const
    //{
    //    if (!caret.blink()) return;

    //    const float font_size = 12; // Todo: magic font_size

    //    float x = position.x + 50;  // Todo: magic margin
    //    float y = position.y + 50;

    //    std::string a = "a"; 
    //    vec2 mesurements = MeasureTextEx(GetFontDefault(), a.data(), font_size, 1.0f); //Todo: replace magic settings
    //    mesurements.x *= zoom;
    //    mesurements.y *= zoom;

    //    size_t current_index = 0;
    //    for (const auto& piece : piece_table.get_pieces())
    //    {
    //        for (const wchar_t c : piece_table.get_add_buffer())
    //        {
    //            if (current_index == caret.get_index())
    //            {
    //                DrawRectangleRec({ x, y, mesurements.x, mesurements.y }, PINK);

    //                return;
    //            }
    //            x += mesurements.x;
    //            if (c == L'\n')
    //            {
    //                x = position.x + 50;
    //                y += mesurements.y;
    //            }
    //            current_index++;
    //        }
    //    }
    //}

};