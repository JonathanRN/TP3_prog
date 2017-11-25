#include "Enemy1.h"
using namespace tp3;


Enemy1::Enemy1(Vector2f position, Texture &texture):Enemy(position, texture)
{
	setScale(getScale().x * 0.8, getScale().y * 0.8);
	setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
	dommage = 3;
	vitesse = 6;
	ptsVie = 3;
}


Enemy1::~Enemy1()
{
}

void Enemy1::action(Vaisseau& cible)
{
	float cibleX = cible.getPosition().x;
	float cibleY = cible.getPosition().y;
	float enemyX = getPosition().x;
	float enemyY = getPosition().y;
	float distance = sqrt(pow(cibleX - enemyX, 2) + pow(cibleY - enemyY, 2));

	// Gestion de l'angle du zombie
	angle = (atanf((cibleY - enemyY) / (cibleX - enemyX)));

	if (cibleX < enemyX)
	{
		angle = angle + M_PI;
	}

	// Rotation du sprite
	setRotation(angle*180 / M_PI);

	setPosition(enemyX + (cos(angle) * vitesse), enemyY + (sin(angle) * vitesse));
}
