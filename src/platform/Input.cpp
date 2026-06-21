//
// Created by TheAwesomeGem on 11/30/2025.
//

#include "Input.h"

#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_mouse.h>

void Input::enable_mouse_cursor(const InputState& input, bool is_enabled)
{
    SDL_SetWindowRelativeMouseMode(input.window->window.wnd, !is_enabled);
}

void Input::create(InputState& input, const GameWindow::WindowState& window)
{
    input.window = &window;
    SDL_SetWindowRelativeMouseMode(window.window.wnd, true);

    int key_count = 0;
    SDL_GetKeyboardState(&key_count);
    input.keys = std::vector(key_count, KeyState{ .was_being_pressed = false, .is_being_pressed = false });
}

void Input::loop(InputState& input)
{
    // handle mouse polling
    {
        input.mouse.prev_pos = input.mouse.cur_pos;

        SDL_GetRelativeMouseState(&input.mouse.delta.x, &input.mouse.delta.y);
        SDL_GetMouseState(&input.mouse.cur_pos.x, &input.mouse.cur_pos.y); // TODO: Handle mouse buttons later
    }
    // ================

    // handle keyboard polling
    {
        const bool* keys = SDL_GetKeyboardState(nullptr);

        for (size_t i = 0; i < input.keys.size(); ++i)
        {
            input.keys[i].was_being_pressed = input.keys[i].is_being_pressed;
            input.keys[i].is_being_pressed = keys[i];
        }
    }
    // ================
}
