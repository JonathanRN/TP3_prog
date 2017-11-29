#include "SceneCombat.h"

using namespace tp3;

SceneCombat::SceneCombat():fond(LARGEUR_ECRAN, HAUTEUR_ECRAN, 5), thrust(1), mouvementJoueur(0, 0)
{
	for (int i = 0; i < NBR_PROJ; i++)
	{
		projectiles[i] = nullptr;
	}
	for (int i = 0; i < NBR_PROJ; i++)
	{
		projectilesEnemy[i] = nullptr;
	}
	for (int i = 0; i < NBR_BONUS; i++)
	{
		bonus[i] = nullptr;
	}
}


SceneCombat::~SceneCombat()
{
}

Scene::scenes SceneCombat::run()
{
	while (isRunning)
	{
		getInputs();
		update();
		draw();
	}
	for (int i = 0; i < NBR_PROJ; i++)
	{
		if (projectiles[i] != nullptr)
		{
			delete projectiles[i];
		}
		if (projectilesEnemy[i] != nullptr)
		{
			delete projectilesEnemy[i];
		}
	}
	for (int i = 0; i < NBR_BONUS; i++)
	{
		if (bonus[i] != nullptr)
		{
			delete bonus[i];
		}
	}
	for (int i = 0; i < ennemis.size(); i++)
	{
		if (ennemis[i] != nullptr)
		{
			delete ennemis[i];
		}
	}
	
	
	
	return transitionVersScene;
}

bool SceneCombat::init(RenderWindow * const window)
{
	srand(time(NULL));
	if (!fond.setTexture("Ressources\\background.jpg"))
	{
		return false;
	}
	if (!projectileT[0].loadFromFile("Ressources\\Projectile_normal.png"))
	{
		return false;
	}
	if (!projectileEnemy.loadFromFile("Ressources\\Projectile_enemy2.png"))
	{
		return false;
	}
	if (!bonusT[0].loadFromFile("Ressources\\Bonus_shield.png"))
	{
		return false;
	}
	if (!player.loadFromFile("Ressources\\Player.png"))
	{
		return false;
	}
	if (!vaisseauJoueur.shield.loadFromFile("Ressources\\Shield.png"))
	{
		return false;
	}
	if (!ennemisT[0].loadFromFile("Ressources\\enemy1.png"))
	{
		return false;
	}
	if (!ennemisT[1].loadFromFile("Ressources\\enemy2.png"))
	{
		return false;
	}
	if (!ennemisT[2].loadFromFile("Ressources\\enemy3.png"))
	{
		return false;
	}
	
	vaisseauJoueur.setTexture(player);
	vaisseauJoueur.setPosition(100, 100);
	vaisseauJoueur.initGraphiques();

	ennemisSuivants.reserve(NBR_ENEMY);

	ennemis.push_back(new Enemy1({ LARGEUR_ECRAN + 100, 100 }, ennemisT[0], choixCouleur()));
	ennemis.push_back(new Enemy2({ LARGEUR_ECRAN + 300, 300 }, ennemisT[1], choixCouleur()));
	ennemis.push_back(new Enemy2({ -200, 300 }, ennemisT[1], choixCouleur()));
	ennemis.push_back(new Enemy3({ -237, 600 }, ennemisT[2], choixCouleur()));



	this->mainWin = window;
	isRunning = true;
	return true;
}

void SceneCombat::getInputs()
{
	while (mainWin->pollEvent(event))
	{
		//x sur la fen�tre
		if (event.type == Event::Closed)
		{
			//Attention, ici simplement fermer la fen�tre ne mettrait pas n�cessairement 
			//fin � l'application
			isRunning = false;
			transitionVersScene = Scene::scenes::SORTIE;
		}
	}

	//Tirer
	if (Keyboard::isKeyPressed(Keyboard::X))
	{
		if (clock_tirer.getElapsedTime().asMilliseconds() >= 100)
		{
			ajouterProjectile(vaisseauJoueur.getPosition());
			clock_tirer.restart();
		}
	}

	//Mouvements gauche et droite
	if (Keyboard::isKeyPressed(Keyboard::Left))
	{
		mouvementJoueur.x = -1;
	}
	else if (Keyboard::isKeyPressed(Keyboard::Right))
	{
		mouvementJoueur.x = 1;
	}
	else
	{
		mouvementJoueur.x = 0;
	}

	//Mouvement haut et bas
	if (Keyboard::isKeyPressed(Keyboard::Up))
	{
		mouvementJoueur.y = -1;
		vaisseauJoueur.monter();
	}
	else if (Keyboard::isKeyPressed(Keyboard::Down))
	{
		mouvementJoueur.y = 1;
		vaisseauJoueur.descendre();
	}
	else
	{
		mouvementJoueur.y = 0;
		vaisseauJoueur.centrer();
	}

	//Pivoter
	if (Keyboard::isKeyPressed(Keyboard::Space))
	{
		if (clock_pivoter.getElapsedTime().asSeconds() >= 1)
		{
			vaisseauJoueur.pivoter();
			clock_pivoter.restart();
		}
	}
		
}

void SceneCombat::update()
{
	fond.move(thrust);
	vaisseauJoueur.mouvementJoueur(mouvementJoueur);
	collisionProjectilesEnnemis();
	collisionVaisseauEnnemis();
	gererBonus();
	gererBoucliers();
	gererEnnemis();
	gererProjectiles();
}

void SceneCombat::draw()
{
	mainWin->clear();
	fond.draw(mainWin);
	for (int i = 0; i < NBR_PROJ; i++)
	{
		if (projectiles[i] != nullptr)
		{
			mainWin->draw(*projectiles[i]);
		}
		if (projectilesEnemy[i] != nullptr)
		{
			mainWin->draw(*projectilesEnemy[i]);
		}
	}
	for (int i = 0; i < ennemis.size(); i++)
	{
		if (ennemis[i] != nullptr)
		{
			mainWin->draw(*ennemis[i]);
		}
	}

	for (int i = 0; i < NBR_BONUS; i++)
	{
		if (bonus[i] != nullptr)
		{
			mainWin->draw(*bonus[i]);
		}
	}	
	if (vaisseauJoueur.shields.size() > 0)
	{
		mainWin->draw(*vaisseauJoueur.shields.top());
	}
	

	mainWin->draw(vaisseauJoueur);
	mainWin->draw(testText);
	mainWin->display();
}

void tp3::SceneCombat::ajouterProjectile(Vector2f position)
{
	for (int i = 0; i < NBR_PROJ; i++)
	{
		if (projectiles[i] == nullptr)
		{
			projectiles[i] = new Projectile_normal(Vector2f(position.x, position.y), 20 * vaisseauJoueur.direction, projectileT[0]);
			projectiles[i]->initGraphiques();
			projectiles[i]->activer();
			return;
		}
	}
}

void SceneCombat::ajouterBonus(Vector2f position)
{
	for (int i = 0; i < NBR_BONUS; i++)
	{
		if (bonus[i] == nullptr)
		{
			int choixBonus = rand() % 2;
			if (choixBonus == 0)
			{
				bonus[i] = new BonusShield(position, bonusT[0]);
				bonus[i]->ajouterObservateur(&vaisseauJoueur);
				return;
			}
			if (choixBonus == 1)
			{
				bonus[i] = new Bombe(position, ennemisT[2]);
				bonus[i]->ajouterObservateur(&vaisseauJoueur);
				for (size_t i = 0; i < ennemis.size(); i++)
				{
					/*if (ennemis[i] != nullptr)
					{
						bonus[i]->ajouterObservateur(ennemis[i]);
					}*/
				}
				return;
			}
		}
	}
}

void SceneCombat::ajouterProjectileEnnemis(Vector2f position, Color color ,int direction)
{
	for (int j = 0; j < NBR_PROJ; j++)
	{
		if (projectilesEnemy[j] == nullptr)
		{
			projectilesEnemy[j] = new Projectile_Enemy(Vector2f(position.x, position.y), 20 * -direction, projectileEnemy, color,direction);
			projectilesEnemy[j]->activer();
			return;
		}
	}
}

Color SceneCombat::choixCouleur()
{
	Color couleur;
	static int choix;
	choix = rand() % 3;

	switch (choix)
	{
	case 0:
		couleur = Color::Red;
		break;
	case 1:
		couleur = Color::Magenta;
		break;
	case 2:
		couleur = Color::Yellow;
		break;
	}
	return couleur;
}

void tp3::SceneCombat::retObservateur(Enemy* observateur)
{
	for (size_t i = 0; i < NBR_BONUS; i++)
	{
		if (bonus[i] != nullptr)
		{
			bonus[i]->retirerObservateur(observateur);
		}
	}
}

void tp3::SceneCombat::collisionProjectilesEnnemis()
{
	for (int i = 0; i < ennemis.size(); i++)
	{
		if (ennemis[i] != nullptr)
		{
			for (int j = 0; j < NBR_PROJ; j++)
			{
				if (projectiles[j] != nullptr)
				{
					if (ennemis[i]->getGlobalBounds().intersects(projectiles[j]->getGlobalBounds()))
					{
						ennemis[i]->ptsVie--;
						delete projectiles[j];
						projectiles[j] = nullptr;
					}
				}
			}
		}
	}
	for (int i = 0; i < NBR_PROJ; i++)
	{
		if (projectilesEnemy[i] != nullptr)
		{
			if (projectilesEnemy[i]->getGlobalBounds().intersects(vaisseauJoueur.getGlobalBounds()))
			{
				if (vaisseauJoueur.shields.size() > 0)
				{
					if (vaisseauJoueur.shields.top()->getColor() != projectilesEnemy[i]->getColor())
					{
						vaisseauJoueur.shields.top()->ptsShield--;
					}
				}
				delete projectilesEnemy[i];
				projectilesEnemy[i] = nullptr;
			}
		}
	}
}

void tp3::SceneCombat::collisionVaisseauEnnemis()
{
	for (int i = 0; i < ennemis.size(); i++)
	{
		if (ennemis[i] != nullptr)
		{
			if (vaisseauJoueur.getGlobalBounds().intersects(ennemis[i]->getGlobalBounds()))
			{
				vaisseauJoueur.ptsVie -= ennemis[i]->dommageCollision;
				retObservateur(ennemis[i]);
				delete ennemis[i];
				ennemis[i] = nullptr;
			}
		}
	}
}

void tp3::SceneCombat::gererBoucliers()
{
	if (vaisseauJoueur.shields.size() > 0)
	{
		if (vaisseauJoueur.shields.top()->ptsShield <= 0)
		{
			vaisseauJoueur.shields.pop();
			cout << "shield miel" << endl;
		}
		if (vaisseauJoueur.shields.size() > 0)
		{
			vaisseauJoueur.shields.top()->setPosition(vaisseauJoueur.getPosition());
		}
	}
	
}

void tp3::SceneCombat::gererProjectiles()
{
	for (size_t i = 0; i < NBR_PROJ; i++)
	{
		if (projectiles[i] != nullptr)
		{
			projectiles[i]->anim(vaisseauJoueur.direction);
			projectiles[i]->move(projectiles[i]->vitesse, 0);
			if (projectiles[i]->getPosition().x > LARGEUR_ECRAN || projectiles[i]->getPosition().x < 0)
			{
				delete projectiles[i];
				projectiles[i] = nullptr;
			}
			
		}
		if (projectilesEnemy[i] != nullptr)
		{
			projectilesEnemy[i]->move(projectilesEnemy[i]->vitesse, 0);
			if (projectilesEnemy[i]->getPosition().x > LARGEUR_ECRAN || projectilesEnemy[i]->getPosition().x < 0)
			{
				delete projectilesEnemy[i];
				projectilesEnemy[i] = nullptr;
			}
		}
	}
}

void tp3::SceneCombat::gererEnnemis()
{
	nbEnnemis();
	for (int i = 0; i < ennemis.size(); i++)
	{
		if (ennemis[i] != nullptr)
		{
			ennemis[i]->action(vaisseauJoueur);
			if (typeid(*ennemis[i]) == typeid(Enemy2))
			{
				if (clock_tire_enemy2.getElapsedTime().asMilliseconds() >= 400 && ennemis[i]->isReady)
				{
					ajouterProjectileEnnemis(ennemis[i]->getPosition(), ennemis[i]->getColor(), ennemis[i]->direction);
					if (i == nbEnemy2)
					{
						clock_tire_enemy2.restart();
					}
				}

			}
			//Si la vie est a 0, detruit l'ennemi
			if (ennemis[i]->ptsVie <= 0)
			{
				//Spawn bonus quand il meurt
				int spwnBonus = rand() % 100;
				if (spwnBonus <= 90)
				{
					ajouterBonus(ennemis[i]->getPosition());
				}
				retObservateur(ennemis[i]);
				delete ennemis[i];
				ennemis[i] = nullptr;
			}
		}
	}
}

void tp3::SceneCombat::gererBonus()
{
	for (int i = 0; i < NBR_BONUS; i++)
	{
		if (bonus[i] != nullptr)
		{
			if (vaisseauJoueur.getGlobalBounds().intersects(bonus[i]->getGlobalBounds()))
			{
				bonus[i]->notifierTousLesObservateurs();
				delete bonus[i];
				bonus[i] = nullptr;
				//Cree un bouclier
				//vaisseauJoueur.shields.push(new Shield(vaisseauJoueur.getPosition(), vaisseauJoueur.shield));

			}
		}
	}
}

void SceneCombat::nbEnnemis()
{
	int temp1=0, temp2=0, temp3=0, temp4=0;
	for (int i = 0; i < ennemis.size(); i++)
	{
		if (ennemis[i] != nullptr)
		{
			if (typeid(*ennemis[i]) == typeid(Enemy1))
			{
				temp1++;
			}
			else if (typeid(*ennemis[i]) == typeid(Enemy2))
			{
				temp2++;
			}
			if (typeid(*ennemis[i]) == typeid(Enemy3))
			{
				temp3++;
			}
		}
	}
	nbEnemy1 = temp1;
	nbEnemy2 = temp2;
	nbEnemy3 = temp3;
}

void SceneCombat::chargerNiveau(const int index)
{
	if (index == 1)
	{
		/*ennemis.push_back(new Enemy1({ LARGEUR_ECRAN + 100, 100 }, ennemisT[0], choixCouleur()));
		ennemis.push_back(new Enemy2({ LARGEUR_ECRAN + 300, 300 }, ennemisT[1], choixCouleur()));
		ennemis.push_back(new Enemy2({ -200, 300 }, ennemisT[1], choixCouleur()));
		ennemis.push_back(new Enemy3({ -237, 600 }, ennemisT[2], choixCouleur()));*/
		ennemisSuivants.push_back(new Enemy1({ LARGEUR_ECRAN + 100, 100 }, ennemisT[0], choixCouleur()));
	}
}
