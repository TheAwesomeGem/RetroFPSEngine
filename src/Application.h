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
 * TODO: Switched to an event queue type model instead of callbacks to handle ToolRenderer code inside Game
 * TODO: Add model loading capabilities maybe with Assimport
 */
namespace Application
{
    struct AppState
    {
        NOT_COPYABLE_AND_MOVEABLE(AppState);
        AppState() = default;

        GameWindow::WindowState window = {};
        Renderer::RendererState renderer = {};
        Input::InputState input = {};
        ToolRenderer::ToolState tool = {};
        Game::GameState game = {};
        std::vector<GameWindow::WindowEvent> window_events = {};
    };

    void run(AppState& app);
}
