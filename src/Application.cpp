//
// Created by TheAwesomeGem on 12/2/2025.
//

#include "Application.h"

#include "util/RandomExt.h"


Application::Application()
    : m_renderer{ }, m_window{  }, m_input{ }, m_tool{ }, m_game{ nullptr }
{
    m_game = std::make_unique<Game>(m_renderer, m_tool);
}

void Application::run()
{
    // App initialization
    {
        GameWindow::create(m_window, Vec2I{ .x = 1920, .y = 1080 });
        Renderer::create(m_renderer, m_window.hwnd);

        // m_window.event_callback = [this](const SDL_Event* event)
        // {
        //     m_tool->on_event(event);
        // };
        // m_window.size_callback = [this](Vec2I /*size*/)
        // {
        //     m_renderer->on_window_size_change();
        // };
        Input::create(m_input, m_window);
        auto [device, device_context] = Renderer::get_tool_context(m_renderer);
        ToolRenderer::create(m_tool, m_window.window.wnd, device.get(), device_context.get());

        m_game->create(m_input);
    }
    // ==================

    // Game Loop
    {
        uint64_t prev_ticks = 0;
        uint64_t current_ticks = SDL_GetPerformanceCounter();
        while (!m_window.should_close)
        {
            prev_ticks = current_ticks;
            current_ticks = SDL_GetPerformanceCounter();
            double const delta_time = (double)(current_ticks - prev_ticks) / (double)SDL_GetPerformanceFrequency();

            GameWindow::loop(m_window);
            Input::loop(m_input);
            m_game->update(m_window, delta_time);
            m_game->render();
        }
    }
    // ==================
}

void Application::on_window_size_change(Vec2I /*new_size*/)
{
    Renderer::on_window_size_change(m_renderer);
}
