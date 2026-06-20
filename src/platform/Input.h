//
// Created by TheAwesomeGem on 11/30/2025.
//

#pragma once
#include <vector>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_video.h>

#include "src/Common.h"

struct KeyState
{
    bool was_being_pressed = false;
    bool is_being_pressed = false;
};

struct MouseState
{
    Vec2F prev_pos = Vec2F::Zero;
    Vec2F cur_pos = Vec2F::Zero;
    Vec2F delta = Vec2F::Zero;
};


class Input
{
public:
    NOT_COPYABLE_AND_MOVEABLE(Input);

    Input();

    [[nodiscard]] Vec2F get_prev_mouse_pos() const
    {
        return m_mouse.prev_pos;
    }

    [[nodiscard]] Vec2F get_cur_mouse_pos() const
    {
        return m_mouse.cur_pos;
    }

    [[nodiscard]] Vec2F get_mouse_delta() const
    {
        return m_mouse.delta;
    }

    [[nodiscard]] bool is_key_down(SDL_Scancode key) const
    {
        return m_keys[key].is_being_pressed;
    }

    [[nodiscard]] bool is_key_just_pressed(SDL_Scancode key) const
    {
        const auto [was_being_pressed, is_being_pressed] = m_keys[key];

        return !was_being_pressed && is_being_pressed;
    }

    [[nodiscard]] bool was_key_just_released(SDL_Scancode key) const
    {
        const auto [was_being_pressed, is_being_pressed] = m_keys[key];

        return was_being_pressed && !is_being_pressed;
    }

    void initialize(SDL_Window* window);
    void loop();
    void enable_mouse_cursor(bool is_enabled);

private:
    SDL_Window* m_window_raw;
    MouseState m_mouse;
    std::vector<KeyState> m_keys;
};
