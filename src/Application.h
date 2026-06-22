//
// Created by TheAwesomeGem on 12/2/2025.
//

#pragma once
#include <memory>

#include "game/Game.h"
#include "imgui/ToolRenderer.h"
#include "platform/GameWindow.h"
#include "renderer/Renderer.h"
#include "scene/Scene.h"


/**
 * TODOs
 * TODO: Add model loading capabilities maybe with Assimport
 */
class Application
{
public:
    NOT_COPYABLE_AND_MOVEABLE(Application);

    Application();
    void run();

private:
    void on_window_size_change(Vec2I new_size);

    GameWindow::WindowState m_window;
    Renderer::RendererState m_renderer = {};
    Input::InputState m_input;
    ToolRenderer::ToolState m_tool;
    std::unique_ptr<Game> m_game;
};
