#include "Vaisseau.h"

using namespace tp3;

Vaisseau::Vaisseau() : image(CENTRE)
{
	isPivoting = false;
}


Vaisseau::~Vaisseau()
{
	
}

//MOUVEMENT QUI SE DIRIGE TOUT SIMPLEMENT VERS LA GAUCHE
void Vaisseau::mouvement(int thrust)
{
	move(-VITESSE - thrust, 0);
}

//MOUVEMENT CONTR�L� PAR LE JOUEUR
void Vaisseau::mouvementJoueur(Vector2i& mouvement)
{
	move(mouvement.x * VITESSE, mouvement.y * VITESSE);
}

//ANIMATION DE LA DESCENTE DU VAISSEAU
void Vaisseau::descendre()
{
	if (image < LIMITE_BAS)
	{
		image++;

		if (image % RHYTME_ANIM == 0)
		{
			rectangleAnimation.left = (rectangleAnimation.width + OFFSET) * (image / RHYTME_ANIM);
			setTextureRect(rectangleAnimation);
		}
	}
}

//ANIMATION DE LA MONT�E DU VAISSEAU
void Vaisseau::monter()
{
	if (image > LIMITE_HAUT)
	{
		image--;

		if (image % RHYTME_ANIM == 9)
		{
			rectangleAnimation.left = (rectangleAnimation.width + OFFSET) * (image / RHYTME_ANIM);
			setTextureRect(rectangleAnimation);
		}
	}
}

//SI ON EST PAS EXACTEMENT AU CENTRE, ON RAM�NE LE VAISSEAU AU CENTRE
void Vaisseau::centrer()
{
	if (!isPivoting)
	{
		if (image == CENTRE)
			return;

		if (image < CENTRE)
		{
			descendre();
		}
		else if (image > CENTRE)
		{
			monter();
		}
	}
}

void Vaisseau::pivoter()
{
	if (!isPivoting)
	{
		image = LIMITE_GAUCHE_PIVOT;
		isPivoting = true;
	}

	while (image != LIMITE_DROITE_PIVOT)
	{
		image++;
		if (image % RHYTME_ANIM == 0)
		{
			rectangleAnimation.left = (rectangleAnimation.width + OFFSET) * (image / RHYTME_ANIM);
			setTextureRect(rectangleAnimation);
		}
	}

	if (image == LIMITE_DROITE_PIVOT)
	{
		isPivoting = false;
		image = CENTRE;
		setScale(getScale().x * -1, getScale().y);
		direction *= -1;
	}
}

void Vaisseau::initGraphiques()
{
	rectangleAnimation.width = (getTextureRect().width / 11) - OFFSET;
	rectangleAnimation.height = getTextureRect().height;
	rectangleAnimation.top = 0;
	rectangleAnimation.left = (rectangleAnimation.width + OFFSET) * (image / RHYTME_ANIM);
	setTextureRect(rectangleAnimation);

	setOrigin(rectangleAnimation.width / 2, rectangleAnimation.height / 2);
}