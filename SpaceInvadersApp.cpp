#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <iomanip>

using namespace sf;
using namespace std;

struct WindowSize{
	int width;
	int height;
};

WindowSize WinSize = {1000,560};
vector <WindowSize> WSize = { WinSize };

struct Bullet {
	int weaponPositionX;
	int weaponPositionY;
	string direction;
};



int main()
{
	bool fire = false, weaponFire=true, bullet=false, leftFireRotation=false, rightFireRotation=false;
	int intweaponFire=0;
	int intBullet = 0, step = 0;
	RenderWindow window(sf::VideoMode(WSize[0].width, WSize[0].height), "SFML Application",Style::Close);
	
	Image bgImage, weaponImg, fireImg, bulletImg;
	bgImage.loadFromFile("images/bg.png");
	weaponImg.loadFromFile("images/weapon.png");
	fireImg.loadFromFile("images/fire.png");
	bulletImg.loadFromFile("images/bullet.png");

	Texture bgTexture, weaponTxre, fireTxre, bulletTxre;
	bgTexture.loadFromImage(bgImage);
	weaponTxre.loadFromImage(weaponImg);
	fireTxre.loadFromImage(fireImg);
	bulletTxre.loadFromImage(bulletImg);

	Sprite bgSprite, weaponSprite, fireSprite, bulletSprite;
	bgSprite.setTexture(bgTexture);
	bgSprite.setPosition(0, 0);
	weaponSprite.setTexture(weaponTxre);
	weaponSprite.setPosition(WSize[0].width/2-75, WSize[0].height -150);
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
				window.close();
			}
				
		}


		if (Keyboard::isKeyPressed(Keyboard::Left)||
			Keyboard::isKeyPressed(Keyboard::A) || 
			Keyboard::isKeyPressed(Keyboard::Num4) ||
			Keyboard::isKeyPressed(Keyboard::Numpad4)) {
			if (weaponSprite.getPosition().x >= 0) {
				weaponSprite.move(-0.4, 0);
				fireSprite.move(-0.4, 0);
				leftFireRotation = true;
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
			
			if (weaponSprite.getPosition().x <= 850) {
				weaponSprite.move(0.4, 0);
				fireSprite.move(0.4, 0);
				rightFireRotation = true;
			}

		}
		else {
			rightFireRotation = false;
		}
		if (Keyboard::isKeyPressed(Keyboard::Up) || 
			Keyboard::isKeyPressed(Keyboard::W) || 
			Keyboard::isKeyPressed(Keyboard::Num8) ||
			Keyboard::isKeyPressed(Keyboard::Numpad8)) {
			bulletSprite.setPosition(weaponSprite.getPosition().x + 67, weaponSprite.getPosition().y + 50);
			fireSprite.setRotation(0);
			weaponSprite.setRotation(0);
			fire = true;
			bullet = true;

		}
		 
		

		if (Keyboard::isKeyPressed(Keyboard::Down)) {
			fire = false;
		}
		if (Keyboard::isKeyPressed(Keyboard::Space)) {
			
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
		if (bullet) {
			if (bulletSprite.getPosition().y >= 15) {
				
				bulletSprite.move(0, -1);
				window.draw(bulletSprite);
			}
		}

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
			
		window.draw(weaponSprite);
		window.display();
	}

	return 0;
}