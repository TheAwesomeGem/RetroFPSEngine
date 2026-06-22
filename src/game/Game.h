//
// Created by TheAwesomeGem on 12/9/2025.
//

#pragma once
#include "src/imgui/ToolRenderer.h"
#include "src/platform/Input.h"

#include "src/scene/Scene.h"

namespace Game
{
    struct GameState
    {
        NOT_COPYABLE_AND_MOVEABLE(GameState);
        GameState() = default;

        Scene::SceneState scene = {};
        Renderer::RendererState* renderer = nullptr;
        Input::InputState* input = nullptr;
        ToolRenderer::ToolState* tool = nullptr;
        Scene::ActorHandle player_handle = Scene::ActorHandle::invalid();
        Scene::ActorHandle crate_handle = Scene::ActorHandle::invalid();
        bool is_tool_shown = false;
    };

    void create(GameState& game, Renderer::RendererState& renderer, Input::InputState& input, ToolRenderer::ToolState& tool);
    void update(GameState& game, const GameWindow::WindowState& window, double delta_time);
    void render(GameState& game);
}
