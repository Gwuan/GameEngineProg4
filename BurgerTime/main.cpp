#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "BurgerBasket.h"
#include "BurgerComponent.h"
#include "ColliderComponent.h"
#include "EnemyComponent.h"
#include "EnemyType.h"
#include "GameOverScreen.h"
#include "HUDComponent.h"
#include "InputManager.h"
#include "JsonResolver.h"
#include "LoadingScreen.h"
#include "MainMenuComp.h"
#include "Minigin.h"
#include "PeterPepperComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "WinScreen.h"

void RegisterGameComponents()
{
	auto& componentFactory = ComponentFactory::GetInstance();

	componentFactory.Register("BurgerSliceComponent",
    [](dae::GameObject& go, const nlohmann::json& jsonData) -> Component*
    {
        if (!jsonData.contains("sliceType") ||
            !jsonData["sliceType"].is_string()) 
        {
            throw std::runtime_error("The given sliceType is invalid.");
        }

        std::string sliceTypeStr = jsonData["sliceType"].get<std::string>();

        SliceType sliceType = SliceType::BottomBread;

        if (sliceTypeStr == "BottomBread") {
            sliceType = SliceType::BottomBread;
        } 
        else if (sliceTypeStr == "Meat") {
            sliceType = SliceType::Meat;
        } 
        else if (sliceTypeStr == "Salad") {
            sliceType = SliceType::Salad;
        } 
        else if (sliceTypeStr == "TopBread") {
            sliceType = SliceType::TopBread;
        } 
        else {
            throw std::runtime_error("Unknown SliceType.");
        }

        return go.AddComponent<BurgerSliceComponent>(sliceType);
    });

	componentFactory.Register("EnemyComponent",
    [](dae::GameObject& go, const nlohmann::json& jsonData) -> Component*
    {
        if (!jsonData.contains("enemyType") ||
            !jsonData["enemyType"].is_string()) 
        {
            throw std::runtime_error("The given enemyType is invalid.");
        }

        std::string enemyTypeStr = jsonData["enemyType"].get<std::string>();

        EnemyType* enemyType = {};

        if (enemyTypeStr == "MrHotdog") {
            enemyType = &g_MrHotDog;
        } 
        else if (enemyTypeStr == "MrPickle") {
            enemyType = &g_MrPickle;
        } 
        else if (enemyTypeStr == "MrEgg") {
            enemyType = &g_MrEgg;
        } 
        else {
            throw std::runtime_error("Unknown EnemyType.");
        }

        return go.AddComponent<EnemyComponent>(*enemyType);
    });

	componentFactory.Register("BurgerBasket",
    [](dae::GameObject& go, const nlohmann::json&) -> Component*
    {
        return go.AddComponent<BurgerBasket>();
    });

	componentFactory.Register("PeterPepper",
    [](dae::GameObject& go, const nlohmann::json&) -> Component*
    {
        return go.AddComponent<PeterPepperComponent>();
    });

	componentFactory.Register("HudComponent",
    [](dae::GameObject& go, const nlohmann::json&) -> Component*
    {
        return go.AddComponent<HUDComponent>();
    });

	componentFactory.Register("MainMenu",
    [](dae::GameObject& go, const nlohmann::json&) -> Component*
    {
        return go.AddComponent<MainMenuComp>();
    });

	componentFactory.Register("GameOver",
    [](dae::GameObject& go, const nlohmann::json&) -> Component*
    {
        return go.AddComponent<GameOverScreen>();
    });

	componentFactory.Register("LoadingScreen",
    [](dae::GameObject& go, const nlohmann::json&) -> Component*
    {
        return go.AddComponent<LoadingScreen>();
    });

	componentFactory.Register("WinScreen",
    [](dae::GameObject& go, const nlohmann::json&) -> Component*
    {
        return go.AddComponent<WinScreen>();
    });
}


void load()
{
	RegisterGameComponents();

	dae::SceneManager::GetInstance().LoadSceneFromJson("../Data/Levels/MainMenu.json");
}


int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}