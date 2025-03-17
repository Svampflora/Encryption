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

    void move_left() noexcept
    { 
        if (index > 0) index--;
    }

    void move_right() noexcept
    { 
        /*if (index < line_length)*/ index++;
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

class Page
{
    vec2 format;
    float character_spacing;
    float zoom;
    float scroll;
    float margin;
    float line_spacing;
    
public:

    Page() noexcept :
        format{ Format::A(4) },
        character_spacing{ 1.0f },
        zoom{ 0.5f },
        scroll{0.0f},
        margin{ 25.0f },
        line_spacing{ 1.0f }
    {
    }

    void draw(const Piece_table& piece_table, const Caret& caret) const noexcept
    {
        draw_blank_page();
        draw_text(piece_table, caret);
    }

    void update() noexcept
    {
        const float scroll_input = GetMouseWheelMove();
        if (is_hovering())
        {
            zoom += scroll_input * GetFrameTime();
            zoom = clamp(zoom, 0.1f, 1.0f);
        }
        else
        {
            scroll += scroll_input * GetFrameTime();
            scroll = clamp(scroll, -0.5f, 0.5f);
        }
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
        const float scroll_offset = page_size.y * scroll;
        return { screen_center.x - half_of(page_size.x), screen_center.y - half_of(page_size.y) + scroll_offset };
    }

    vec2 size() const noexcept
    {
        const float relative_zoom = zoom * screen_width();
        return{ Format::A().x * relative_zoom, Format::A().y * relative_zoom };
    }

    float max_line_width() const noexcept
    {
        const float total_width = size().x;
        return total_width - 2 * margin;
    }

    bool is_hovering() const noexcept
    {
        return CheckCollisionPointRec(GetMousePosition(), rectangle());
    }

    /*void draw_text(const Piece_table& piece_table) const noexcept 
    {
        vec2 draw_position = first_line();
        float remaining_width = max_line_width();

        for (const auto& piece : piece_table.get_pieces()) 
        {
            const std::wstring& source = piece.is_original ? piece_table.get_original_text() : piece_table.get_add_buffer();
            std::wstring text = source.substr(piece.start, piece.length);

            while (!text.empty()) 
            {
                const vec2 piece_size = MeasureTextEx(GetFontDefault(), TextFormat("%ls", text.c_str()), piece.size * zoom, character_spacing);

                if (piece_size.x > remaining_width) 
                {
                    size_t cut_length = text.length();
                    while (cut_length > 0) 
                    {
                        std::wstring candidate = text.substr(0, cut_length);
                        const vec2 candidate_size = MeasureTextEx(GetFontDefault(), TextFormat("%ls", candidate.c_str()), piece.size * zoom, character_spacing);

                        if (candidate_size.x <= remaining_width) break;
                        cut_length--;
                    }

                    std::wstring part_to_draw = text.substr(0, cut_length);
                    DrawTextEx(GetFontDefault(), TextFormat("%ls", part_to_draw.c_str()), draw_position, piece.size * zoom, character_spacing, piece.color);

                    draw_position.x = first_line().x;
                    draw_position.y += piece_size.y + line_spacing;
                    remaining_width = max_line_width();

                    text = text.substr(cut_length);
                }
                else 
                {
                    DrawTextEx(GetFontDefault(), TextFormat("%ls", text.c_str()), draw_position, piece.size * zoom, character_spacing, piece.color);

                    draw_position.x += piece_size.x;
                    remaining_width -= piece_size.x;

                    text.clear();
                }
            }
        }
    }*/

    void draw_text(const Piece_table& piece_table, const Caret& caret) const noexcept 
    {
        vec2 draw_position = first_line();
        float remaining_width = max_line_width();
        size_t character_index = 0;
        vec2 caret_position = draw_position;

        for (const auto& piece : piece_table.get_pieces()) 
        {
            const std::wstring& source = piece.is_original ? piece_table.get_original_text() : piece_table.get_add_buffer();
            std::wstring text = source.substr(piece.start, piece.length);

            while (!text.empty()) 
            {
                const vec2 piece_size = MeasureTextEx(GetFontDefault(), TextFormat("%ls", text.c_str()), piece.size * zoom, character_spacing);

                if (piece_size.x > remaining_width) 
                {
                    size_t cut_length = text.length();
                    while (cut_length > 0) 
                    {
                        std::wstring candidate = text.substr(0, cut_length);
                        const vec2 candidate_size = MeasureTextEx(GetFontDefault(), TextFormat("%ls", candidate.c_str()), piece.size * zoom, character_spacing);
                        if (candidate_size.x <= remaining_width) break;
                        cut_length--;
                    }

                    // Todo: magic font
                    std::wstring part_to_draw = text.substr(0, cut_length);
                    DrawTextEx(GetFontDefault(), TextFormat("%ls", part_to_draw.c_str()), draw_position, piece.size * zoom, character_spacing, piece.color);

                    // find caret
                    if (caret.get_index() >= character_index && caret.get_index() < character_index + cut_length) 
                    {
                        const vec2 caret_offset = MeasureTextEx(GetFontDefault(), TextFormat("%ls", text.substr(0, caret.get_index() - character_index).c_str()), piece.size * zoom, character_spacing);
                        caret_position = { draw_position.x + caret_offset.x, draw_position.y };
                    }

                    //Todo: next_line()
                    draw_position.x = first_line().x;
                    draw_position.y += piece_size.y + line_spacing;
                    remaining_width = max_line_width();
                    character_index += cut_length;
                    text = text.substr(cut_length);
                }
                else 
                {
                    // Todo: magic font
                    DrawTextEx(GetFontDefault(), TextFormat("%ls", text.c_str()), draw_position, piece.size * zoom, character_spacing, piece.color);

                    if (caret.get_index() >= character_index && caret.get_index() < character_index + text.length()) 
                    {
                        const vec2 caret_offset = MeasureTextEx(GetFontDefault(), TextFormat("%ls", text.substr(0, caret.get_index() - character_index).c_str()), piece.size * zoom, character_spacing);
                        caret_position = { draw_position.x + caret_offset.x, draw_position.y };
                    }

                    //Todo: next_letter()
                    draw_position.x += piece_size.x;
                    remaining_width -= piece_size.x;
                    character_index += text.length();
                    text.clear();
                }
            }
        }

        //Todo: move to caret.draw
        if (caret.blink())
        {
            DrawLineV(caret_position, { caret_position.x, caret_position.y + 20.0f }, BLACK);

        }
    }


};