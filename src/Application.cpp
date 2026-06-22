//
// Created by TheAwesomeGem on 12/2/2025.
//

#include "Application.h"

#include "util/RandomExt.h"

void Application::run(AppState& app)
{
    // App initialization
    {
        GameWindow::create(app.window, Vec2I{ .x = 1920, .y = 1080 });
        Renderer::create(app.renderer, app.window.hwnd);

        app.window.event_callback = [](const SDL_Event* event)
        {
            ToolRenderer::on_event(event);
        };
        app.window.size_callback = [](void* receiver, Vec2I /*size*/)
        {
            Renderer::on_window_size_change(((AppState*)receiver)->renderer);
        };
        Input::create(app.input, app.window);
        auto [device, device_context] = Renderer::get_tool_context(app.renderer);
        ToolRenderer::create(app.tool, app.window.window.wnd, device.get(), device_context.get());

        Game::create(app.game, app.renderer, app.input, app.tool);
    }
    // ==================

    // Game Loop
    {
        uint64_t prev_ticks = 0;
        uint64_t current_ticks = SDL_GetPerformanceCounter();
        while (!app.window.should_close)
        {
            prev_ticks = current_ticks;
            current_ticks = SDL_GetPerformanceCounter();
            double const delta_time = (double)(current_ticks - prev_ticks) / (double)SDL_GetPerformanceFrequency();

            GameWindow::loop(app.window, &app);
            Input::loop(app.input);
            Game::update(app.game, app.window, delta_time);
            Game::render(app.game);
        }
    }
    // ==================
}
