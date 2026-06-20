//
// Created by TheAwesomeGem on 11/30/2025.
//

#include "Input.h"

#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_mouse.h>

Input::Input()
    : m_window_raw{ nullptr }, m_keys{}
{
}

void Input::initialize(SDL_Window* window_raw)
{
    m_window_raw = window_raw;
    SDL_SetWindowRelativeMouseMode(m_window_raw, true);

    int key_count = 0;
    SDL_GetKeyboardState(&key_count);
    m_keys = std::vector(key_count, KeyState{ .was_being_pressed = false, .is_being_pressed = false });
}

void Input::loop()
{
    // handle mouse polling
    {
        m_mouse.prev_pos = m_mouse.cur_pos;

        SDL_GetRelativeMouseState(&m_mouse.delta.x, &m_mouse.delta.y);
        SDL_GetMouseState(&m_mouse.cur_pos.x, &m_mouse.cur_pos.y); // TODO: Handle mouse buttons later
    }
    // ================

    // handle keyboard polling
    {
        const bool* keys = SDL_GetKeyboardState(nullptr);

        for (size_t i = 0; i < m_keys.size(); ++i)
        {
            m_keys[i].was_being_pressed = m_keys[i].is_being_pressed;
            m_keys[i].is_being_pressed = keys[i];
        }
    }
    // ================
}

void Input::enable_mouse_cursor(bool is_enabled)
{
    SDL_SetWindowRelativeMouseMode(m_window_raw, !is_enabled);
}
