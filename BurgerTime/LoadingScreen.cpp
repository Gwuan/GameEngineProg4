#include "LoadingScreen.h"

#include <imgui.h>

#include "LevelManager.h"
#include "ScoreManager.h"

const float LoadingScreen::m_MaxLoadTime = 2.f;

void LoadingScreen::BeginPlay()
{
	m_pScoreManager = &ScoreManager::GetInstance();
}

void LoadingScreen::Update(const float deltaTime)
{
	if (!m_LoadingMap)
	{
		m_ElapsedTime += deltaTime;

		if (m_ElapsedTime >= m_MaxLoadTime)
		{
			m_LoadingMap = true;
			LevelManager::GetInstance().LoadNextLevel();
		}
	}
}

void LoadingScreen::Render() const
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

	float fontSize = ImGui::GetFontSize();
	float padding = 20.0f;

	// Left center
	std::string textLeftStr = "Extra lives: " + std::to_string(m_pScoreManager->GetExtraLives());
	ImGui::SetCursorPos(ImVec2(padding, (windowSize.y - fontSize) * 0.5f));
	ImGui::Text("%s", textLeftStr.c_str());

	// Top center
	std::string textCenterStr = "Score: " + std::to_string(m_pScoreManager->GetScore());
	float topWidth = ImGui::CalcTextSize(textCenterStr.c_str()).x;
	ImGui::SetCursorPos(ImVec2((windowSize.x - topWidth) * 0.5f, padding));
	ImGui::Text("%s", textCenterStr.c_str());

	// Loading text
	const char* text = "Loading...";
	float textWidth = ImGui::CalcTextSize(text).x;

	ImGui::SetCursorPos(ImVec2((windowSize.x - textWidth) * 0.5f, (windowSize.y - fontSize) * 0.5f));
	ImGui::Text("%s", text);

	// Right center
	std::string textRightStr = "Pepper: " + std::to_string(m_pScoreManager->GetAmountOfPepper());
	float rightWidth = ImGui::CalcTextSize(textRightStr.c_str()).x;
	ImGui::SetCursorPos(ImVec2(windowSize.x - rightWidth - padding, (windowSize.y - fontSize) * 0.5f));
	ImGui::Text("%s", textRightStr.c_str());

	ImGui::End();
}
