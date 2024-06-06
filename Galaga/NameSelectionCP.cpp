#include "NameSelectionCP.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "RenderComponent.h"
#include "ResourceManager.h"
#include "Servicelocator.h"
#include "SoundIDs.h"

NameSelectionCP::NameSelectionCP(engine::GameObject* pOwner)
    : Component("NameSelectionCP", pOwner),
    m_CurrentLetterIndex{ 0 },
    m_CurrentSelection{ 0 },
    m_NameConfirmed{ false },
    m_ArrowObject{ nullptr }
{
    m_Letters = { 'A', 'A', 'A' };
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

    if (m_ArrowObject)
    {
        glm::vec3 arrowPos = m_LetterObjects[m_CurrentSelection]->GetWorldPosition();
        arrowPos.x -= 165.f;
        arrowPos.y -= 180.f;
        m_ArrowObject->GetComponent<engine::TransformComponent>()->SetLocalPosition(arrowPos);
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
    // Play confirmed sound to let the player know it has been applied
    auto& soundSystem = engine::Servicelocator::Get_Sound_System();
    soundSystem.PlaySound(short(Sounds::confirm));
    m_NameConfirmed = true;
}

void NameSelectionCP::AddLetterObject(engine::GameObject* letterObject)
{
    m_LetterObjects.push_back(letterObject);
}

void NameSelectionCP::SetArrowObject(engine::GameObject* arrowObject)
{
    m_ArrowObject = arrowObject;
}

const std::vector<char>& NameSelectionCP::GetLetters() const
{
    return m_Letters;
}