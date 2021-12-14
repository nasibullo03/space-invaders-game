#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <ctime>
#include <string>

using namespace sf;
using namespace std;

bool fire = false, weaponFire = true, leftFireRotation = false,
rightFireRotation = false, clickOnKeyboardUP = false,
first = true, hitTheTarget = false, endOfProgram = false;

int intweaponFire = 0, backgroundIndex=0;
float weaponX, weaponY, bullEndPositionY, backgroundTimer, invidersTimer, invidersSpeed;

Image bgImage, weaponImg, fireImg, bulletImg, invaders[9], explosionImg[50], background[25];
Texture bgTexture, weaponTxre, fireTxre, bulletTxre,invidersTexture[9], explosionTexture[50], backgroundTexture[25];
Sprite bgSprite, weaponSprite, fireSprite, bulletSprite, invidersSprite[9],  explosionSprite[50], backgroundSprite[25];

struct WindowSize{
	int width;
	int height;
};

WindowSize WinSize = {1000,560};
vector <WindowSize> WSize = { WinSize };

RenderWindow window(sf::VideoMode(WSize[0].width, WSize[0].height), "SFML Application", Style::Close);

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
	bool life;
	int column; 
};

vector <Invaders> Level1;

struct Explosion{
	float explosionPositionX;
	float explosionPositionY;
	bool endOfExplosion;
	float explosionSpeed;
	int explosionImgIndex;
};

vector <Explosion> Explosions;
//удалить взрывающие захватчики из структа вектора 
void RemoveLevel1(std::vector<Invaders> & Level1, bool life) {
	Level1.erase(
		std::remove_if(Level1.begin(), Level1.end(), [&](Invaders const & v) {
		return v.life == false;
	}),
		Level1.end());
}

//импортирование картинки и обработики картинки
void settingsImages() {
	
	//добавл€ем картинки 
	bgImage.loadFromFile("images/bg.png");//добавление background image 
	weaponImg.loadFromFile("images/weapon.png");//добавление оруж€ 
	fireImg.loadFromFile("images/fire.png");//добавление эффект огны
	bulletImg.loadFromFile("images/bullet.png");//добавление пул€
	//добавление захватчики 
	for (int imgIndex = 0; imgIndex < 9; ++imgIndex) {
		invaders[imgIndex].loadFromFile("images/inviders/100px/" + to_string(imgIndex) + ".png");
	}
	//добавление картинки эффекта взрыва (explosion)
	for (int imgIndex = 0; imgIndex < 50; ++imgIndex) {
		explosionImg[imgIndex].loadFromFile("images/explosion/" + to_string(imgIndex) + ".png");
	}

	for (int imgIndex = 0; imgIndex < 25; ++imgIndex) {
		background[imgIndex].loadFromFile("images/bg/" + to_string(imgIndex) + ".jpg");
	}

	//добавл€ем текстура в картинки 
	
	bgTexture.loadFromImage(bgImage);
	weaponTxre.loadFromImage(weaponImg);
	fireTxre.loadFromImage(fireImg);
	bulletTxre.loadFromImage(bulletImg);
	//текстура дл€ захватчики 
	for (int texture = 0; texture < 9; ++texture) {
		invidersTexture[texture].loadFromImage(invaders[texture]);
	}
	//текстура дл€ картинки взрыва 
	for (int texture = 0; texture < 50; ++texture) {
		explosionTexture[texture].loadFromImage(explosionImg[texture]);
	}

	for (int texture = 0; texture < 25; ++texture) {
		backgroundTexture[texture].loadFromImage(background[texture]);
	}

	//добавл€ем спрайт дл€ текстуры

	bgSprite.setTexture(bgTexture);
	bgSprite.setPosition(0, 0);
	weaponSprite.setTexture(weaponTxre);
	weaponSprite.setPosition(float(WSize[0].width / 2 - 75), float(WSize[0].height - 170));//задаем началные позиции оруж€ 
	weaponSprite.setScale(
		150.0f / weaponSprite.getLocalBounds().width,
		150.0f / weaponSprite.getLocalBounds().height
	); //  задем размер дл€ оруж€ 
	fireSprite.setTexture(fireTxre);
	bulletSprite.setTexture(bulletTxre);

	bulletSprite.setTextureRect(IntRect(339, 212, 26, 26));
	bulletSprite.setPosition(weaponSprite.getPosition().x + 67, weaponSprite.getPosition().y + 50);//задаем стартовий позиции оруж€ 
	bulletSprite.setScale(
		15.0f / bulletSprite.getLocalBounds().width,
		15.0f / bulletSprite.getLocalBounds().height
	); // задаем размер пул€


	//спрайт дл€ захватчики 
	for (int sprite = 0; sprite < 9; ++sprite) {
		invidersSprite[sprite].setTexture(invidersTexture[sprite]);
	}

	//спрайт дл€ эффект взрыва
	for (int sprite = 0; sprite < 50; ++sprite) {
		explosionSprite[sprite].setTexture(explosionTexture[sprite]);
		explosionSprite[sprite].setScale(
			200.0f / explosionSprite[sprite].getLocalBounds().width,
			200.0f / explosionSprite[sprite].getLocalBounds().height
		);
	}

	for (int sprite = 0; sprite < 25; ++sprite) {
		backgroundSprite[sprite].setTexture(backgroundTexture[sprite]);
		backgroundSprite[sprite].setTextureRect(IntRect(140, 0, 1000, 560));
		backgroundSprite[sprite].setScale(
			1000.0f / backgroundSprite[sprite].getLocalBounds().width,
			560.0f / backgroundSprite[sprite].getLocalBounds().height
		);
	}

}

//первоначальные захватчики и их свойств
void makeInvides() {
	
	//структура Level 1
	

}

//доступ на клавиатуру
void keyboard(float time) {
	if (Keyboard::isKeyPressed(Keyboard::Left) ||
		Keyboard::isKeyPressed(Keyboard::A) ||
		Keyboard::isKeyPressed(Keyboard::Num4) ||
		Keyboard::isKeyPressed(Keyboard::Numpad4)) {
		if (!clickOnKeyboardUP) {
			if (weaponSprite.getPosition().x >= 0) {
				weaponSprite.move(float(-0.2)*time, float(0));
				fireSprite.move(float(-0.2)*time, float(0));
				leftFireRotation = true;
			}
		}

	}
	else
	{
		leftFireRotation = false;
	}

	if (Keyboard::isKeyPressed(Keyboard::Right) ||
		Keyboard::isKeyPressed(Keyboard::D) ||
		Keyboard::isKeyPressed(Keyboard::Num6) ||
		Keyboard::isKeyPressed(Keyboard::Numpad6)) {
		if (!clickOnKeyboardUP) {
			if (weaponSprite.getPosition().x <= 850) {
				weaponSprite.move(float(0.2)*time, float(0));
				fireSprite.move(float(0.2)*time, float(0));
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
		Keyboard::isKeyPressed(Keyboard::Numpad8) ||
		Keyboard::isKeyPressed(Keyboard::Space)) {
		fire = true;
		clickOnKeyboardUP = true;
		bullEndPositionY = 15;
		if (first == true) {
			weaponX = weaponSprite.getPosition().x + 67;
			weaponY = weaponSprite.getPosition().y + 50;
			BulletList.push_back({ weaponX, weaponY, "UP",true });
			first = false;

		}

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

	}



	if (Keyboard::isKeyPressed(Keyboard::Down)) {
		fire = false;
	}
}

//показать огн€ при стельбе 
void fireWhileShooting() {
	if (fire) {
		fireSprite.setTextureRect(IntRect(75, 113, 35, 37));
		fireSprite.setPosition(weaponSprite.getPosition().x + 55, weaponSprite.getPosition().y + 110);
		window.draw(fireSprite);
		fireSprite.setTextureRect(IntRect(75, 113, 35, 37));
		fireSprite.setPosition(weaponSprite.getPosition().x + 55, weaponSprite.getPosition().y + 40);
		window.draw(fireSprite);

		fire = false;
	}
}

//стерл€т и удалить мертвые захватчики из вектор захватчиков
void bullet(float time) {
	for (int count = 0; count < BulletList.size(); ++count) {

		if (BulletList[count].bullet == true && BulletList[count].weaponPositionY >= 15) {
			if (BulletList[count].weaponPositionY < WSize[0].height - 150) {
				clickOnKeyboardUP = false;
			}
			for (int inviders = 0; inviders < Level1.size(); ++inviders) {

				if (((BulletList[count].weaponPositionX <= Level1[inviders].invaderPositionX + 100) &&
					(BulletList[count].weaponPositionX >= Level1[inviders].invaderPositionX)) &&
					(BulletList[count].weaponPositionY - 80 <= Level1[inviders].invaderPositionY)) {
					bullEndPositionY = BulletList[count].weaponPositionY;;
					Level1[inviders].life = false;
					RemoveLevel1(Level1, false);
					Explosions.push_back({ BulletList[count].weaponPositionX,BulletList[count].weaponPositionY,false,0 });
					first = true;
					BulletList[count].bullet = false;
					BulletList.clear();

					break;
				}

			}

			bulletSprite.setPosition(BulletList[count].weaponPositionX, BulletList[count].weaponPositionY);
			BulletList[count].weaponPositionY -= float(0.8*time);
			window.draw(bulletSprite);
			break;

		}

		if (BulletList[count].weaponPositionY < bullEndPositionY) {
			first = true;
			BulletList[count].bullet = false;
			BulletList.clear();
			hitTheTarget = false;
			break;
		}


	}
}

//огн€ мотора от ракеты 
void fireFromWeapon() {
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
}

void addInviders() {
	srand(time_t(NULL));
	if (Level1.size() > 0 && Level1[0].invaderPositionY == 20 && Level1[0].invaderPositionX >= 130) {
		Level1.insert(Level1.cbegin(), { 50,-100, rand() % 9, 0, true, 0 });
	} else if (Level1.size() == 0) {
		Level1.push_back({ 50,-100, rand() % 9, 0, true, 0 });
	}
	
	

}
//двигать захватчики 
void moveInviders(float time) {
	if (Level1.size() > 0) {
		invidersTimer += time;
		
		for (int inviders = 0; inviders < Level1.size(); ++inviders) {
			
			if (Level1[inviders].invaderPositionY < float(20)) {
				if (invidersTimer > 1) {
					Level1[inviders].invaderPositionY += float(0.5);
				}
			}

			if (Level1[inviders].invaderPositionY >= float(20) && Level1[inviders].invaderPositionY <= float(30)){
				Level1[inviders].invaderPositionY == float(20);
				//cout << Level1[inviders].invaderPositionY << endl;
				Level1[inviders].column = 1;
			}
			invidersTimer = 0;
	
			for (int sprite = 0; sprite < 9; ++sprite) {
				invidersSprite[sprite].setPosition(Level1[inviders].invaderPositionX, Level1[inviders].invaderPositionY);
			}
			//1 column
			if (Level1[inviders].column == 1) {
				Level1[inviders].invaderPositionX += float(0.05*time); // двигаетс€ на право 
			}

			if (Level1[inviders].invaderPositionX >= float(850) && Level1[inviders].column == 1) {
				Level1[inviders].invaderPositionX = float(850);
				Level1[inviders].column = 6;//спускатс€ из 1 column в 2 column
			}
			if (Level1[inviders].column == 6) {
				Level1[inviders].invaderPositionX = float(850);
				Level1[inviders].invaderPositionY += float(0.05*time); //по шагам спускаетс€ вниз 
			}
			//column 2 
			if (Level1[inviders].invaderPositionY >= float(120) && 
				Level1[inviders].invaderPositionY <= float(200) && 
				Level1[inviders].column == 6) {
				Level1[inviders].invaderPositionY = 120;
				Level1[inviders].column = 2;
			}

			if (Level1[inviders].column == 2) {
				Level1[inviders].invaderPositionX -= float(0.05*time); //двигаетс€ налево
			}
			
			if (Level1[inviders].invaderPositionX <= float(50) && Level1[inviders].column==2) {
				Level1[inviders].invaderPositionX = float(50);
				Level1[inviders].column = 7;//спускатс€ из 2 column в 3 column
				Level1[inviders].invaderPositionY += float(0.05*time);
			}
			if (Level1[inviders].column == 7) {
				Level1[inviders].invaderPositionX = float(50);
				Level1[inviders].invaderPositionY += float(0.05*time);// спускаетс€ вниз

			}
			//сolumn 3
			if (Level1[inviders].invaderPositionY >= float(220) && 
				Level1[inviders].invaderPositionY <= float(250) && 
				Level1[inviders].column == 7) {
				Level1[inviders].invaderPositionY = 220;
				Level1[inviders].column = 3;
			}
			
			if (Level1[inviders].column == 3) {
				Level1[inviders].invaderPositionX += float(0.05*time); // двигаетс€ на право 
			}

			if ((Level1[inviders].invaderPositionX >= float(850)) && (Level1[inviders].column == 3)) {
				Level1[inviders].invaderPositionX = float(850);
				Level1[inviders].column = 8;//спускатс€ из 3 column в 4 column
			}
			
			if (Level1[inviders].column == 8) {
				Level1[inviders].invaderPositionY += float(0.05*time); // ch
			}
			if (Level1[inviders].invaderPositionY >= float(320) &&
				Level1[inviders].invaderPositionY <= float(350) &&
				Level1[inviders].column == 8) {
				Level1[inviders].invaderPositionY = 320;
				Level1[inviders].column = 4;
			}

			
			
			
			cout << 0.05*time << endl;
			window.draw(invidersSprite[Level1[inviders].invidersIndex]);

		}
		
		
	}
	
	
}

//взорвать захватчики
void explosions(float time) {
	if (Explosions.size() > 0) {
		for (int index = 0; index < Explosions.size(); ++index) {
			if (!Explosions[index].endOfExplosion) {
				Explosions[index].explosionSpeed += time;
				for (int sprite = 0; sprite < 50; ++sprite) {
					explosionSprite[sprite].setPosition(Explosions[index].explosionPositionX - 50, Explosions[index].explosionPositionY - 100);
				}
				window.draw(explosionSprite[Explosions[index].explosionImgIndex]);
				if (Explosions[index].explosionSpeed > 50) {
					++Explosions[index].explosionImgIndex;
					if (Explosions[index].explosionImgIndex == 50) {
						Explosions[index].explosionImgIndex = 0;
						Explosions[index].endOfExplosion = true;
					}
					Explosions[index].explosionSpeed = 0;
				}

			}

		}
	}
}

//показад анимаци€ фона 
void showBackground(float time) {
	backgroundTimer += time;
	if (backgroundTimer > 1) {
		window.draw(backgroundSprite[backgroundIndex]); // показать фон
		if (backgroundTimer > 200) {
			++backgroundIndex;
			backgroundTimer = 0;
		}
		if (backgroundIndex == 25) {
			backgroundIndex = 0;
		}
	}
}


int main()
{
	settingsImages();
	
	Clock clock; // создает переменную времени, т.о. прив€зка ко времени (а не загруженности мощьности процессора)

	while (window.isOpen())
	{
		float time = float(clock.getElapsedTime().asMicroseconds());//дать прощедшее врем€ в микросекундах;
		clock.restart();//презагружает врем€
		time = time / 400;//скорость игры 

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) {
				endOfProgram = true;
				window.close();
			}
				
		}
		keyboard(time);//доступ на клавиатуру и вдигать объекты с помощью клавиатуру

		window.clear();//отчистить окно 

		showBackground(time);
		fireWhileShooting(); // 
		fireFromWeapon();
		bullet(time);

		//добавл€ем новые захватчики 
		//потом 
		addInviders();
		moveInviders(time);
	
		explosions(time);
		window.draw(weaponSprite);
		window.display();
		
	}
	
	return 0;
}