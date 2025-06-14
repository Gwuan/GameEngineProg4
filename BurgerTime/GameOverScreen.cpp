#include "GameOverScreen.h"
#include <imgui.h>
#include <SDL.h>

#include "LevelManager.h"

void GameOverScreen::Render() const
{
	ImVec2 windowSize = ImGui::GetIO().DisplaySize;

    ImGuiWindowFlags windowFlags = 
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoBringToFrontOnFocus;

    ImGui::SetNextWindowPos(ImVec2(0, 0)); 
    ImGui::SetNextWindowSize(windowSize);

    ImGui::Begin("Canvas", nullptr, windowFlags);

    // Title
    ImFont* font = ImGui::GetFont();
    float fontSize = ImGui::GetFontSize();
    ImU32 color = IM_COL32(255, 0, 0, 255);

    const char* gameOver = "Game Over";

    float textWidth = ImGui::CalcTextSize(gameOver).x;
    ImVec2 pos = ImVec2((windowSize.x - textWidth) * 0.5f, 50.0f);
    ImGui::GetWindowDrawList()->AddText(font, fontSize, pos, color, gameOver);

    // Buttons  
    constexpr float padding = 20.0f;
    ImVec2 buttonSize(100, 75);
    ImVec2 buttonPos((windowSize.x - buttonSize.x) * 0.5f, (windowSize.y - buttonSize.y) * 0.5f);

    ImGui::SetCursorPos(buttonPos);
    if (ImGui::Button("MainMenu", buttonSize))
    {
        LevelManager::GetInstance().LoadNextLevel();
    }

    buttonPos.y += buttonSize.y + padding;

    ImGui::SetCursorPos(buttonPos);
    if (ImGui::Button("Quit", buttonSize))
    {
        SDL_Quit();
        exit(0);
    }

    ImGui::End();
}

GameOverScreen::GameOverScreen(dae::GameObject& owner)
	: Component(owner)
{
}