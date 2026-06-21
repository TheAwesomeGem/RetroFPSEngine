//
// Created by TheAwesomeGem on 12/9/2025.
//

#pragma once
#include "src/imgui/ToolRenderer.h"
#include "src/platform/Input.h"

#include "src/scene/Scene.h"

class Game
{
public:
    NOT_COPYABLE_AND_MOVEABLE(Game);

    explicit Game(Renderer* renderer, ToolRenderer::ToolState& tool);
    void create(Input::InputState& input);
    void update(const GameWindow::WindowState& window, double delta_time);
    void render();

private:
    Scene m_scene;
    Renderer* m_renderer;
    Input::InputState* m_input;
    ToolRenderer::ToolState* m_tool;
    ActorHandle m_player_handle;
    ActorHandle m_crate_handle;
    bool m_is_tool_shown;
};
