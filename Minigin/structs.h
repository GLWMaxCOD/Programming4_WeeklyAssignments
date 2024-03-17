#pragma once

#ifndef GAME_ENGINE_STRUCTS
#define GAME_ENGINE_STRUCTS

#include <string>

struct Window
{
	explicit Window(const std::string& title = "Title", float width = 320.0f,
		float height = 180.0f);

	std::string title;
	float width;
	float height;

};


#endif