#include "WinScreen.h"

#include <imgui.h>

#include "LevelManager.h"
#include "ScoreManager.h"

void WinScreen::BeginPlay()
{
	m_pScoreManager = &ScoreManager::GetInstance();
}

void WinScreen::Render() const
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

    ImGui::Begin("WinScreen", nullptr, windowFlags);

    // Title
    ImGui::SetWindowFontScale(2.0f);
    const char* winTitle = "YOU WON!";
    float titleWidth = ImGui::CalcTextSize(winTitle).x;
    ImGui::SetCursorPosX((windowSize.x - titleWidth) * 0.5f);
    ImGui::SetCursorPosY(windowSize.y * 0.4f);
    ImGui::Text("%s", winTitle);

    // Score
    ImGui::SetWindowFontScale(1.0f);
    std::string scoreText = "Score: " + std::to_string(m_pScoreManager->GetScore());
    float scoreWidth = ImGui::CalcTextSize(scoreText.c_str()).x;
    ImGui::SetCursorPosX((windowSize.x - scoreWidth) * 0.5f);
    ImGui::SetCursorPosY(windowSize.y * 0.5f);
    ImGui::Text("%s", scoreText.c_str());

    // Button to return to main menu
    ImVec2 buttonSize(100, 50);
    ImGui::SetCursorPosX((windowSize.x - buttonSize.x) * 0.5f);
    ImGui::SetCursorPosY(windowSize.y * 0.65f);
    if (ImGui::Button("Main Menu", buttonSize))
    {
        LevelManager::GetInstance().LoadNextLevel();
    }

    ImGui::End();
}
