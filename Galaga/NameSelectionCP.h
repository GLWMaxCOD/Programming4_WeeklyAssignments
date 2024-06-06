#ifndef GALAGA_NAMESELECTIONCP
#define GALAGA_NAMESELECTIONCP

#include <Component.h>
#include <string>
#include <vector>
#include "glm/vec3.hpp"

class NameSelectionCP final : public engine::Component
{
public:

    NameSelectionCP(engine::GameObject* pOwner, const glm::vec3& startPosition);
    virtual ~NameSelectionCP() override;

    virtual void Update(const float deltaTime) override;
    virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

    std::string GetEnteredName() const;
    bool IsNameConfirmed() const;

    void CycleLetter(int direction);
    void MoveSelection(int direction);
    void ConfirmName();

private:

    std::vector<char> m_Letters;
    size_t m_CurrentLetterIndex;
    size_t m_CurrentSelection;
    bool m_NameConfirmed;

    std::vector<engine::GameObject*> m_LetterObjects;
    glm::vec3 m_StartPosition;
};

#endif // GALAGA_NAMESELECTIONCP