#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;
namespace tp3
{
	class Bonus : public Sprite
	{
	public:
		Bonus(Vector2f position,Texture& texture);
		~Bonus();
	};
}

