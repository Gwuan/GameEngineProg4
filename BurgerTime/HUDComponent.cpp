#include "HUDComponent.h"

#include "imgui.h"
#include "ResourceManager.h"
#include "ScoreManager.h"
#include "TextComponent.h"

void HUDComponent::BeginPlay()
{
	m_pScoreManager = &ScoreManager::GetInstance();
}

void HUDComponent::Render() const
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

	// Set background to fully transparent
    ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(0, 0, 0, 0));
	ImGui::Begin("Canvas", nullptr, windowFlags);

	ImDrawList* drawList = ImGui::GetWindowDrawList();
	ImFont* font = ImGui::GetFont();
	const float fontSize = ImGui::GetFontSize();
	ImU32 color = IM_COL32(255, 255, 255, 255);

	std::string textLeftStr = "Extra lives: " + std::to_string(m_pScoreManager->GetExtraLives());
	std::string textCenterStr = "Score: " + std::to_string(m_pScoreManager->GetScore());
	std::string textRightStr = "Pepper: " + std::to_string(m_pScoreManager->GetAmountOfPepper());

	const char* textLeft = textLeftStr.c_str();
	const char* textCenter = textCenterStr.c_str();
	const char* textRight = textRightStr.c_str();

	constexpr float padding = 10.0f;

	ImVec2 posLeft = ImVec2(padding, padding);
	float textCenterWidth = ImGui::CalcTextSize(textCenter).x;
	ImVec2 posCenter = ImVec2((windowSize.x - textCenterWidth) * 0.5f, padding);
	float textRightWidth = ImGui::CalcTextSize(textRight).x;
	ImVec2 posRight = ImVec2(windowSize.x - textRightWidth - padding, padding);

	drawList->AddText(font, fontSize, posLeft, color, textLeft);
	drawList->AddText(font, fontSize, posCenter, color, textCenter);
	drawList->AddText(font, fontSize, posRight, color, textRight);


	ImGui::End();
	ImGui::PopStyleColor(1);
}

HUDComponent::HUDComponent(dae::GameObject& owner)
	: Component(owner)
{
}