#include "NameSelectionCP.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "RenderComponent.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "Command.h"

NameSelectionCP::NameSelectionCP(engine::GameObject* pOwner, const glm::vec3& startPosition)
    : Component("NameSelectionCP", pOwner),
    m_CurrentLetterIndex{ 0 },
    m_CurrentSelection{ 0 },
    m_NameConfirmed{ false },
    m_StartPosition{ startPosition }
{
    m_Letters = { 'A', 'A', 'A' };

    auto galaga_Font = engine::ResourceManager::GetInstance().LoadFont("Fonts/Emulogic-zrEw.ttf", 17);
    SDL_Color yellowColor = { 255, 255, 0 };

    glm::vec3 letterPos = m_StartPosition;
    for (size_t i = 0; i < 3; ++i)
    {
        auto letterGO = new engine::GameObject(nullptr, "UI", letterPos);
        letterGO->AddComponent<engine::RenderComponent>(letterGO);
        letterGO->AddComponent<TextComponent>(letterGO, std::string(1, m_Letters[i]), galaga_Font, yellowColor);
        m_LetterObjects.push_back(letterGO);
        letterPos.x += 50.f;
    }
}

NameSelectionCP::~NameSelectionCP()
{
}

void NameSelectionCP::Update(const float)
{
    for (size_t i = 0; i < 3; ++i)
    {
        auto textComp = m_LetterObjects[i]->GetComponent<TextComponent>();
        if (textComp)
        {
            textComp->SetText(std::string(1, m_Letters[i]));
        }
    }
}

void NameSelectionCP::ReceiveMessage(const std::string&, const std::string&)
{
}

std::string NameSelectionCP::GetEnteredName() const
{
    return std::string(m_Letters.begin(), m_Letters.end());
}

bool NameSelectionCP::IsNameConfirmed() const
{
    return m_NameConfirmed;
}

void NameSelectionCP::CycleLetter(int direction)
{
    m_Letters[m_CurrentSelection] = static_cast<char>((m_Letters[m_CurrentSelection] - 'A' + direction + 26) % 26 + 'A');
}

void NameSelectionCP::MoveSelection(int direction)
{
    m_CurrentSelection = (m_CurrentSelection + direction + 3) % 3;
}

void NameSelectionCP::ConfirmName()
{
    m_NameConfirmed = true;
}