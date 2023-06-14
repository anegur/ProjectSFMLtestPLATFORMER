#include <SFML/Graphics.hpp>
#include "map.h"
#include "view.h"
#include <iostream>
#include <sstream>
#include "mission.h"

using namespace sf;

class Player { //класс игрока
private:
	float x, y = 0;
public:
	float w, h, dx, dy, speed;
	int dir, playerScore, health;
	bool life;
	String File;
	Image image;
	Texture texture;
	Sprite sprite;
//private:
	Player(const String F, int X, int Y, float W, float H) { //конструктор класса Игрок
		dx = 0; dy = 0; speed = 0; dir = 0; health = 120;
		life = true;
		playerScore = 0;
		File = F;
		w = W, h = H;
		image.loadFromFile("images/" + File);
		image.createMaskFromColor(Color(41, 33, 59));
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		x = X, y = Y;
		sprite.setTextureRect(IntRect(w, h, w, h));
	}
	void update(float time) {
		switch (dir) {
		case 0: dx = speed; dy = 0; break;
		case 1: dx = -speed; dy = 0; break;
		case 2: dx = 0; dy = speed; break;
		case 3: dx = 0; dy = -speed; break;
		}
		x += dx*time;
		y += dy * time;

		speed = 0;
		sprite.setPosition(x, y);
		interactionWithMap();
		if (health <= 0) { life = false; }
	}

	float getplayercoordinateX() {
		return x;
	}
	float getplayercoordinateY() {
		return y;
	}


	void interactionWithMap()//функция взаимодействия с картой 
	{
		for (int i = y / 32; i < (y + h) / 32; i++)
			for (int j = x / 32; j < (x + h) / 32; j++)
			{
				if (TileMap[i][j] == '0')//ограничение у границы с картой
				{
					if (dy > 0)
					{
						y = i * 32 - h;
					}
					if (dy < 0)
					{
						y = i * 32 + 32;
					}
					if (dx > 0)
					{
						x = j * 32 - w;
					}
					if (dx < 0)
					{
						x = j * 32 + 32;
					}
				}
				if (TileMap[i][j] == 's')//столкновение с камнем (очки)
				{
					playerScore++;
					TileMap[i][j] = ' ';
				}
				if (TileMap[i][j] == 'f')//столкновение с цветком (-жизнь)
				{
					health -= 40;
					TileMap[i][j] = ' ';
				}
				if (TileMap[i][j] == 'h')//столкновение с сердечком (+жизнь)
				{
					health += 20;
					TileMap[i][j] = ' ';
				}
			}
	}

};


int main()
{
	RenderWindow window(sf::VideoMode(640, 480), "project Humanity-Complimintation"); //создание окна
	view.reset(FloatRect(0, 0, 640, 480));

	Font font;
	font.loadFromFile("CyrilicOld.ttf");		//добавление шрифта
	Text text("", font, 20);		//текст по умолчанию (его нет)
	text.setFillColor(Color::Black);

	Image map_image; //картинка карты (блоки)
	map_image.loadFromFile("images/map.png");
	Texture map;	 //текстура карты
	map.loadFromImage(map_image);
	Sprite s_map;	 //спрайт карты
	s_map.setTexture(map);

	Image quest_image;
	quest_image.loadFromFile("images/missionbg.jpg");
	quest_image.createMaskFromColor(Color(0, 0, 0));
	Texture quest_texture;
	quest_texture.loadFromImage(quest_image);
	Sprite s_quest;
	s_quest.setTexture(quest_texture);
	s_quest.setTextureRect(IntRect(0, 0, 340, 510));  //приведение типов, размеры картинки исходные
	s_quest.setScale(0.6f, 0.6f);//чуть уменьшили картинку, => размер стал меньше
	
	Player p("hero.png", 250, 250, 96.0, 96.0); //объект игрок класса игрок

	bool showMissionText = true; //переменная, отвечающая за показ миссии на экране

	float CurrentFrame = 0;
	Clock clock;
	Clock gameTimeClock; //переменная, считающая время игры
	int gameTime = 0;
	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();

		if (p.life) gameTime = gameTimeClock.getElapsedTime().asSeconds(); //подсчёт времени
		clock.restart();
		time = time / 400; //скорость игры

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == Event::KeyPressed)//событие нажатия клавиши
				if ((event.key.code == Keyboard::Tab)) {//если клавиша ТАБ


					switch (showMissionText) {//переключатель, реагирующий на логическую переменную showMissionText

					case true: {
						std::ostringstream playerHealthString;//строка здоровья игрока
						playerHealthString << p.health; //заносим в строку здоровье 
						std::ostringstream task;//строка текста миссии
						task << getTextMission(getCurrentMission(p.getplayercoordinateX()));//вызывается функция getTextMission (она возвращает текст миссии), которая принимает в качестве аргумента функцию getCurrentMission(возвращающую номер миссии), а уже эта ф-ция принимает в качестве аргумента функцию p.getplayercoordinateX() (эта ф-ция возвращает Икс координату игрока)
						text.setString("Здоровье: " + playerHealthString.str() + "\n" + task.str());//задаем
						text.setPosition(view.getCenter().x + 125, view.getCenter().y - 130);//позиция всего этого текстового блока
						s_quest.setPosition(view.getCenter().x + 115, view.getCenter().y - 130);//позиция фона для блока
						showMissionText = false;//эта строка позволяет убрать все что мы вывели на экране
						break;//выходим , чтобы не выполнить условие "false" (которое ниже)
					}
					case false: {
						text.setString("");//если не нажата клавиша таб, то весь этот текст пустой
						showMissionText = true;// а эта строка позволяет снова нажать клавишу таб и получить вывод на экран
						break;
					}
					}
				}
		}
		if (p.life) 
		{
			if ((Keyboard::isKeyPressed(Keyboard::Left) || (Keyboard::isKeyPressed(Keyboard::A)))) {
				p.dir = 1; p.speed = 0.1;//dir =1 - направление вверх, speed =0.1 - скорость движения.
				CurrentFrame += 0.005 * time;
				if (CurrentFrame > 3) CurrentFrame -= 3;
				p.sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 96, 96, 96)); //через объект p класса player меняем спрайт, делая анимацию (используя оператор точку)
			}

			if ((Keyboard::isKeyPressed(Keyboard::Right) || (Keyboard::isKeyPressed(Keyboard::D)))) {
				p.dir = 0; p.speed = 0.1;//направление вправо
				CurrentFrame += 0.005 * time;
				if (CurrentFrame > 3) CurrentFrame -= 3;
				p.sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 192, 96, 96));  //через объект p класса player меняем спрайт, делая анимацию (используя оператор точку)
			}

			if ((Keyboard::isKeyPressed(Keyboard::Up) || (Keyboard::isKeyPressed(Keyboard::W)))) {
				p.dir = 3; p.speed = 0.1;//направление вниз, см выше
				CurrentFrame += 0.005 * time;
				if (CurrentFrame > 3) CurrentFrame -= 3;
				p.sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 307, 96, 96));  //через объект p класса player меняем спрайт, делая анимацию (используя оператор точку)
			}

			if ((Keyboard::isKeyPressed(Keyboard::Down) || (Keyboard::isKeyPressed(Keyboard::S)))) { //если нажата клавиша стрелка влево или англ буква А
				p.dir = 2; p.speed = 0.1;//направление вверх, см выше
				CurrentFrame += 0.005 * time; //служит для прохождения по "кадрам". переменная доходит до трех суммируя произведение времени и скорости. изменив 0.005 можно изменить скорость анимации
				if (CurrentFrame > 3) CurrentFrame -= 3; //проходимся по кадрам с первого по третий включительно. если пришли к третьему кадру - откидываемся назад.
				p.sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 0, 96, 96)); //проходимся по координатам Х. получается 96,96*2,96*3 и опять 96
			}
			// getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY()); //уравление камерой
		}

		sf::Vector2i localPosition = Mouse::getPosition(window);

		std::cout << "locPos" << localPosition.x << std::endl;

		if (localPosition.x < 3) { view.move(-0.2 * time, 0); }//если слева - влево
		if (localPosition.x > window.getSize().x - 3) { view.move(0.2 * time, 0); }//правый край-вправо
		if (localPosition.y > window.getSize().y - 3) { view.move(0, 0.2 * time); }//нижний край - вниз
		if (localPosition.y < 3) { view.move(0, -0.2 * time); }//верхний край - вверх
		
		p.update(time);

		window.setView(view); //установка камеры на игрока
		window.clear();       //перерисовка

		for (int i = 0; i < HEIGHT_MAP; i++)
			for (int j = 0; j < WIDTH_MAP; j++) {
				if (TileMap[i][j] == ' ') s_map.setTextureRect(IntRect(0, 0, 32, 32));
				if (TileMap[i][j] == 's') s_map.setTextureRect(IntRect(32, 0, 32, 32));//камень
				if ((TileMap[i][j] == '0')) s_map.setTextureRect(IntRect(64, 0, 32, 32));
				if ((TileMap[i][j] == 'f')) s_map.setTextureRect(IntRect(96, 0, 32, 32));//цветок
				if ((TileMap[i][j] == 'h')) s_map.setTextureRect(IntRect(128, 0, 32, 32));//сердечко
				

				s_map.setPosition(j * 32, i * 32);
				window.draw(s_map);
			}
		if (!showMissionText) {
			text.setPosition(view.getCenter().x + 125, view.getCenter().y - 130);//позиция всего этого текстового блока
			s_quest.setPosition(view.getCenter().x + 115, view.getCenter().y - 130);//позиция фона для блока			
			window.draw(s_quest); window.draw(text); //рисуем спрайт свитка (фон для текста миссии). а затем и текст. все это завязано на логическую переменную, которая меняет свое состояние от нажатия клавиши ТАБ
		}

		window.draw(p.sprite); //отрисовка самого игрока (спрайта)
		window.display();      //вывод информации на экран после рендеринга
	}

	return 0;
}