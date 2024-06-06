#ifndef GALAGA_NAMESELECTIONCP
#define GALAGA_NAMESELECTIONCP

#include <Component.h>
#include <string>
#include <vector>

namespace engine
{
    class GameObject;
}

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

    std::vector<char> GetLetters() const;
    size_t GetCurrentSelection() const;

private:
    std::vector<char> m_Letters;
    size_t m_CurrentSelection;
    bool m_NameConfirmed;
    std::vector<engine::GameObject*> m_LetterObjects;
};

#endif // GALAGA_NAMESELECTIONCP