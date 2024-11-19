#pragma once



static constexpr int SCREEN_WIDTH = 1920;
static constexpr int SCREEN_HEIGHT = 1080 - 80;

static constexpr wchar_t ALPHABET_ENG[] = L"abcdefghijklmnopqrstuvwxyz";
static constexpr wchar_t ALPHABET_SWE[] = L"abcdefghijklmnopqrstuvwxyzåäö";
static constexpr size_t ALPHABET_SIZE = sizeof(ALPHABET_SWE) - 1;

