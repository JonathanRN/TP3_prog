#pragma once
#include "SceneTitre.h"
#include "SceneCombat.h"
#include <SFML/Graphics.hpp>

using namespace sf;

namespace tp3
{
	class Game
	{
	public:
		Game();
		int run();
		int testTest();

	private:
		const int LARGEUR = 1280;
		const int HAUTEUR = 720;
		int yolo;
		bool init();

		RenderWindow mainWin;
		Event event;
	};
}