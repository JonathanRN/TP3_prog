#pragma once
#include "Enemy.h"
namespace tp3
{
	class Enemy3 : public Enemy
	{
	public:
		Enemy3(Vector2f position, Texture &texture);
		~Enemy3();
	};
}
