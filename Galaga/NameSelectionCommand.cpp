#include "NameSelectionCommand.h"
#include "NameSelectionCP.h"

// Constructor to initialize the command with the name selection component and action
NameSelectionCommand::NameSelectionCommand(NameSelectionCP* pNameSelectionCP, Action action)
    : Command(),
    m_pNameSelection{ pNameSelectionCP },
    m_Action{ action }
{

}

// Destructor
NameSelectionCommand::~NameSelectionCommand()
{

}

// Execute the appropriate action based on the command
void NameSelectionCommand::Execute(float)
{
    if (m_pNameSelection != nullptr)
    {
        switch (m_Action)
        {
        case Action::CycleUp:
            m_pNameSelection->CycleLetter(1);
            break;
        case Action::CycleDown:
            m_pNameSelection->CycleLetter(-1);
            break;
        case Action::MoveLeft:
            m_pNameSelection->MoveSelection(-1);
            break;
        case Action::MoveRight:
            m_pNameSelection->MoveSelection(1);
            break;
        case Action::Confirm:
            m_pNameSelection->ConfirmName();
            break;
        default:
            break;
        }
    }
}