//
// Created by TheAwesomeGem on 12/2/2025.
//

#include "Application.h"

#include "util/RandomExt.h"


Application::Application()
    : m_renderer{ nullptr }, m_window{ nullptr }, m_input{ nullptr }, m_tool{ nullptr }, m_game{ nullptr }
{
    m_renderer = std::make_unique<Renderer>();
    m_window = std::make_unique<GameWindow>();
    m_input = std::make_unique<Input>();
    m_tool = std::make_unique<ToolRenderer>();
    m_game = std::make_unique<Game>(m_renderer.get(), m_tool.get());
}

void Application::run()
{
    // App initialization
    {
        m_window->create(Vec2I{ .x = 1920, .y = 1080 });
        m_renderer->init(m_window->get_hwnd());

        m_window->m_event_callback = [this](const SDL_Event* event)
        {
            m_tool->on_event(event);
        };
        m_window->m_size_callback = [this](Vec2I /*size*/)
        {
            m_renderer->on_window_size_change();
        };
        m_input->initialize(m_window->m_window_raw);
        auto [device, device_context] = m_renderer->get_tool_context();
        m_tool->init(m_window->m_window_raw, device.get(), device_context.get());

        m_game->init(m_input.get());
    }
    // ==================

    // Game Loop
    {
        uint64_t prev_ticks = 0;
        uint64_t current_ticks = SDL_GetPerformanceCounter();
        while (!m_window->should_close())
        {
            prev_ticks = current_ticks;
            current_ticks = SDL_GetPerformanceCounter();
            double const delta_time = (double)(current_ticks - prev_ticks) / (double)SDL_GetPerformanceFrequency();

            m_window->loop();
            m_input->loop();
            m_game->update(delta_time);
            m_game->render();
        }
    }
    // ==================
}

void Application::on_window_size_change(Vec2I /*new_size*/) const
{
    m_renderer->on_window_size_change();
}
