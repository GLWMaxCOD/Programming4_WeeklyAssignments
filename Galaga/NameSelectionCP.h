#ifndef GALAGA_NAMESELECTIONCP
#define GALAGA_NAMESELECTIONCP

#include <Component.h>
#include <string>
#include <vector>
#include "glm/vec3.hpp"

class NameSelectionCP final : public engine::Component
{
public:
    NameSelectionCP(engine::GameObject* pOwner);
    virtual ~NameSelectionCP() override;

    virtual void Update(const float deltaTime) override;
    virtual void ReceiveMessage(const std::string& message, const std::string& value) override;

    std::string GetEnteredName() const;
    bool IsNameConfirmed() const;
    void CycleLetter(int direction);
    void MoveSelection(int direction);
    void ConfirmName();
    void AddLetterObject(engine::GameObject* letterObject);
    void SetArrowObject(engine::GameObject* arrowObject);
    const std::vector<char>& GetLetters() const;

private:
    std::vector<char> m_Letters;
    size_t m_CurrentLetterIndex;
    size_t m_CurrentSelection;
    bool m_NameConfirmed;

    std::vector<engine::GameObject*> m_LetterObjects;
    engine::GameObject* m_ArrowObject;
};

#endif // GALAGA_NAMESELECTIONCP