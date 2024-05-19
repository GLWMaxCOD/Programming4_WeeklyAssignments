#include "MenuSelectionCommand.h"
#include "MenuSelectionCP.h"
#include <iostream>

MenuSelectionCommand::MenuSelectionCommand(MenuSelectionCP* pMenuSelectionCP, int menuAction)
	:Command(),
	m_pMenuSelection{ pMenuSelectionCP },
	m_MenuAction{ menuAction }
{

}

MenuSelectionCommand::~MenuSelectionCommand()
{
	std::cout << "MenuSelectionCommand destructor" << std::endl;
}

void MenuSelectionCommand::Execute(float)
{
	if (m_pMenuSelection != nullptr)
	{
		if (m_MenuAction != 0)
		{
			// Either go to the next or previous selection
			m_pMenuSelection->ChangeSelection(m_MenuAction);
			return;
		}

		// Player wants to select this option
		m_pMenuSelection->SetOptionSelected(true);
	}
}