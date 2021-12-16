#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <ctime>
#include <string>
//#include <sstring>

using namespace sf;
using namespace std;

bool fire = false, weaponFire = true, leftFireRotation = false,
rightFireRotation = false, clickOnKeyboardUP = false,
first = true, hitTheTarget = false, endOfProgram = false, shotOnProtection=false;
int score = 0, life=3;
int intweaponFire = 0, backgroundIndex=0;
float weaponX, weaponY, bullEndPositionY, backgroundTimer, invidersTimer, invidersSpeed;

Image bgImage, weaponImg, fireImg, bulletImg, invaders[9], explosionImg[50], background[25], protectionImg;
Texture bgTexture, weaponTxre, fireTxre, bulletTxre,invidersTexture[9], explosionTexture[50], backgroundTexture[25], protectionTexture;
Sprite bgSprite, weaponSprite, fireSprite, bulletSprite, invidersSprite[9],  explosionSprite[50], backgroundSprite[25], protectionSprite;
Font font;
Text scoreText("", font, 20), lifeText("", font, 20);

struct WindowSize{
	int width;
	int height;
};

WindowSize WinSize = {1000,600};
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
	bool stop;
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

struct SProtection {
	float x;
	float y;
	bool visibility; 
};

vector <SProtection> Protection;


//удалить взрывающие захватчики из структа вектора 
void RemoveLevel1(std::vector<Invaders> & Level1, bool life) {
	Level1.erase(
		std::remove_if(Level1.begin(), Level1.end(), [&](Invaders const & v) {
		return v.life == false;
	}),
		Level1.end());
}

//удалить взрывающие протекторы из структа вектора 
void RemoveProtection(std::vector<SProtection> & Protection, bool visibility) {
	Protection.erase(
		std::remove_if(Protection.begin(), Protection.end(), [&](SProtection const & v) {
		return v.visibility == false;
	}),
		Protection.end());
}

//отчистит ветора после взрыва 
void RemoveExplosion(std::vector<Explosion> & Explosions, bool endOfExplosion) {
	Explosions.erase(
		std::remove_if(Explosions.begin(), Explosions.end(), [&](Explosion const & v) {
		return v.endOfExplosion == true;
	}),
		Explosions.end());
}

//импортирование картинки и обработики картинки
void settingsImages() {
	
	//добавл€ем картинки 
	bgImage.loadFromFile("images/bg.png");//добавление background image 
	weaponImg.loadFromFile("images/weapon.png");//добавление оруж€ 
	fireImg.loadFromFile("images/fire.png");//добавление эффект огны
	bulletImg.loadFromFile("images/bullet.png");//добавление пул€
	protectionImg.loadFromFile("images/protection.png");
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
	protectionTexture.loadFromImage(protectionImg);

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
	protectionSprite.setTexture(protectionTexture);
	protectionSprite.setScale(
		30.0f / protectionSprite.getLocalBounds().width, 
		30.0f / protectionSprite.getLocalBounds().height
	);
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
		backgroundSprite[sprite].setTextureRect(IntRect(140, 0, 1000, 600));
		backgroundSprite[sprite].setScale(
			1000.0f / backgroundSprite[sprite].getLocalBounds().width,
			600.0f / backgroundSprite[sprite].getLocalBounds().height
		);
	}
	//координации protection
	for (int protectionIndex = 0; protectionIndex < 150; protectionIndex += 30) {

		Protection.push_back({ float(50 + protectionIndex), float(WinSize.height - 220),true });
		Protection.push_back({ float(50 + protectionIndex), float(WinSize.height - 250),true });
		Protection.push_back({ float(425 + protectionIndex),float(WinSize.height - 220),true });
		Protection.push_back({ float(425 + protectionIndex), float(WinSize.height - 250),true });
		Protection.push_back({ float(800 + protectionIndex), float(WinSize.height - 220),true });
		Protection.push_back({ float(800 + protectionIndex), float(WinSize.height - 250),true });
	}



}

void fontsProsession() {
	font.loadFromFile("fonts/ChargeVector.ttf");
	//scoreText.setColor(Color::Red);
	scoreText.setStyle(Text::Bold);
	lifeText.setStyle(Text::Bold);

}

void printingTextWithFont() {
	
	lifeText.setString("Life: " + to_string(life));
	lifeText.setPosition(WinSize.width - 320, 4);

	scoreText.setString("Score: " + to_string(score));
	scoreText.setPosition(WinSize.width - 220, 4);
	
	window.draw(lifeText);
	window.draw(scoreText);

}

//первоначальные захватчики и их свойств
void makeInvides() {
	
	//структура Level 1
	

}

//доступ на клавиатуру
void keyboard(float time) {
	
	if (Keyboard::isKeyPressed(Keyboard::Escape)|| 
	   (Keyboard::isKeyPressed(Keyboard::LAlt)||
		(Keyboard::isKeyPressed(Keyboard::RAlt)) && Keyboard::isKeyPressed(Keyboard::F4))) {
		window.close();
	}
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
						score += 10;
						first = true;
						BulletList[count].bullet = false;
						BulletList.clear();

						break;
					}

				}

				for (int indexx = 0; indexx < Protection.size(); ++indexx) {
					if (((Protection[indexx].x-15  <= BulletList[count].weaponPositionX) &&
						(Protection[indexx].x + 30 >= BulletList[count].weaponPositionX)) &&
						(Protection[indexx].y >= BulletList[count].weaponPositionY - 30)) {
						bullEndPositionY = BulletList[count].weaponPositionY;;
						Protection[indexx].visibility = false;
						RemoveProtection(Protection, false);
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
		Level1.insert(Level1.cbegin(), { 50,-100, rand() % 9, 0, true, 0, false });
	} else if (Level1.size() == 0) {
		Level1.push_back({ 50,-100, rand() % 9, 0, true, 0, false });
	}
}

//двигать захватчики 
void moveInviders(float time) {
	if (Level1.size() > 0) {
		invidersTimer += time;
		for (int inviders = 0; inviders < Level1.size(); ++inviders) {
			
			//соблюдение расто€ние между inviders
			if((Level1.size()>2 && Level1.size()-1!=inviders && Level1[inviders].invaderPositionX+100 <= Level1[inviders+1].invaderPositionX &&
				Level1[inviders].invaderPositionX + 105 >= Level1[inviders + 1].invaderPositionX && 
				Level1[inviders].invaderPositionY == Level1[inviders + 1].invaderPositionY && (
				Level1[inviders].column==1 || Level1[inviders].column == 3))) {
				//соблюдают в colum1 и column 3  
				Level1[inviders].stop=true;
			} else if ((Level1.size() > 2 && Level1.size() - 1 != inviders && Level1[inviders+1].invaderPositionX + 100 <= Level1[inviders].invaderPositionX &&
				Level1[inviders+1].invaderPositionX + 105 >= Level1[inviders].invaderPositionX &&
				Level1[inviders].invaderPositionY == Level1[inviders + 1].invaderPositionY &&
				Level1[inviders].column == 2)) {
				//соблюдают в colum2
				Level1[inviders].stop = true;

			} else if ((Level1.size() > 2 && Level1.size() - 1 != inviders && Level1[inviders].invaderPositionY + 100 <= Level1[inviders + 1].invaderPositionY &&
				Level1[inviders].invaderPositionY <= Level1[inviders + 1].invaderPositionY &&
				Level1[inviders].invaderPositionY + 105 >= Level1[inviders + 1].invaderPositionY &&
				Level1[inviders + 1].invaderPositionX <= 160 && inviders != 0)) {
				//сто€т на месте когда другие захватчики спускаютс€ из column 1 в column 2 
				Level1[inviders].stop = true;

			} else if ((Level1.size() > 2 && Level1.size() - 1 != inviders && Level1[inviders].invaderPositionY + 100 <= Level1[inviders + 1].invaderPositionY &&
				Level1[inviders].invaderPositionY <= Level1[inviders + 1].invaderPositionY &&
				Level1[inviders].invaderPositionY + 105 >= Level1[inviders + 1].invaderPositionY &&
				Level1[inviders + 1].invaderPositionX >= 740 && inviders != 0)) {
				//сто€т на месте когда другие захватчики спускаютс€ из column 2 в column 3 
				Level1[inviders].stop = true;
			}
			else {
				//в остальном случае двигаютс€
				Level1[inviders].stop = false;
			}

			if (Level1[inviders].invaderPositionY < float(20)) {
				if (invidersTimer > 1) {
					if (!Level1[inviders].stop) {
						Level1[inviders].invaderPositionY += float(0.5);
					}
				}
			}

			if (Level1[inviders].invaderPositionY >= float(20) && 
				Level1[inviders].invaderPositionY <= float(30) && 
				Level1[inviders].column==0){
				Level1[inviders].invaderPositionY = float(20);
				
				Level1[inviders].column = 1;
				
				
			}
			invidersTimer = 0;
	
			for (int sprite = 0; sprite < 9; ++sprite) {
				invidersSprite[sprite].setPosition(Level1[inviders].invaderPositionX, Level1[inviders].invaderPositionY);
			}
			//1 column
			if (Level1[inviders].column == 1) {
				if (Level1[inviders].invaderPositionX + 100 != Level1[inviders + 1].invaderPositionX) {
					if (!Level1[inviders].stop) {
						Level1[inviders].invaderPositionX += float(0.05*time); // двигаетс€ на право 
					}
				}
				

			}

			if (Level1[inviders].invaderPositionX >= float(850) && Level1[inviders].column == 1) {
				Level1[inviders].invaderPositionX = float(850);
				Level1[inviders].column = 6;//спускатс€ из 1 column в 2 column
			}
			if (Level1[inviders].column == 6) {
				Level1[inviders].invaderPositionX = float(850);
				if (!Level1[inviders].stop) {
					Level1[inviders].invaderPositionY += float(0.05*time); //по шагам спускаетс€ вниз 
				}
			}
			//column 2 
			if (Level1[inviders].invaderPositionY >= float(119) && 
				Level1[inviders].invaderPositionY <= float(200) && 
				Level1[inviders].column == 6) {
				Level1[inviders].invaderPositionY = 120;
				Level1[inviders].column = 2;
			}

			if (Level1[inviders].column == 2) {
				if (!Level1[inviders].stop) {
					Level1[inviders].invaderPositionX -= float(0.05*time); //двигаетс€ налево
				}
			}
			
			if (Level1[inviders].invaderPositionX <= float(50) && Level1[inviders].column==2) {

				Level1[inviders].invaderPositionX = float(50);
				Level1[inviders].column = 7;//спускатс€ из 2 column в 3 column
				if (!Level1[inviders].stop) {
					Level1[inviders].invaderPositionY += float(0.05*time);
				}
			}
			if (Level1[inviders].column == 7) {
				Level1[inviders].invaderPositionX = float(50);
				if (!Level1[inviders].stop) {
					Level1[inviders].invaderPositionY += float(0.05*time);// спускаетс€ вниз
				}
			}
			//сolumn 3
			if (Level1[inviders].invaderPositionY >= float(220) && 
				Level1[inviders].invaderPositionY <= float(250) && 
				Level1[inviders].column == 7) {
				Level1[inviders].invaderPositionY = 220;
				Level1[inviders].column = 3;
			}
			
			if (Level1[inviders].column == 3) {
				if (!Level1[inviders].stop) {
					Level1[inviders].invaderPositionX += float(0.05*time); // двигаетс€ на право 
				}
			}

			if ((Level1[inviders].invaderPositionX >= float(850)) && (Level1[inviders].column == 3)) {
				Level1[inviders].invaderPositionX = float(850);
				Level1[inviders].column = 8;//спускатс€ из 3 column в 4 column
			}
		
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
		RemoveExplosion(Explosions, true);
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

void protection() {
	
	for (int protectionIndex = 0; protectionIndex < Protection.size(); ++protectionIndex) {
		protectionSprite.setPosition(Protection[protectionIndex].x, Protection[protectionIndex].y);
		window.draw(protectionSprite);
	}
		
}

int main()
{
	settingsImages();
	fontsProsession();

	
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
		printingTextWithFont();
		protection();
		fireWhileShooting(); // 
		fireFromWeapon();
		bullet(time);
		addInviders();
		moveInviders(time);
		explosions(time);
		window.draw(weaponSprite);
		window.display();
		
	}
	
	return 0;
}