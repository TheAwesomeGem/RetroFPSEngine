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
    void on_window_size_change(Vec2I new_size) const;

    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<GameWindow> m_window;
    std::unique_ptr<Input> m_input;
    std::unique_ptr<ToolRenderer> m_tool;
    std::unique_ptr<Game> m_game;
};
