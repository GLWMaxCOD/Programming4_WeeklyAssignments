#include "NameSelectionCP.h"
#include "GameObject.h"
#include "TextComponent.h"

NameSelectionCP::NameSelectionCP(engine::GameObject* pOwner)
    : Component("NameSelectionCP", pOwner),
    m_CurrentSelection{ 0 },
    m_NameConfirmed{ false }
{
    m_Letters = { 'A', 'A', 'A' };
}

NameSelectionCP::~NameSelectionCP()
{
}

void NameSelectionCP::Update(const float)
{
    for (size_t i = 0; i < m_Letters.size(); ++i)
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

void NameSelectionCP::AddLetterObject(engine::GameObject* letterObject)
{
    m_LetterObjects.push_back(letterObject);
}

std::vector<char> NameSelectionCP::GetLetters() const
{
    return m_Letters;
}

size_t NameSelectionCP::GetCurrentSelection() const
{
    return m_CurrentSelection;
}