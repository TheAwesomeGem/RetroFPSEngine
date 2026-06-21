//
// Created by TheAwesomeGem on 12/9/2025.
//

#pragma once
#include "src/platform/Input.h"

#include "src/scene/Scene.h"

class ToolRenderer;

class Game
{
public:
    NOT_COPYABLE_AND_MOVEABLE(Game);

    explicit Game(Renderer* renderer, ToolRenderer* tool);
    void create(Input::InputState& input);
    void update(double delta_time);
    void render();

private:
    Scene m_scene;
    Renderer* m_renderer;
    Input::InputState* m_input;
    ToolRenderer* m_tool;
    ActorHandle m_player_handle;
    ActorHandle m_crate_handle;
    bool m_is_tool_shown;
};
