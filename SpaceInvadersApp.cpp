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
first = true, hitTheTarget = false, shotOnProtection = false,
dropingBomb = true, newGame = true, gameOver = true, goingUpLetter=true;
int score = 0, life=3, colorR=0, colorG = 0, colorB = 0;
int intweaponFire = 0, backgroundIndex=0, indexOfBomb, indexOfLetterForAnimating=0;
float weaponX, weaponY, bullEndPositionY, backgroundTimer, invidersTimer, invidersSpeed, bombTimer, changeColorTimer, animatingLettersTimer;

Image bgImage, weaponImg, fireImg, bulletImg, invaders[9], explosionImg[50], 
background[25], protectionImg, emptyFrame, bombImg;
Texture bgTexture, weaponTxre, fireTxre, bulletTxre,invidersTexture[9], 
explosionTexture[50], backgroundTexture[25], protectionTexture, emptyFrameTexture, bombTexture;
Sprite bgSprite, weaponSprite, fireSprite, bulletSprite, invidersSprite[9],  
explosionSprite[50], backgroundSprite[25], protectionSprite, emptyFrameSprite, bombSprite;
Font font, newGameFont;
Text scoreText("", font, 20), lifeText("", font, 20), gameOverText("", font, 36), scoreTextOnTheEnd("", font, 26), 
pressKeybordText("", font, 16), newGameText[13];



struct WindowSize{
	int width;
	int height;
};

WindowSize WinSize = {1000,600};
vector <WindowSize> WSize = { WinSize };

RenderWindow window(sf::VideoMode(WSize[0].width, WSize[0].height), "SPACE INVADERS");

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

struct SBomb {
	float x;
	float y;
	bool exploded;
	float speed;
};

vector <SBomb> Bomb;

//удалить взрывающие захватчики из структа вектора 
void RemoveLevel1(std::vector<Invaders> & Level1, bool life) {
	Level1.erase(
		remove_if(Level1.begin(), Level1.end(), [&](Invaders const & v) {
		return v.life == false;
	}),
		Level1.end());
}

//удалить взрывающие протекторы из структа вектора 
void RemoveProtection(std::vector<SProtection> & Protection, bool visibility) {
	Protection.erase(
		remove_if(Protection.begin(), Protection.end(), [&](SProtection const & v) {
		return v.visibility == false;
	}),
		Protection.end());
}

//отчистит векора после взрыва invaiderи 
void RemoveExplosion(std::vector<Explosion> & Explosions, bool endOfExplosion) {
	Explosions.erase(
		remove_if(Explosions.begin(), Explosions.end(), [&](Explosion const & v) {
		return v.endOfExplosion == true;
	}),
		Explosions.end());
}

//отчистит ветора после взрыва бомба
void RemoveBomb(std::vector<SBomb> & Bomb, bool exploded) {
	Bomb.erase(
		remove_if(Bomb.begin(), Bomb.end(), [&](SBomb const & v) {
		return v.exploded == true;
	}),
		Bomb.end());
}

//импортирование картинки и обработики картинки
void settingsImages() {
	
	//добавляем картинки 
	weaponImg.loadFromFile("images/weapon.png");//добавление оружя 
	fireImg.loadFromFile("images/fire.png");//добавление эффект огны
	bulletImg.loadFromFile("images/bullet.png");//добавление пуля
	protectionImg.loadFromFile("images/protection.png");//добавление защитники 
	emptyFrame.loadFromFile("images/empty_button.png"); 
	bombImg.loadFromFile("images/bomb.png"); // добавление бомба 
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

	//добавляем текстура в картинки 
	weaponTxre.loadFromImage(weaponImg);
	fireTxre.loadFromImage(fireImg);
	bulletTxre.loadFromImage(bulletImg);
	protectionTexture.loadFromImage(protectionImg);
	emptyFrameTexture.loadFromImage(emptyFrame);
	bombTexture.loadFromImage(bombImg);


	//текстура для захватчики 
	for (int texture = 0; texture < 9; ++texture) {
		invidersTexture[texture].loadFromImage(invaders[texture]);
	}
	//текстура для картинки взрыва 
	for (int texture = 0; texture < 50; ++texture) {
		explosionTexture[texture].loadFromImage(explosionImg[texture]);
	}

	for (int texture = 0; texture < 25; ++texture) {
		backgroundTexture[texture].loadFromImage(background[texture]);
	}

	//добавляем спрайт для текстуры
	bombSprite.setTexture(bombTexture);
	bombSprite.setTextureRect(IntRect(0, 22, 48, 48));
	emptyFrameSprite.setTexture(emptyFrameTexture);
	weaponSprite.setTexture(weaponTxre);
	weaponSprite.setPosition(float(WSize[0].width / 2 - 75), float(WSize[0].height - 170));//задаем началные позиции оружя 
	weaponSprite.setScale(
		150.0f / weaponSprite.getLocalBounds().width,
		150.0f / weaponSprite.getLocalBounds().height
	); //  задем размер для оружя 
	fireSprite.setTexture(fireTxre);
	bulletSprite.setTexture(bulletTxre);

	bulletSprite.setTextureRect(IntRect(339, 212, 26, 26));
	bulletSprite.setPosition(weaponSprite.getPosition().x + 67, weaponSprite.getPosition().y + 50);//задаем стартовий позиции оружя 
	bulletSprite.setScale(
		15.0f / bulletSprite.getLocalBounds().width,
		15.0f / bulletSprite.getLocalBounds().height
	); // задаем размер пуля
	protectionSprite.setTexture(protectionTexture);
	protectionSprite.setScale(
		30.0f / protectionSprite.getLocalBounds().width, 
		30.0f / protectionSprite.getLocalBounds().height
	);
	//спрайт для захватчики 
	for (int sprite = 0; sprite < 9; ++sprite) {
		invidersSprite[sprite].setTexture(invidersTexture[sprite]);
	}

	//спрайт для эффект взрыва
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
		Protection.push_back({ float(100 + protectionIndex), float(WinSize.height - 320),true });
		Protection.push_back({ float(100 + protectionIndex), float(WinSize.height - 350),true });
		Protection.push_back({ float(400 + protectionIndex),float(WinSize.height - 320),true });
		Protection.push_back({ float(400 + protectionIndex), float(WinSize.height - 350),true });
		Protection.push_back({ float(700 + protectionIndex), float(WinSize.height - 320),true });
		Protection.push_back({ float(700 + protectionIndex), float(WinSize.height - 350),true });
	}
}

void fontsProsession() {
	font.loadFromFile("fonts/ChargeVector.ttf");
	newGameFont.loadFromFile("fonts/CubicPixel.otf");
	scoreText.setStyle(Text::Bold);
	lifeText.setStyle(Text::Bold);
	for (int i = 0; i < 13; ++i) {
		newGameText[i].setFont(newGameFont);
		newGameText[i].setCharacterSize(60);
		newGameText[i].setStyle(Text::Bold);
	}

	newGameText[0].setString("S");
	newGameText[1].setString("P");
	newGameText[2].setString("A");
	newGameText[3].setString("C"); 
	newGameText[4].setString("E");
	newGameText[5].setString("I");
	newGameText[6].setString("N");
	newGameText[7].setString("V");
	newGameText[8].setString("A");
	newGameText[9].setString("D");
	newGameText[10].setString("E");
	newGameText[11].setString("R");
	newGameText[12].setString("S");
	int startCoordinationX = 430, startCoordinationY = 175;
	for (int i = 0; i < 5; ++i) {
		newGameText[i].setPosition(startCoordinationX, startCoordinationY);
		startCoordinationX += 32;
	}
	startCoordinationX = 390;
	startCoordinationY = 250;
	newGameText[5].setPosition(startCoordinationX, startCoordinationY);
	startCoordinationX += 15;
	for (int i = 6; i < 13; ++i) {
		newGameText[i].setPosition(startCoordinationX, startCoordinationY);
		startCoordinationX += 32;
	}


}

void printingTextWithFont() {
	//frame но пока не надо , елси будеть нужны поставлю
	/*emptyFrameSprite.setPosition(WinSize.width - 235, 4);
	emptyFrameSprite.setScale(
		100.0f / emptyFrameSprite.getLocalBounds().width,
		35.0f / emptyFrameSprite.getLocalBounds().height
	);*/
	lifeText.setString("Life: " + to_string(life));
	lifeText.setPosition(float(WinSize.width - 320), float(8));

	scoreText.setString("Score: " + to_string(score));
	scoreText.setPosition(float(WinSize.width - 220), float(8));

	
	
	window.draw(lifeText);
	window.draw(scoreText);

}

//доступ на клавиатуру
void keyboard(float time) {
	
	if (Keyboard::isKeyPressed(Keyboard::Escape)){
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

//показать огня при стельбе 
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

//стерлят и удалить мертвые захватчики из вектор захватчиков
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
					if (((Protection[indexx].x  <= BulletList[count].weaponPositionX) &&
						(Protection[indexx].x + 30 >= BulletList[count].weaponPositionX-15)) &&
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

//огня мотора от ракеты 
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
			
			//соблюдение растояние между inviders
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
				//стоят на месте когда другие захватчики спускаются из column 1 в column 2 
				Level1[inviders].stop = true;

			} else if ((Level1.size() > 2 && Level1.size() - 1 != inviders && Level1[inviders].invaderPositionY + 100 <= Level1[inviders + 1].invaderPositionY &&
				Level1[inviders].invaderPositionY <= Level1[inviders + 1].invaderPositionY &&
				Level1[inviders].invaderPositionY + 105 >= Level1[inviders + 1].invaderPositionY &&
				Level1[inviders + 1].invaderPositionX >= 740 && inviders != 0)) {
				//стоят на месте когда другие захватчики спускаются из column 2 в column 3 
				Level1[inviders].stop = true;
			}
			else {
				//в остальном случае двигаются
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
						Level1[inviders].invaderPositionX += float(0.05*time); // двигается на право 
					}
				}
				

			}

			if (Level1[inviders].invaderPositionX >= float(850) && Level1[inviders].column == 1) {
				Level1[inviders].invaderPositionX = float(850);
				Level1[inviders].column = 6;//спускатся из 1 column в 2 column
			}
			if (Level1[inviders].column == 6) {
				Level1[inviders].invaderPositionX = float(850);
				if (!Level1[inviders].stop) {
					Level1[inviders].invaderPositionY += float(0.05*time); //по шагам спускается вниз 
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
					Level1[inviders].invaderPositionX -= float(0.05*time); //двигается налево
				}
			}
			
			if (Level1[inviders].invaderPositionX <= float(50) && Level1[inviders].column==2) {

				Level1[inviders].invaderPositionX = float(50);
				Level1[inviders].column = 7;//спускатся из 2 column в 3 column
				if (!Level1[inviders].stop) {
					Level1[inviders].invaderPositionY += float(0.05*time);
				}
			}
			if (Level1[inviders].column == 7) {
				Level1[inviders].invaderPositionX = float(50);
				if (!Level1[inviders].stop) {
					Level1[inviders].invaderPositionY += float(0.05*time);// спускается вниз
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
					Level1[inviders].invaderPositionX += float(0.05*time); // двигается на право 
				}
			}

			if ((Level1[inviders].invaderPositionX >= float(850)) && (Level1[inviders].column == 3)) {
				Level1[inviders].invaderPositionX = float(850);
				Level1[inviders].column = 8;//спускатся из 3 column в 4 column
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

//показад анимация фона 
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
		protectionSprite.setPosition(Protection[protectionIndex].x+26, Protection[protectionIndex].y+100);
		window.draw(protectionSprite);
	}
		
}
//неправильно работает надо как то изменить чтобы праильно появилась бомба на координате оружя 
void bomb(float time) {
	bombTimer += time;
	if (bombTimer > 15000) {
		for (int inviders = 0; inviders < Level1.size(); ++inviders) {
			if (Level1[inviders].invaderPositionX >= weaponSprite.getPosition().x &&
				Level1[inviders].invaderPositionX +150>= weaponSprite.getPosition().x) {
				Bomb.push_back({ Level1[inviders].invaderPositionX,Level1[inviders].invaderPositionY, false });
				dropingBomb = false;
				bombTimer = 0;
				break;
			}
		}
		
	}
	
	if (Bomb.size() > 0) {
		for (int bombIndex = 0; bombIndex < Bomb.size(); ++bombIndex) {
			
			if (!Bomb[bombIndex].exploded) {
				if (Bomb[bombIndex].y < 600 && Bomb[bombIndex].x > 50) {
					Bomb[bombIndex].speed += time;
					bombSprite.setPosition(Bomb[bombIndex].x, Bomb[bombIndex].y);
					if (Bomb[bombIndex].speed > 5) {
						Bomb[bombIndex].y += 1.0f;
						Bomb[bombIndex].speed = 0;
						
					}
					
				}
				
				if (Bomb[bombIndex].y >= 600) {
					Bomb[bombIndex].exploded = true; 
					RemoveBomb(Bomb, true);
				}
				
				if (weaponSprite.getPosition().x <= Bomb[bombIndex].x &&
					weaponSprite.getPosition().x+150>= Bomb[bombIndex].x &&
					Bomb[bombIndex].y >= 480 ){
					Explosions.push_back({ Bomb[bombIndex].x,Bomb[bombIndex].y,false,0 });
					Bomb[bombIndex].exploded = true;
					RemoveBomb(Bomb, true);
					--life;
					if (life == 0) {
						gameOver = true;
					}
				}
				
				
				
				
				window.draw(bombSprite);
			}

		}
		
	}

	cout << Bomb.size() << endl; 
	
}

void GameOver(float time) {
	
	changeColorTimer += time;
	RectangleShape rectangleBack(Vector2f(420.f, 310.f));
	rectangleBack.move(295, 135);
	rectangleBack.setFillColor(Color(100-colorR, 100-colorG, 100-colorB));
	window.draw(rectangleBack);
	if (changeColorTimer > 500) {
		colorR = 2* rand() % 50;
		colorG = 2 * rand() % 50;
		colorB = 2 * rand() % 50;
		changeColorTimer = 0;
	}
	
	
	RectangleShape rectangle(Vector2f(410.f, 300.f));
	rectangle.move(300, 140);
	rectangle.setFillColor(Color(29, 7, 68));
	window.draw(rectangle);

	gameOverText.setString("Game over!");
	gameOverText.setPosition(390, 155);
	gameOverText.setStyle(Text::Bold);
	window.draw(gameOverText);

	scoreTextOnTheEnd.setString("You have erned ");
	scoreTextOnTheEnd.setPosition(390, 220);
	scoreTextOnTheEnd.setStyle(Text::Bold);
	window.draw(scoreTextOnTheEnd);

	int lengthText = strlen(to_string(score).c_str());
	scoreTextOnTheEnd.setString(to_string(score));
	int goBackCoordination = 0;

	if (lengthText > 1) {
		for (int textLenght = 1; textLenght < lengthText; ++textLenght) {
			goBackCoordination += 8;
		}
	}

	scoreTextOnTheEnd.setPosition(492 - goBackCoordination, 260);
	window.draw(scoreTextOnTheEnd);

	goBackCoordination = 0;

	scoreTextOnTheEnd.setString(" score!!");
	scoreTextOnTheEnd.setPosition(450, 300);
	window.draw(scoreTextOnTheEnd);

	pressKeybordText.setString("Press Enter to start a new game");
	pressKeybordText.setPosition(340, 350);
	window.draw(pressKeybordText);

	pressKeybordText.setString("or");
	pressKeybordText.setPosition(490, 370);
	window.draw(pressKeybordText);

	pressKeybordText.setString("press Escape to exit the game.");
	pressKeybordText.setPosition(350, 390);
	window.draw(pressKeybordText);

	if (Keyboard::isKeyPressed(Keyboard::Enter)) {
		gameOver = false;
		settingsImages();
		fontsProsession();
		Level1.clear();
		Explosions.clear();
		BulletList.clear();
		Protection.clear();
		life = 3;
		score = 0;
		for (int protectionIndex = 0; protectionIndex < 150; protectionIndex += 30) {
			Protection.push_back({ float(100 + protectionIndex), float(WinSize.height - 320),true });
			Protection.push_back({ float(100 + protectionIndex), float(WinSize.height - 350),true });
			Protection.push_back({ float(400 + protectionIndex),float(WinSize.height - 320),true });
			Protection.push_back({ float(400 + protectionIndex), float(WinSize.height - 350),true });
			Protection.push_back({ float(700 + protectionIndex), float(WinSize.height - 320),true });
			Protection.push_back({ float(700 + protectionIndex), float(WinSize.height - 350),true });
		}

	}
	if (Keyboard::isKeyPressed(Keyboard::Escape)) {
		window.close();
	}
}

void startANewGame(float time) {
	changeColorTimer += time;
	animatingLettersTimer += time; 
	RectangleShape rectangleBack(Vector2f(420.f, 310.f));
	rectangleBack.move(295, 135);
	rectangleBack.setFillColor(Color(100 - colorR, 100 - colorG, 100 - colorB));
	window.draw(rectangleBack);
	if (changeColorTimer > 500) {
		colorR = 2 * rand() % 50;
		colorG = 2 * rand() % 50;
		colorB = 2 * rand() % 50;
		changeColorTimer = 0;
	}


	RectangleShape rectangle(Vector2f(410.f, 300.f));
	rectangle.move(300, 140);
	rectangle.setFillColor(Color(29, 7, 68));
	window.draw(rectangle);

	for (int i = 0; i < 13; ++i) {
		if (animatingLettersTimer > 10) {
			if (goingUpLetter) {
				float y = newGameText[indexOfLetterForAnimating].getPosition().y;
				float x = newGameText[indexOfLetterForAnimating].getPosition().x;
				y -= 0.5f;
				newGameText[indexOfLetterForAnimating].setPosition(x, y);
				if (y <= 165 && indexOfLetterForAnimating<5) {
					goingUpLetter = false;
				}
				else if (y <= 230 && indexOfLetterForAnimating >= 5) {
					goingUpLetter = false;
				}
			}
			if (!goingUpLetter) {
				float y = newGameText[indexOfLetterForAnimating].getPosition().y;
				float x = newGameText[indexOfLetterForAnimating].getPosition().x;
				y += 0.5f;
				newGameText[indexOfLetterForAnimating].setPosition(x, y);
				
				if (y >= 175 && indexOfLetterForAnimating < 5) {
					goingUpLetter = true;
					++indexOfLetterForAnimating;
					if (indexOfLetterForAnimating == 13) {
						indexOfLetterForAnimating = 0;
					}
				}
				else if (y >= 240 && indexOfLetterForAnimating >= 5) {
					goingUpLetter = true;
					++indexOfLetterForAnimating;
					if (indexOfLetterForAnimating == 13) {
						indexOfLetterForAnimating = 0;
					}
				}
			}
			
			animatingLettersTimer = 0;
		}
		window.draw(newGameText[i]);
	}
		
	pressKeybordText.setString("Press Enter to start a new game");
	pressKeybordText.setPosition(350, 350);
	window.draw(pressKeybordText);

	pressKeybordText.setString("or");
	pressKeybordText.setPosition(500, 370);
	window.draw(pressKeybordText);

	pressKeybordText.setString("press Escape to exit the game.");
	pressKeybordText.setPosition(360, 390);
	window.draw(pressKeybordText);

	if (Keyboard::isKeyPressed(Keyboard::Enter)) {
		newGame = false;

	}
	if (Keyboard::isKeyPressed(Keyboard::Escape)) {
		window.close();
	}
}

int main()
{
	settingsImages();
	fontsProsession();

	Clock clock; // создает переменную времени, т.о. привязка ко времени (а не загруженности мощьности процессора)

	while (window.isOpen())
	{
		float time = float(clock.getElapsedTime().asMicroseconds());//дать прощедшее время в микросекундах;
		clock.restart();//презагружает время
		time = time / 400;//скорость игры 

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) {
				window.close();
			}
				
		}

		window.clear();//отчистить окно 

		showBackground(time);
		protection();
		fireFromWeapon();
		window.draw(weaponSprite);

		if (newGame) {
			startANewGame(time);
		}

		if (!newGame) {
			if (gameOver) {
				GameOver(time);
			}

			if (!gameOver) {
				keyboard(time);//доступ на клавиатуру и вдигать объекты с помощью клавиатуру
				printingTextWithFont();
				fireWhileShooting(); // 
				bullet(time);
				addInviders();
				moveInviders(time);
				explosions(time);
				bomb(time);

			}
		}
		
		
		window.display();
		
	}
	
	return 0;
}