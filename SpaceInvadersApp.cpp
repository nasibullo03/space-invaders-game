#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <ctime>

using namespace sf;
using namespace std;

struct WindowSize{
	int width;
	int height;
};

WindowSize WinSize = {1000,560};
vector <WindowSize> WSize = { WinSize };

struct Bullet {
	float weaponPositionX;
	float weaponPositionY;
	string direction; 
	bool bullet;
};

vector<Bullet> BulletList;

struct Invaders {
	float invaderPositionX;
	float invaderPositionY;
	int invidersIndex;
	int speed;
	int directionOfMovig;
	bool life;
};

vector <Invaders> Level1;

int main()
{
	bool fire = false, weaponFire = true, leftFireRotation = false, 
		rightFireRotation = false, clickOnKeyboardUP = false, 
		first = true, hitTheTarget=false, endOfProgram=false, break1=false, break2=false;

	int intweaponFire = 0, invider = 0;
	int intBullet = 0, bullEndPositionY, step = 0, speed = 0;
	float weaponX, weaponY, invidersSpeed = 1;;
	
	srand(time_t(NULL));
	//структура Level 1
	Level1.push_back({50,20,   rand() % 9,0, rand() % 2,true});
	Level1.push_back({ 150,140, rand() % 9,0, rand() % 2,true });
	Level1.push_back({ 250,20, rand() % 9,0, rand() % 2,true });
	Level1.push_back({ 350,140, rand() % 9,0, rand() % 2,true });
	Level1.push_back({ 450,20, rand() % 9,0, rand() % 2,true });
	Level1.push_back({ 550,140, rand() % 9,0, rand() % 2,true });
	Level1.push_back({ 650,20, rand() % 9,0, rand() % 2,true });
	Level1.push_back({ 750,140, rand() % 9,0, rand() % 2,true });
	Level1.push_back({ 850,20, rand() % 9,0, rand() % 2,true });
	
	/*Level1[0] = {};
	Level1[1] = {};
	Level1[2] = {};
	Level1[3] = {};
	Level1[4] = {};
	Level1[5] = {};
	Level1[6] = {};
	Level1[7] = {};
	Level1[8] = {};
	*/

	RenderWindow window(sf::VideoMode(WSize[0].width, WSize[0].height), "SFML Application", Style::Close);

	Image bgImage, weaponImg, fireImg, bulletImg;
	bgImage.loadFromFile("images/bg.png");
	weaponImg.loadFromFile("images/weapon.png");
	fireImg.loadFromFile("images/fire.png");
	bulletImg.loadFromFile("images/bullet.png");

	Image invaders[9];
	invaders[0].loadFromFile("images/inviders/100px/0.png");
	invaders[1].loadFromFile("images/inviders/100px/1.png");
	invaders[2].loadFromFile("images/inviders/100px/2.png");
	invaders[3].loadFromFile("images/inviders/100px/3.png");
	invaders[4].loadFromFile("images/inviders/100px/4.png");
	invaders[5].loadFromFile("images/inviders/100px/5.png");
	invaders[6].loadFromFile("images/inviders/100px/6.png");
	invaders[7].loadFromFile("images/inviders/100px/7.png");
	invaders[8].loadFromFile("images/inviders/100px/8.png");

	Texture invidersTexture[9];
	invidersTexture[0].loadFromImage(invaders[0]);
	invidersTexture[1].loadFromImage(invaders[1]);
	invidersTexture[2].loadFromImage(invaders[2]);
	invidersTexture[3].loadFromImage(invaders[3]);
	invidersTexture[4].loadFromImage(invaders[4]);
	invidersTexture[5].loadFromImage(invaders[5]);
	invidersTexture[6].loadFromImage(invaders[6]);
	invidersTexture[7].loadFromImage(invaders[7]);
	invidersTexture[8].loadFromImage(invaders[8]);

	Sprite invidersSprite[9];
	invidersSprite[0].setTexture(invidersTexture[0]);
	invidersSprite[1].setTexture(invidersTexture[1]);
	invidersSprite[2].setTexture(invidersTexture[2]);
	invidersSprite[3].setTexture(invidersTexture[3]);
	invidersSprite[4].setTexture(invidersTexture[4]);
	invidersSprite[5].setTexture(invidersTexture[5]);
	invidersSprite[6].setTexture(invidersTexture[6]);
	invidersSprite[7].setTexture(invidersTexture[7]);
	invidersSprite[8].setTexture(invidersTexture[8]);

	/*invidersSprite[0].setScale(100.0f / invidersSprite[0].getLocalBounds().width, 100.0f / invidersSprite[0].getLocalBounds().height);
	invidersSprite[1].setScale(100.0f / invidersSprite[1].getLocalBounds().width, 100.0f / invidersSprite[1].getLocalBounds().height);
	invidersSprite[2].setScale(100.0f / invidersSprite[2].getLocalBounds().width, 100.0f / invidersSprite[2].getLocalBounds().height);
	invidersSprite[3].setScale(100.0f / invidersSprite[3].getLocalBounds().width, 100.0f / invidersSprite[3].getLocalBounds().height);
	invidersSprite[4].setScale(100.0f / invidersSprite[4].getLocalBounds().width, 100.0f / invidersSprite[4].getLocalBounds().height);
	invidersSprite[5].setScale(100.0f / invidersSprite[5].getLocalBounds().width, 100.0f / invidersSprite[5].getLocalBounds().height);
	invidersSprite[6].setScale(100.0f / invidersSprite[6].getLocalBounds().width, 100.0f / invidersSprite[6].getLocalBounds().height);
	invidersSprite[7].setScale(100.0f / invidersSprite[7].getLocalBounds().width, 100.0f / invidersSprite[7].getLocalBounds().height);
	invidersSprite[8].setScale(100.0f / invidersSprite[8].getLocalBounds().width, 100.0f / invidersSprite[8].getLocalBounds().height);
	*/

	Texture bgTexture, weaponTxre, fireTxre, bulletTxre;
	bgTexture.loadFromImage(bgImage);
	weaponTxre.loadFromImage(weaponImg);
	fireTxre.loadFromImage(fireImg);
	bulletTxre.loadFromImage(bulletImg);

	Sprite bgSprite, weaponSprite, fireSprite, bulletSprite;
	bgSprite.setTexture(bgTexture);
	bgSprite.setPosition(0, 0);
	weaponSprite.setTexture(weaponTxre);
	weaponSprite.setPosition(float(WSize[0].width/2-75), float(WSize[0].height -150));
	weaponSprite.setScale(
		150.0f / weaponSprite.getLocalBounds().width,
		150.0f / weaponSprite.getLocalBounds().height
		);
	fireSprite.setTexture(fireTxre);
	bulletSprite.setTexture(bulletTxre);

	bulletSprite.setTextureRect(IntRect(339, 212, 26, 26));
	bulletSprite.setPosition(weaponSprite.getPosition().x + 67, weaponSprite.getPosition().y + 50);
	bulletSprite.setScale(
		15.0f / bulletSprite.getLocalBounds().width,
		15.0f / bulletSprite.getLocalBounds().height
	);
	

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) {
				endOfProgram = true;
				window.close();
			}
				
		}


		if (Keyboard::isKeyPressed(Keyboard::Left)||
			Keyboard::isKeyPressed(Keyboard::A) || 
			Keyboard::isKeyPressed(Keyboard::Num4) ||
			Keyboard::isKeyPressed(Keyboard::Numpad4)) {
			if (!clickOnKeyboardUP) {
				if (weaponSprite.getPosition().x >= 0) {
					weaponSprite.move(float(-0.4), float(0));
					fireSprite.move(float(-0.4), float(0));
					leftFireRotation = true;
				}
			}

		}
		else
		{
			leftFireRotation = false;
		}

		if (Keyboard::isKeyPressed(Keyboard::Right)||
			Keyboard::isKeyPressed(Keyboard::D) || 
			Keyboard::isKeyPressed(Keyboard::Num6) ||
			Keyboard::isKeyPressed(Keyboard::Numpad6)) {
			if (!clickOnKeyboardUP) {
				if (weaponSprite.getPosition().x <= 850) {
					weaponSprite.move(float(0.4), float(0));
					fireSprite.move(float(0.4), float(0));
					rightFireRotation = true;
				}
			}
			

		}
		else {
			rightFireRotation = false;
		}
		if (Keyboard::isKeyPressed(Keyboard::Up) || 
			Keyboard::isKeyPressed(Keyboard::W) || 
			Keyboard::isKeyPressed(Keyboard::Num8) ||
			Keyboard::isKeyPressed(Keyboard::Numpad8)|| 
			Keyboard::isKeyPressed(Keyboard::Space)) {
			fire = true;
			clickOnKeyboardUP = true;
			bullEndPositionY = 15;
			if (first==true) {
				weaponX = weaponSprite.getPosition().x + 67;
				weaponY = weaponSprite.getPosition().y + 50;
				BulletList.push_back({ weaponX, weaponY, "UP",true });
				first = false;
				
			}
			// ком кардаги буд
			else if (BulletList[BulletList.size() - 1].bullet == false) {
				weaponX = weaponSprite.getPosition().x + 67;
				weaponY = weaponSprite.getPosition().y + 50;
				BulletList.push_back({ weaponX, weaponY, "UP",true });
				first = false;
			}
			
		    clickOnKeyboardUP = true;
			if (clickOnKeyboardUP) {
				bulletSprite.setPosition(weaponSprite.getPosition().x + 67, weaponSprite.getPosition().y + 50);
			}
			
			fireSprite.setRotation(0);
			weaponSprite.setRotation(0);
			//;
	

	
			//bullet = true;

		}
		 
		

		if (Keyboard::isKeyPressed(Keyboard::Down)) {
			fire = false;
		}
		
		
		window.clear();
		window.draw(bgSprite);
		if (fire) {
			fireSprite.setTextureRect(IntRect(75, 113, 35, 37));
			fireSprite.setPosition(weaponSprite.getPosition().x + 55, weaponSprite.getPosition().y + 110);
			window.draw(fireSprite);
			fireSprite.setTextureRect(IntRect(75, 113, 35, 37));
			fireSprite.setPosition(weaponSprite.getPosition().x + 55, weaponSprite.getPosition().y+40);
			window.draw(fireSprite);
			
			fire = false;
		}
		
		/*if (clickOnKeyboardUP) {
			if (bulletSprite.getPosition().y<=15) {
				bulletSprite.move(0, -1);
				window.draw(bulletSprite);
			}
			else {
				clickOnKeyboardUP = false;
			}
		}*/
		for (int count = 0; count < BulletList.size(); ++count) {
						
			/*if (BulletList[count].bullet == false) {
				BulletList.erase(BulletList.begin(),count);
		}
			*/
			if (BulletList[count].bullet == true && BulletList[count].weaponPositionY >= 15) {
				if (BulletList[count].weaponPositionY < WSize[0].height - 150) {
					clickOnKeyboardUP = false;
				}
				for (int inviders = 0; inviders < Level1.size(); ++inviders) {

					if(((BulletList[count].weaponPositionX <=Level1[inviders].invaderPositionX + 100) &&
						(BulletList[count].weaponPositionX>= Level1[inviders].invaderPositionX)) && 
						(BulletList[count].weaponPositionY-80 <= Level1[inviders].invaderPositionY)){
						bullEndPositionY = BulletList[count].weaponPositionY;;
						Level1[inviders] = { 0,0,0, 0,false };
						invidersSpeed +=0.8;
						//hitTheTarget = true;
						first = true;
						BulletList[count].bullet = false;
						BulletList.clear();
					//	hitTheTarget = false;
						break;
					}

				}

				if (!hitTheTarget) {
					bulletSprite.setPosition(BulletList[count].weaponPositionX, BulletList[count].weaponPositionY);
					BulletList[count].weaponPositionY -= 8;
					window.draw(bulletSprite);
					break;
				}
				
			}

			if (BulletList[count].weaponPositionY < bullEndPositionY) {
				first = true;
				BulletList[count].bullet = false;
				BulletList.clear();
				hitTheTarget = false;
				break;
			}

				
		}
		
		
		/*if (bullet) {
			if (bulletSprite.getPosition().y >= 15) {
				
				bulletSprite.move(0, -1);
				window.draw(bulletSprite);
			}
		}*/

		if (weaponFire) {
			
			fireSprite.setTextureRect(IntRect(450, 593, 43, 97));
			fireSprite.setPosition(weaponSprite.getPosition().x + 75, weaponSprite.getPosition().y + 75);
			window.draw(fireSprite);
			fireSprite.setPosition(weaponSprite.getPosition().x + 17, weaponSprite.getPosition().y + 75);
			window.draw(fireSprite);
			if (leftFireRotation) {
				fireSprite.setRotation(-10);
			}
			else if (rightFireRotation) {
				fireSprite.setRotation(10);
			}
			weaponFire = false;
		

		}
		else if (!weaponFire) {
			fireSprite.setTextureRect(IntRect(260, 610, 49, 93));
			fireSprite.setPosition(weaponSprite.getPosition().x + 75, weaponSprite.getPosition().y + 75);
			window.draw(fireSprite);
			fireSprite.setPosition(weaponSprite.getPosition().x + 17, weaponSprite.getPosition().y + 75);
			window.draw(fireSprite);
			if (leftFireRotation) {
				fireSprite.setRotation(-5);
				weaponSprite.setRotation(-2);
			}
			else if (rightFireRotation) {
				fireSprite.setRotation(5);
				weaponSprite.setRotation(2);
			}
			else if (!rightFireRotation || !leftFireRotation) {
				fireSprite.setRotation(0);
				weaponSprite.setRotation(0);
			}
			weaponFire = true;
				
		}

		for (int inviders = 0; inviders < Level1.size(); ++inviders) {

			if (Level1[inviders].life) {
				if ((Level1[inviders].speed == 0) || (Level1[inviders].speed == Level1.size() + 5)) {
					
					if (Level1[inviders].directionOfMovig == 0) {
						Level1[inviders].invaderPositionX += invidersSpeed;
						if (Level1[inviders].invaderPositionX >= 850) {
							Level1[inviders].directionOfMovig = 1;
						}
						break1 = false;
						for (int invidersIndexA = 0; invidersIndexA < Level1.size(); ++invidersIndexA) {
							for (int invidersIndexB = 0; invidersIndexB < Level1.size(); ++invidersIndexB) {
								if (invidersIndexA != invidersIndexB) {
									if ((Level1[invidersIndexA].invaderPositionX + 100 == Level1[invidersIndexB].invaderPositionX) &&
										(Level1[invidersIndexA].invaderPositionY == Level1[invidersIndexB].invaderPositionY)) {
										Level1[invidersIndexA].directionOfMovig = 1;
										Level1[invidersIndexB].directionOfMovig = 0;
										break1 = true;
										break;
									}
								}
								
							}
							if (break1) {
								break;
							}
						}
						/*if (Level1[inviders].invaderPositionX + 100 == Level1[inviders + 2].invaderPositionX) {
							Level1[inviders].directionOfMovig = 1;
							Level1[inviders + 2].directionOfMovig = 0;
						}*/
					}
					if (Level1[inviders].directionOfMovig == 1) {
						Level1[inviders].invaderPositionX -= invidersSpeed;;
						if (Level1[inviders].invaderPositionX <= 50) {
							Level1[inviders].directionOfMovig = 0;
						}
						break1 = false;
						for (int invidersIndexA = 0; invidersIndexA < Level1.size(); ++invidersIndexA) {
							for (int invidersIndexB = 0; invidersIndexB < Level1.size(); ++invidersIndexB) {
								if (invidersIndexA != invidersIndexB) {
									if ((Level1[invidersIndexA].invaderPositionX - 100 == Level1[invidersIndexB].invaderPositionX) &&
										(Level1[invidersIndexA].invaderPositionY == Level1[invidersIndexB].invaderPositionY)) {
										Level1[invidersIndexA].directionOfMovig = 0;
										Level1[invidersIndexB].directionOfMovig = 1;
										break1 = true;
										break;
									}
								}

							}
							if (break1) {
								break;
							}
						}
						/*else if (Level1[inviders].invaderPositionX - 100 == Level1[inviders - 2].invaderPositionX) {
							
						}*/
					}
				}
			}

			if (!Level1[inviders].life) {
				continue;
			}
		
			invidersSprite[0].setPosition(Level1[inviders].invaderPositionX, Level1[inviders].invaderPositionY);
			invidersSprite[1].setPosition(Level1[inviders].invaderPositionX, Level1[inviders].invaderPositionY);
			invidersSprite[2].setPosition(Level1[inviders].invaderPositionX, Level1[inviders].invaderPositionY);
			invidersSprite[3].setPosition(Level1[inviders].invaderPositionX, Level1[inviders].invaderPositionY);
			invidersSprite[4].setPosition(Level1[inviders].invaderPositionX, Level1[inviders].invaderPositionY);
			invidersSprite[5].setPosition(Level1[inviders].invaderPositionX, Level1[inviders].invaderPositionY);
			invidersSprite[6].setPosition(Level1[inviders].invaderPositionX, Level1[inviders].invaderPositionY);
			invidersSprite[7].setPosition(Level1[inviders].invaderPositionX, Level1[inviders].invaderPositionY);
			invidersSprite[8].setPosition(Level1[inviders].invaderPositionX, Level1[inviders].invaderPositionY);


			window.draw(invidersSprite[Level1[inviders].invidersIndex]);
			if (Level1[inviders].speed == 0 || Level1[inviders].speed == Level1.size() + 5) {
				++Level1[inviders].invidersIndex;
				if (Level1[inviders].invidersIndex == 9) {
					Level1[inviders].invidersIndex = 0;
				}
			}
			else if (Level1[inviders].speed == 2 * Level1.size() + 10) {
				Level1[inviders].speed = 0;
			}
			++Level1[inviders].speed;
		}
	
		
		
		window.draw(weaponSprite);
		window.display();
	}

	return 0;
}