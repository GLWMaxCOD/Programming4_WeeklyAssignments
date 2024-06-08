#include "MenuSelectionCommand.h"
#include "MenuSelectionCP.h"

MenuSelectionCommand::MenuSelectionCommand(MenuSelectionCP* pMenuSelectionCP, int menuAction)
	:Command(),
	m_pMenuSelection{ pMenuSelectionCP },
	m_MenuAction{ menuAction }
{
	// Constructor: Initialize with a menu selection component and an action
}

MenuSelectionCommand::~MenuSelectionCommand()
{
	// Destructor
}

void MenuSelectionCommand::Execute(float)
{
	if (m_pMenuSelection != nullptr)
	{
		if (m_MenuAction != 0)
		{
			// Change to the next or previous selection
			m_pMenuSelection->ChangeSelection(m_MenuAction);
			return;
		}

		// Player wants to select this option
		m_pMenuSelection->SetOptionSelected(true);
	}
}