#include "MainMenuComp.h"

#include <imgui.h>
#include <SDL.h>

#include "IRendererService.h"
#include "LevelManager.h"
#include "SceneManager.h"
#include "ServiceAllocator.h"
#include "SoundSystem.hpp"
#include "TextureComponent.h"
#include "Transform.h"

void MainMenuComp::BeginPlay()
{
	Component::BeginPlay();

    const auto windowHalfSize = ServiceAllocator::GetRenderer().GetWindowSize() * .5f;
	GetOwner().GetTransform()->SetPosition(windowHalfSize);

    ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0, 0, 0, 0); // transparent background

    ServiceAllocator::GetSoundSystem().PlayMusic("../Data/Sounds/BGM.wav", 0.8f);
}

void MainMenuComp::Render() const
{
    ImVec2 windowSize = ImGui::GetIO().DisplaySize;

    // Set position and size BEFORE Begin
    ImGui::SetNextWindowPos(ImVec2(0, 0)); 
    ImGui::SetNextWindowSize(windowSize);

    ImGuiWindowFlags windowFlags = 
        ImGuiWindowFlags_NoTitleBar | 
        ImGuiWindowFlags_NoResize | 
        ImGuiWindowFlags_NoMove | 
        ImGuiWindowFlags_NoScrollbar | 
        ImGuiWindowFlags_NoCollapse | 
        ImGuiWindowFlags_NoBringToFrontOnFocus;

    ImGui::Begin("Canvas", nullptr, windowFlags);

    // ---- UI ----
    ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);

    ImGui::SetCursorPosX((windowSize.x - m_ButtonSize.x) * 0.5f);
    ImGui::SetCursorPosY((windowSize.y - m_ButtonSize.y) * 0.5f);

    // Set focus to first button for controller/keyboard

    if (ImGui::Button("1 Player", {m_ButtonSize.x, m_ButtonSize.y}))
    {
       LevelManager::GetInstance().LoadNextLevel();
    }

    ImGui::SetCursorPosX((windowSize.x - m_ButtonSize.x) * 0.5f);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20.0f);

    if (ImGui::Button("Quit game", {m_ButtonSize.x, m_ButtonSize.y}))
    {
        SDL_Quit();
        exit(0);
        // Implement your quit functionality here
        // e.g.: 
        // yourApplication.Quit();
    }

    ImGui::End();
}

MainMenuComp::MainMenuComp(dae::GameObject& owner)
	: Component(owner)
{
    m_BackgroundTexture = owner.AddComponent<TextureComponent>("background.png");
}