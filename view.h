#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

View view;
//функция для привязки камеры к игроку
View getplayercoordinateforview(float x, float y) { 
	float tempX = x;
	float tempY = y;
	//ограничение передвижения камеры около границы карты
	if (x < 320) tempX = 320;
	if (y < 240) tempY = 240;
	if (y > 554) tempY = 554;

	//центровка камеры по координатам игрока
	view.setCenter(tempX, tempY);
	return view;
}