#include "GameOverState.h"
#include "glm/vec3.hpp"
#include "SceneManager.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "RenderComponent.h"
#include "PlayerScoreCP.h"
#include "Scene.h"
#include "GalagaStrings.h"
#include <iostream>
#include "PlayerScoreCP.h"
#include <algorithm>
#include <fstream> // For file operations
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/ostreamwrapper.h"

GameOverState::~GameOverState()
{

}

void GameOverState::OnEnter()
{
	auto& sceneManager = engine::SceneManager::GetInstance();
	auto& scene = sceneManager.GetActiveScene();
	auto& window = sceneManager.GetSceneWindow();

	auto player = scene.FindGameObjectByTag(STR_PLAYER_TAG, false);
	PlayerScoreCP* pPlayerScore{};
	if (player != nullptr)
	{
		pPlayerScore = player->GetComponent<PlayerScoreCP>();
	}

	// Save the score to JSON
	if (pPlayerScore != nullptr)
	{
		SaveScoreToJson(pPlayerScore);
	}

	glm::vec3 gameOverPos{ (window.width / 2.f) - 80.f, (window.height / 2.f) - 80.f, 0.f };
	auto galaga_Font = engine::ResourceManager::GetInstance().LoadFont("Fonts/Emulogic-zrEw.ttf", 17);
	SDL_Color blueColor = { 0,255, 222 };

	auto gameOverText{ std::make_shared<engine::GameObject>(nullptr, "UI", gameOverPos) };
	gameOverText->AddComponent<engine::RenderComponent>(gameOverText.get());
	gameOverText->AddComponent<TextComponent>(gameOverText.get(), "GAME OVER", galaga_Font, blueColor);
	gameOverText->SetIsActive(false);

	SDL_Color redColor{ 255, 0, 0 };
	SDL_Color yellowColor{ 255, 255, 0 };
	//glm::vec3 resultsPos{ (window.width / 2.f) - 50.f, (window.height / 2.f) - 50.f, 0.f };
	auto resultsGO{ std::make_shared<engine::GameObject>(nullptr, "UI", gameOverPos) };
	resultsGO->AddComponent<engine::RenderComponent>(resultsGO.get());
	resultsGO->AddComponent<TextComponent>(resultsGO.get(), "-RESULTS-", galaga_Font, redColor);
	resultsGO->SetIsActive(false);


	std::string shotsFired{ "SHOTS FIRED     " };
	if (pPlayerScore != nullptr)
	{
		shotsFired += pPlayerScore->GetShots();
	}
	gameOverPos.x = gameOverPos.x - 100.f;
	gameOverPos.y = gameOverPos.y + 50.f;
	engine::GameObject* pShootsFired{ new engine::GameObject(resultsGO.get(), std::string{"UI"}, gameOverPos) };
	pShootsFired->AddComponent<engine::RenderComponent>(pShootsFired);
	pShootsFired->AddComponent<TextComponent>(pShootsFired, shotsFired, galaga_Font, yellowColor);

	std::string numberHits{ "NUMBER OF HITS   " };
	if (pPlayerScore != nullptr)
	{
		numberHits += pPlayerScore->GetHits();
	}
	gameOverPos.y = gameOverPos.y + 50.f;
	engine::GameObject* pNumberHits{ new engine::GameObject(resultsGO.get(), std::string{"UI"}, gameOverPos) };
	pNumberHits->AddComponent<engine::RenderComponent>(pNumberHits);
	pNumberHits->AddComponent<TextComponent>(pNumberHits, numberHits, galaga_Font, yellowColor);

	std::string ratio{ "HIT-MISS RATIO  " };
	if (pPlayerScore != nullptr)
	{
		ratio += pPlayerScore->GetRatio();
		ratio += " %";
	}

	gameOverPos.y = gameOverPos.y + 50.f;
	engine::GameObject* pRatio{ new engine::GameObject(resultsGO.get(), std::string{"UI"}, gameOverPos) };
	pRatio->AddComponent<engine::RenderComponent>(pRatio);
	pRatio->AddComponent<TextComponent>(pRatio, ratio, galaga_Font);

	// HighscoresGO implementation
	SDL_Color whiteColor = { 255, 255, 255 };
	glm::vec3 highScoresPos{ gameOverPos.x + 65.f, 200.f, 0.f };
	glm::vec3 ScoresPos{ gameOverPos.x + 100.f, 240.f, 0.f };
	auto highScoresGO{ std::make_shared<engine::GameObject>(nullptr, "UI", highScoresPos) };
	highScoresGO->AddComponent<engine::RenderComponent>(highScoresGO.get());
	highScoresGO->AddComponent<TextComponent>(highScoresGO.get(), "--HIGHSCORES--", galaga_Font, whiteColor);
	highScoresGO->SetIsActive(false);

	std::vector<std::pair<std::string, int>> highScores = LoadHighScores();
	highScoresPos.y += 50.f;

	auto galaga_FontSmall = engine::ResourceManager::GetInstance().LoadFont("Fonts/Emulogic-zrEw.ttf", 12);
	for (const auto& scorePair : highScores)
	{
		std::string scoreText = scorePair.first + "        " + std::to_string(scorePair.second);
		engine::GameObject* pHighScore{ new engine::GameObject(highScoresGO.get(), std::string{"UI"}, ScoresPos) };
		pHighScore->AddComponent<engine::RenderComponent>(pHighScore);
		pHighScore->AddComponent<TextComponent>(pHighScore, scoreText, galaga_FontSmall, yellowColor);
		ScoresPos.y += 40.f;
	}

	scene.Add(gameOverText);
	scene.Add(resultsGO);
	scene.Add(highScoresGO);

	m_GameOverObjects.emplace_back(gameOverText.get());
	m_GameOverObjects.emplace_back(resultsGO.get());
	m_GameOverObjects.emplace_back(highScoresGO.get());
}

void GameOverState::OnExit()
{

}

GameState* GameOverState::GetChangeState()
{
	return nullptr;
}

void GameOverState::UpdateState(const float deltaTime)
{
	if (m_CurrentShowing < int(m_GameOverObjects.size()))
	{
		UpdateUIObjects(deltaTime);
	}
}

void GameOverState::UpdateUIObjects(const float deltaTime)
{
	if (m_elapsedSec < m_MaxTime)
	{
		m_elapsedSec += deltaTime;
	}
	else
	{
		if (m_CurrentShowing < int(m_GameOverObjects.size()))
		{
			int previous{ m_CurrentShowing - 1 };
			if (previous >= 0)
			{
				m_GameOverObjects.at(previous)->MarkAsDead();
			}
			m_GameOverObjects.at(m_CurrentShowing)->SetIsActive(true);
			m_CurrentShowing++;
			m_MaxTime = 2.5f;
		}

		m_elapsedSec = 0.f;
	}
}

std::vector<std::pair<std::string, int>> GameOverState::LoadHighScores()
{
	using namespace rapidjson;
	std::vector<std::pair<std::string, int>> scores;

	std::ifstream ifs("PlayerScores.json");
	if (ifs.is_open())
	{
		IStreamWrapper isw(ifs);
		Document document;
		document.ParseStream(isw);
		if (document.HasMember("Scores") && document["Scores"].IsArray())
		{
			const Value& scoresArray = document["Scores"];
			for (SizeType i = 0; i < scoresArray.Size(); i++)
			{
				const Value& scoreObject = scoresArray[i];
				if (scoreObject.HasMember("Name") && scoreObject["Name"].IsString() &&
					scoreObject.HasMember("Score") && scoreObject["Score"].IsInt())
				{
					scores.emplace_back(scoreObject["Name"].GetString(), scoreObject["Score"].GetInt());
				}
			}
		}
		ifs.close();
	}

	std::sort(scores.begin(), scores.end(), [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
		return b.second < a.second;
		});

	return scores;
}

void GameOverState::SaveScoreToJson(PlayerScoreCP* pPlayerScore)
{
	using namespace rapidjson;

	Document document;
	document.SetObject();
	Document::AllocatorType& allocator = document.GetAllocator();

	std::vector<std::pair<std::string, int>> scores;

	// Read existing scores from the JSON file
	{
		std::ifstream ifs("PlayerScores.json");
		if (ifs.is_open())
		{
			IStreamWrapper isw(ifs);
			document.ParseStream(isw);
			if (document.HasMember("Scores") && document["Scores"].IsArray())
			{
				const Value& scoresArray = document["Scores"];
				for (SizeType i = 0; i < scoresArray.Size(); i++)
				{
					const Value& scoreObject = scoresArray[i];
					if (scoreObject.HasMember("Name") && scoreObject["Name"].IsString() &&
						scoreObject.HasMember("Score") && scoreObject["Score"].IsInt())
					{
						scores.emplace_back(scoreObject["Name"].GetString(), scoreObject["Score"].GetInt());
					}
				}
			}
			ifs.close();
		}
	}

	// Add the new score
	scores.emplace_back("TVH", pPlayerScore->GetCurrentScore());

	// Sort scores in descending order and keep only the top 10
	std::sort(scores.begin(), scores.end(), [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
		return b.second < a.second;
		});
	if (scores.size() > 10)
	{
		scores.resize(10);
	}

	// Create the JSON array
	Value scoresArray(kArrayType);
	for (const auto& scorePair : scores)
	{
		Value scoreObject(kObjectType);
		scoreObject.AddMember("Name", StringRef(scorePair.first.c_str()), allocator);
		scoreObject.AddMember("Score", scorePair.second, allocator);
		scoresArray.PushBack(scoreObject, allocator);
	}

	// Add the array to the document
	document.RemoveAllMembers();
	document.AddMember("Scores", scoresArray, allocator);

	// Write the updated document to the JSON file
	std::ofstream ofs("PlayerScores.json");
	if (ofs.is_open())
	{
		OStreamWrapper osw(ofs);
		Writer<OStreamWrapper> writer(osw);
		document.Accept(writer);
		ofs.close();
	}
	else
	{
		std::cerr << "Could not open file for writing!" << std::endl;
	}
}