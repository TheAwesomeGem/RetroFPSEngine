//
// Created by TheAwesomeGem on 11/30/2025.
//

#pragma once
#include <vector>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_video.h>

#include "GameWindow.h"
#include "src/Common.h"

namespace Input
{
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

    struct InputState
    {
        NOT_COPYABLE_AND_MOVEABLE(InputState);
        InputState() = default;

        MouseState mouse = MouseState{};
        std::vector<KeyState> keys = std::vector<KeyState>{};
        const GameWindow::WindowState* window = nullptr;
    };

    void create(InputState& input, const GameWindow::WindowState& window);
    void enable_mouse_cursor(const InputState& input, bool is_enabled);
    void loop(InputState& input);

    [[nodiscard]] inline bool is_key_down(const InputState& input, SDL_Scancode key)
    {
        return input.keys[key].is_being_pressed;
    }

    [[nodiscard]] inline bool is_key_just_pressed(const InputState& input, SDL_Scancode key)
    {
        const auto [was_being_pressed, is_being_pressed] = input.keys[key];

        return !was_being_pressed && is_being_pressed;
    }

    [[nodiscard]] inline bool was_key_just_released(const InputState& input, SDL_Scancode key)
    {
        const auto [was_being_pressed, is_being_pressed] = input.keys[key];

        return was_being_pressed && !is_being_pressed;
    }
}
