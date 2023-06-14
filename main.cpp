#include <SFML/Graphics.hpp>
#include "map.h"
#include "view.h"
#include <iostream>
#include <sstream>
#include "mission.h"

using namespace sf;

class Player { //����� ������
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
	Player(const String F, int X, int Y, float W, float H) { //����������� ������ �����
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


	void interactionWithMap()//������� �������������� � ������ 
	{
		for (int i = y / 32; i < (y + h) / 32; i++)
			for (int j = x / 32; j < (x + h) / 32; j++)
			{
				if (TileMap[i][j] == '0')//����������� � ������� � ������
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
				if (TileMap[i][j] == 's')//������������ � ������ (����)
				{
					playerScore++;
					TileMap[i][j] = ' ';
				}
				if (TileMap[i][j] == 'f')//������������ � ������� (-�����)
				{
					health -= 40;
					TileMap[i][j] = ' ';
				}
				if (TileMap[i][j] == 'h')//������������ � ��������� (+�����)
				{
					health += 20;
					TileMap[i][j] = ' ';
				}
			}
	}

};


int main()
{
	RenderWindow window(sf::VideoMode(640, 480), "project Humanity-Complimintation"); //�������� ����
	view.reset(FloatRect(0, 0, 640, 480));

	Font font;
	font.loadFromFile("CyrilicOld.ttf");		//���������� ������
	Text text("", font, 20);		//����� �� ��������� (��� ���)
	text.setFillColor(Color::Black);

	Image map_image; //�������� ����� (�����)
	map_image.loadFromFile("images/map.png");
	Texture map;	 //�������� �����
	map.loadFromImage(map_image);
	Sprite s_map;	 //������ �����
	s_map.setTexture(map);

	Image quest_image;
	quest_image.loadFromFile("images/missionbg.jpg");
	quest_image.createMaskFromColor(Color(0, 0, 0));
	Texture quest_texture;
	quest_texture.loadFromImage(quest_image);
	Sprite s_quest;
	s_quest.setTexture(quest_texture);
	s_quest.setTextureRect(IntRect(0, 0, 340, 510));  //���������� �����, ������� �������� ��������
	s_quest.setScale(0.6f, 0.6f);//���� ��������� ��������, => ������ ���� ������
	
	Player p("hero.png", 250, 250, 96.0, 96.0); //������ ����� ������ �����

	bool showMissionText = true; //����������, ���������� �� ����� ������ �� ������

	float CurrentFrame = 0;
	Clock clock;
	Clock gameTimeClock; //����������, ��������� ����� ����
	int gameTime = 0;
	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();

		if (p.life) gameTime = gameTimeClock.getElapsedTime().asSeconds(); //������� �������
		clock.restart();
		time = time / 400; //�������� ����

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == Event::KeyPressed)//������� ������� �������
				if ((event.key.code == Keyboard::Tab)) {//���� ������� ���


					switch (showMissionText) {//�������������, ����������� �� ���������� ���������� showMissionText

					case true: {
						std::ostringstream playerHealthString;//������ �������� ������
						playerHealthString << p.health; //������� � ������ �������� 
						std::ostringstream task;//������ ������ ������
						task << getTextMission(getCurrentMission(p.getplayercoordinateX()));//���������� ������� getTextMission (��� ���������� ����� ������), ������� ��������� � �������� ��������� ������� getCurrentMission(������������ ����� ������), � ��� ��� �-��� ��������� � �������� ��������� ������� p.getplayercoordinateX() (��� �-��� ���������� ��� ���������� ������)
						text.setString("��������: " + playerHealthString.str() + "\n" + task.str());//������
						text.setPosition(view.getCenter().x + 125, view.getCenter().y - 130);//������� ����� ����� ���������� �����
						s_quest.setPosition(view.getCenter().x + 115, view.getCenter().y - 130);//������� ���� ��� �����
						showMissionText = false;//��� ������ ��������� ������ ��� ��� �� ������ �� ������
						break;//������� , ����� �� ��������� ������� "false" (������� ����)
					}
					case false: {
						text.setString("");//���� �� ������ ������� ���, �� ���� ���� ����� ������
						showMissionText = true;// � ��� ������ ��������� ����� ������ ������� ��� � �������� ����� �� �����
						break;
					}
					}
				}
		}
		if (p.life) 
		{
			if ((Keyboard::isKeyPressed(Keyboard::Left) || (Keyboard::isKeyPressed(Keyboard::A)))) {
				p.dir = 1; p.speed = 0.1;//dir =1 - ����������� �����, speed =0.1 - �������� ��������.
				CurrentFrame += 0.005 * time;
				if (CurrentFrame > 3) CurrentFrame -= 3;
				p.sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 96, 96, 96)); //����� ������ p ������ player ������ ������, ����� �������� (��������� �������� �����)
			}

			if ((Keyboard::isKeyPressed(Keyboard::Right) || (Keyboard::isKeyPressed(Keyboard::D)))) {
				p.dir = 0; p.speed = 0.1;//����������� ������
				CurrentFrame += 0.005 * time;
				if (CurrentFrame > 3) CurrentFrame -= 3;
				p.sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 192, 96, 96));  //����� ������ p ������ player ������ ������, ����� �������� (��������� �������� �����)
			}

			if ((Keyboard::isKeyPressed(Keyboard::Up) || (Keyboard::isKeyPressed(Keyboard::W)))) {
				p.dir = 3; p.speed = 0.1;//����������� ����, �� ����
				CurrentFrame += 0.005 * time;
				if (CurrentFrame > 3) CurrentFrame -= 3;
				p.sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 307, 96, 96));  //����� ������ p ������ player ������ ������, ����� �������� (��������� �������� �����)
			}

			if ((Keyboard::isKeyPressed(Keyboard::Down) || (Keyboard::isKeyPressed(Keyboard::S)))) { //���� ������ ������� ������� ����� ��� ���� ����� �
				p.dir = 2; p.speed = 0.1;//����������� �����, �� ����
				CurrentFrame += 0.005 * time; //������ ��� ����������� �� "������". ���������� ������� �� ���� �������� ������������ ������� � ��������. ������� 0.005 ����� �������� �������� ��������
				if (CurrentFrame > 3) CurrentFrame -= 3; //���������� �� ������ � ������� �� ������ ������������. ���� ������ � �������� ����� - ������������ �����.
				p.sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 0, 96, 96)); //���������� �� ����������� �. ���������� 96,96*2,96*3 � ����� 96
			}
			// getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY()); //��������� �������
		}

		sf::Vector2i localPosition = Mouse::getPosition(window);

		std::cout << "locPos" << localPosition.x << std::endl;

		if (localPosition.x < 3) { view.move(-0.2 * time, 0); }//���� ����� - �����
		if (localPosition.x > window.getSize().x - 3) { view.move(0.2 * time, 0); }//������ ����-������
		if (localPosition.y > window.getSize().y - 3) { view.move(0, 0.2 * time); }//������ ���� - ����
		if (localPosition.y < 3) { view.move(0, -0.2 * time); }//������� ���� - �����
		
		p.update(time);

		window.setView(view); //��������� ������ �� ������
		window.clear();       //�����������

		for (int i = 0; i < HEIGHT_MAP; i++)
			for (int j = 0; j < WIDTH_MAP; j++) {
				if (TileMap[i][j] == ' ') s_map.setTextureRect(IntRect(0, 0, 32, 32));
				if (TileMap[i][j] == 's') s_map.setTextureRect(IntRect(32, 0, 32, 32));//������
				if ((TileMap[i][j] == '0')) s_map.setTextureRect(IntRect(64, 0, 32, 32));
				if ((TileMap[i][j] == 'f')) s_map.setTextureRect(IntRect(96, 0, 32, 32));//������
				if ((TileMap[i][j] == 'h')) s_map.setTextureRect(IntRect(128, 0, 32, 32));//��������
				

				s_map.setPosition(j * 32, i * 32);
				window.draw(s_map);
			}
		if (!showMissionText) {
			text.setPosition(view.getCenter().x + 125, view.getCenter().y - 130);//������� ����� ����� ���������� �����
			s_quest.setPosition(view.getCenter().x + 115, view.getCenter().y - 130);//������� ���� ��� �����			
			window.draw(s_quest); window.draw(text); //������ ������ ������ (��� ��� ������ ������). � ����� � �����. ��� ��� �������� �� ���������� ����������, ������� ������ ���� ��������� �� ������� ������� ���
		}

		window.draw(p.sprite); //��������� ������ ������ (�������)
		window.display();      //����� ���������� �� ����� ����� ����������
	}

	return 0;
}