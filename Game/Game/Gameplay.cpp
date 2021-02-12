#include <SFML/Graphics.hpp>
#include "ENTITY.h"
#include "GlobalValue.h"
#include "level.h"
#include "menu.h"
#include "LifeBar.h"
#include "score.h"
#include <vector>
#include <list>
#include "TinyXML/tinyxml.h"
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>



using namespace sf;
using namespace std;


string NamePlayer;
string PlayerScore;

void Exit()
{
	view.reset(FloatRect(0, 0, 1500, 800)); //камера вида
	window.setView(view);
	Vector2u center = window.getSize();
	int x = center.x / 2;
	int y = center.y / 2;
	RectangleShape shape(Vector2f(500, 300));
	shape.setFillColor(Color::White);
	shape.setOutlineThickness(2);
	shape.setOutlineColor(Color::Black);
	shape.setOrigin(shape.getGlobalBounds().width / 2, shape.getGlobalBounds().height / 2);
	shape.setPosition(x, y);
	Text end;
	end.setString(L"Выйти из игры?");
	end.setFont(font);
	end.setCharacterSize(25);
	end.setOutlineColor(Color::Black);
	end.setFillColor(Color::Black);
	end.setOrigin(end.getGlobalBounds().width / 2, end.getGlobalBounds().height / 2);
	end.setPosition(x, y - 50);
	MENU yes(L"Да", shape.getGlobalBounds().left + 100, y + 20);
	MENU no(L"Нет", shape.getGlobalBounds().left + shape.getGlobalBounds().width - 100, y + 20);
	int state = 0;
	while(true)
	{
		yes.update(Color::Black);
		no.update(Color::Black);

		state = 0;

		if (yes.getRectPunctMenu().contains(Mouse::getPosition(window)))
		{
			yes.update(Color::Red);
			state = 1;
		}
		if (no.getRectPunctMenu().contains(Mouse::getPosition(window))) 
		{
			no.update(Color::Red);
			state = 2;
		}

		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (state == 1)
			{
				exit(0);
			}
			if (state == 2)
			{
				return;
			}
		}
		window.draw(shape);
		window.draw(end);
		window.draw(yes.text);
		window.draw(no.text);
		window.display();
	}
}

bool choicePunctMenu(int i, int status)
{
	Vector2u center = window.getSize();
	int x = center.x / 2;
	int y = center.y / 2;
	bool back = false;

	if (i == 1)
	{
		if (status == 0)
		{
			window.setTitle(L"Mario. Новая игра");
			window.clear(Color(255, 255, 255));
			MENU Back(L"Назад", 80, 20);

			Text NameText(L"Введите имя", font, 100);
			NameText.setOutlineColor(Color::Black);
			NameText.setFillColor(Color::Black);
			NameText.setOrigin(NameText.getGlobalBounds().width / 2, NameText.getGlobalBounds().height / 2);
			NameText.setPosition(x, 100);

			Text ValueName;
			ValueName.setFont(font);
			ValueName.setCharacterSize(50);
			ValueName.setOutlineColor(Color::Black);
			ValueName.setFillColor(Color::Black);

			MENU Ok(L"Далее", x, y + ValueName.getGlobalBounds().height + 150);

			char *name = new char[20]();
			char key;
			int count = 0;
			int code = 0;
			int ok = 0;
			while (!Keyboard::isKeyPressed(Keyboard::Enter))
			{
				Event event;
				while (window.pollEvent(event))
				{
					if (Keyboard::isKeyPressed(Keyboard::Tab) || (Mouse::isButtonPressed(Mouse::Left) && back))
					{
						window.setTitle(L"Mario. Главное меню");
						return true;
					}
					if (event.type == Event::Closed)
					{
						Exit();
					}

					Back.update(Color::Black);
					Ok.update(Color::Black);
					back = false;

					if (Back.getRectPunctMenu().contains(Mouse::getPosition(window))) 
					{
						Back.update(Color::Red);
						back = true;
					}

					if (Ok.getRectPunctMenu().contains(Mouse::getPosition(window)))
					{
						Ok.update(Color::Red);
						ok = 1;
					}

					if(Mouse::isButtonPressed(Mouse::Left) && ok == 1)
					{
						NamePlayer = ValueName.getString();
						return false;
					}

					if (count <= 20)
					{
						if (event.type == Event::TextEntered)
						{
							key = event.text.unicode;
							cout << key;
							name[count] = key;
							count += 1;
							ValueName.setString((string)name);
						}
					}

					if (Keyboard::isKeyPressed(Keyboard::BackSpace))
					{
						if (count > 0)
						{
							count -= 1;
							name[count] = '\0';
						}
					}

					if (Keyboard::isKeyPressed(Keyboard::Escape))
					{
						Exit();
					}

					ValueName.setPosition(x - (int)(count / 2) * 35, y);
					window.clear(Color::White);
					window.draw(Back.text);
					window.draw(Ok.text);
					window.draw(NameText);
					window.draw(ValueName);
					window.display();
				}
			}
			if (code == 0)
			{
				NamePlayer = ValueName.getString();
				return false;
			}
		}

		if (status == 1)
		{
			return false;
		}
	}
	if (i == 2)
	{
		MENU Back(L"Назад", 80, 20);
		window.setTitle(L"Mario. Таблица рекордов");
		Text TextAbout;
		TextAbout.setFont(font);
		TextAbout.setCharacterSize(25);
		TextAbout.setOutlineColor(Color::Black);
		TextAbout.setFillColor(Color::Black);
		int dx = 0;
		int count = 0;
		fstream f;
		f.open("Таблица рекордов.txt");
		if (!f.is_open())
			cout << "Файл не может быть открыт!\n";
		else
		{
			while (!f.eof())
			{
				string s;
				getline(f, s);
				count++;
			}
		}
		f.close();
		f.open("Таблица рекордов.txt");
		string s;
		string *items = new string[count];
		for (int k = 0; k < count; k++)
		{
			getline(f, s);
			items[k] = s;
		}
		f.close();
		while (!Keyboard::isKeyPressed(Keyboard::Tab))
		{
			s = "";
			int l = 0;
			if (count > 20)
			{
				l = dx + 20;
			}
			else
			{
				l = count;
			}
			for (int k = dx; k < l; k++)
			{
				s += items[k] + "\n";
			}
			TextAbout.setString(s);

			Back.update(Color::Black);
			back = false;

			if (Back.getRectPunctMenu().contains(Mouse::getPosition(window))) {
				Back.update(Color::Red);
				back = true;
			}

			if (Mouse::isButtonPressed(Mouse::Left) && back)
			{
				window.setTitle(L"Mario. Главное меню");
				break;
			}

			if (Keyboard::isKeyPressed(Keyboard::Escape))
			{
				Exit();
			}

			if (count > 20)
			{
				if (Keyboard::isKeyPressed(Keyboard::Down))
				{
					if (dx < count - 20)
					{
						dx += 1;
					}
				}
				if (Keyboard::isKeyPressed(Keyboard::Up))
				{
					if (dx > 0)
					{
						dx -= 1;
					}
				}
			}
			TextAbout.setOrigin(TextAbout.getGlobalBounds().width / 2, TextAbout.getGlobalBounds().height / 2);
			TextAbout.setPosition(x, y);
			window.clear(Color::White);
			window.draw(TextAbout);
			window.draw(Back.text);
			window.display();
		}
	}
	if (i == 3)
	{
		window.setTitle(L"Mario. Правила игры");
		MENU Back(L"Назад", 80, 20);

		Texture Regulations;
		Regulations.loadFromFile("Image\\Правила игры.png");

		Sprite regulations(Regulations);

		regulations.setTextureRect(IntRect(0, 0, 1200, 735));
		regulations.setOrigin(regulations.getGlobalBounds().width /2, regulations.getGlobalBounds().height / 2);
		regulations.setPosition(x, y);
		
		while (!Keyboard::isKeyPressed(Keyboard::Tab))
		{
			if (Keyboard::isKeyPressed(Keyboard::Escape))
			{
				Exit();
			}

			Back.update(Color::Black);
			back = false;

			if (Back.getRectPunctMenu().contains(Mouse::getPosition(window))) {
				Back.update(Color::Red);
				back = true;
			}

			if (Mouse::isButtonPressed(Mouse::Left) && back)
			{
				window.setTitle(L"Mario. Главное меню");
				break;
			}
			window.clear(Color::White);
			window.draw(Back.text);
			window.draw(regulations);
			window.display();
		}
	}
	if (i == 4)
	{
		window.setTitle(L"Mario. О программе");
		MENU Back(L"Назад", 80, 20);
		Text TextAbout;
		TextAbout.setFont(font);
		TextAbout.setCharacterSize(25);
		TextAbout.setOutlineColor(Color::Black);
		TextAbout.setFillColor(Color::Black);
		TextAbout.setString(L"Курсовая работа \n по предмету 'Компьютерная графика' \n SFML C++ \n Разработчик: \n Студентка группы 643 Паршина Анна Романовна");
		TextAbout.setOrigin(TextAbout.getGlobalBounds().width / 2, TextAbout.getGlobalBounds().height / 2);
		TextAbout.setPosition(x, y - 50);
		while (!Keyboard::isKeyPressed(Keyboard::Tab))
		{
			if (Keyboard::isKeyPressed(Keyboard::Escape))
			{
				Exit();
			}
			Back.update(Color::Black);
			back = false;

			if (Back.getRectPunctMenu().contains(Mouse::getPosition(window))) {
				Back.update(Color::Red);
				back = true;
			}

			if (Mouse::isButtonPressed(Mouse::Left) && back)
			{
				window.setTitle(L"Mario. Главное меню");
				break;
			}
			window.clear(Color::White);
			window.draw(Back.text);
			window.draw(TextAbout);
			window.display();
		}
	}
	if (i == 5) { Exit(); }

	return true;
}

int menu(int status)
{
	window.setTitle(L"Mario. Главное меню");
	view.reset(FloatRect(0, 0, 1500, 800)); //камера вида
	window.setView(view);
	Vector2u center = window.getSize();
	int x = center.x / 2;
	int y = center.y / 2;

	MENU Start("", x, y);

	if (status == 0) Start.text.setString(L"Новая игра");
	else Start.text.setString(L"Продолжить");
	Start.text.setOrigin(Start.text.getGlobalBounds().width / 2, Start.text.getGlobalBounds().height / 2);
	Start.text.setPosition(x, y);

	MENU TableRecord(L"Таблица рекордов", x, y + 40);
	MENU Regulations(L"Правила игры", x, y + 2 * 40);
	MENU Information(L"О программе ", x, y + 3 * 40);
	MENU Close(L"Выход", x, y + 4 * 40);

	Texture MarioText;
	MarioText.loadFromFile("Image\\MarioText.png");
	Sprite mariotext(MarioText);

	mariotext.setTextureRect(IntRect(0, 0, 695, 321));
	mariotext.setPosition(x - (695 / 2), 50);

	bool Menu = true;
	int MenuNum = 0;
	window.clear();

	//меню
	while (Menu)
	{
		Start.update(Color::Black);
		TableRecord.update(Color::Black);
		Regulations.update(Color::Black);
		Information.update(Color::Black);
		Close.update(Color::Black);

		MenuNum = 0;

		if (Start.getRectPunctMenu().contains(Mouse::getPosition(window)))
		{
			Start.update(Color::Red);
			MenuNum = 1;
		}
		if (TableRecord.getRectPunctMenu().contains(Mouse::getPosition(window))) {
			TableRecord.update(Color::Red);
			MenuNum = 2;
		}
		if (Regulations.getRectPunctMenu().contains(Mouse::getPosition(window))) {
			Regulations.update(Color::Red);
			MenuNum = 3;
		}
		if (Information.getRectPunctMenu().contains(Mouse::getPosition(window)))
		{
			Information.update(Color::Red);
			MenuNum = 4;
		}
		if (Close.getRectPunctMenu().contains(Mouse::getPosition(window)))
		{
			Close.update(Color::Red);
			MenuNum = 5;
		}

		if (Mouse::isButtonPressed(Mouse::Left) && MenuNum != 0)
		{
			bool state = choicePunctMenu(MenuNum, status);
			if (MenuNum != 1 && !state) return 1;
			if (MenuNum == 1 && !state) break;
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			Exit();
		}

		window.clear(Color::White);
		window.draw(mariotext);
		window.draw(Start.text);
		window.draw(TableRecord.text);
		window.draw(Regulations.text);
		window.draw(Information.text);
		window.draw(Close.text);
		window.display();
	}
	return 0;
}

void iterectionsEntitis(PLAYER &p, list<ENTITY*> &entities, float time)
{
	list<ENTITY*>::iterator it;//второй итератор.для взаимодействия между объектами списка
	for (it = entities.begin(); it != entities.end();)//говорим что проходимся от начала до конца
	{
		ENTITY *b = *it;//для удобства, чтобы не писать (*it)->
		b->update(time);//вызываем ф-цию update для всех объектов (по сути для тех, кто жив)
		if (b->life == false) { it = entities.erase(it); delete b; }// если этот объект мертв, то удаляем его
		else it++;//и идем курсором (итератором) к след объекту. так делаем со всеми объектами списка
	}

	for (it = entities.begin(); it != entities.end(); it++)//проходимся по эл-там списка
	{
		if ((*it)->getRect().intersects(p.getRect()))//если прямоугольник спрайта объекта пересекается с игроком
		{
			if (((*it)->name == "Platform") && ((*it)->getRect().intersects(p.getRect())))//если игрок столкнулся с объектом списка и имя этого объекта movingplatform
			{
				ENTITY *movPlat = *it;
				if ((p.dy > 0) || (p.onGround == false))//при этом игрок находится в состоянии после прыжка, т.е падает вниз
					if (p.rect.top + p.rect.height < movPlat->rect.top + movPlat->rect.height)//если игрок находится выше платформы, т.е это его ноги минимум (тк мы уже проверяли что он столкнулся с платформой)
					{
						p.rect.top = movPlat->rect.top - p.rect.height + 3; p.rect.left += movPlat->dx*time; p.dy = 0; p.onGround = true;
					// то выталкиваем игрока так, чтобы он как бы стоял на платформе
					}
			}
			if ((*it)->name == "easyEnemy1" || (*it)->name == "easyEnemy2")
			{
				if ((p.dy > 0) && (!p.onGround)) { (*it)->dx = 0; p.dy = -0.3; p.onGround = false; (*it)->health = 0; p.playerScore += 150; return; }//если прыгнули на врага,то даем врагу скорость 0,отпрыгиваем от него чуть вверх,даем ему здоровье 0
				else 
				{
					p.health -= 70;	//иначе враг подошел к нам сбоку и нанес урон
					if ((*it)->dx > 0)//если враг идет вправо
					{
						(*it)->rect.left = p.rect.left - (*it)->rect.width - 2; //отталкиваем его от игрока влево (впритык)
						(*it)->dx *= -1;//меняем направление
					}
					else if ((*it)->dx < 0)//если враг идет влево
					{
						(*it)->rect.left = p.rect.left + p.rect.width + 2; //аналогично - отталкиваем вправо
						(*it)->dx *= -1;//останавливаем
					}
					if (p.dx < 0) { p.rect.left = (*it)->rect.left + (*it)->rect.width + 2; p.dx *= -1; p.dy = -0.3; p.onGround = false; }//если столкнулись с врагом и игрок идет влево то выталкиваем игрока
					else if (p.dx > 0) { p.rect.left = (*it)->rect.left - p.rect.width - 2; p.dx *= -1; p.dy = -0.3; p.onGround = false;}//если столкнулись с врагом и игрок идет вправо то выталкиваем игрока

				}
			}
			if ((*it)->name == "Coin") 
			{
				(*it)->health = 0;
				p.playerScore += 50;	
			}
			if ((*it)->name == "Kristall")
			{
				(*it)->health = 0;
				p.playerScore += 200;
			}
			if ((*it)->name == "Key")
			{
				(*it)->health = 0;
				p.key = true;
			}

		}
	}
}

int startGame()
{
	window.setTitle(L"Mario");
	view.reset(FloatRect(0,0,500,500)); //камера вида
	view.setSize(2500.f, 1250.f);

	Level lvl; //создали экземпляр класса уровень
	lvl.LoadFromFile("map.tmx"); //загрузили в него карту, внутри класса с помощью методов он ее обработает.
	
#pragma region Player

	Image ImagePlayer;
	ImagePlayer.loadFromFile("Image\\Mario.png");
	ImagePlayer.createMaskFromColor(Color::White);

	Texture TexturePlayer;
	TexturePlayer.loadFromImage(ImagePlayer);

	Object player = lvl.GetObject("Player");//объект игрока на нашей карте.задаем координаты игроку в начале при помощи него

	PLAYER p = PLAYER(TexturePlayer, lvl, "Player1", player.rect.left, player.rect.top, 155, 260);

#pragma endregion

#pragma region Enemy

	Image ImageEnemy1;
	ImageEnemy1.loadFromFile("Image\\Enemy.png");
	ImageEnemy1.createMaskFromColor(Color(255, 255, 255));

	Texture TextureEnemy1;
	TextureEnemy1.loadFromImage(ImageEnemy1);

	Image ImageEnemy2;
	ImageEnemy2.loadFromFile("Image\\EasyEnemy.png");
	ImageEnemy2.createMaskFromColor(Color::White);

	Texture TextureEnemy2;
	TextureEnemy2.loadFromImage(ImageEnemy2);

	list<ENTITY*>  entities;//создаю список, сюда буду кидать объекты.например врагов.
	list<ENTITY*>::iterator it;//итератор чтобы проходить по эл-там списка
	vector<Object> e = lvl.GetObjects("easyEnemy1");//все объекты врага на tmx карте хранятся в этом векторе

	for (int i = 0; i < e.size(); i++)//проходимся по элементам этого вектора(а именно по врагам)
		entities.push_back(new ENEMY(TextureEnemy1, lvl, "easyEnemy1", e[i].rect.left, e[i].rect.top, 170, 200));//и закидываем в список всех наших врагов с карты

	e = lvl.GetObjects("easyEnemy2"); 

	for (int i = 0; i < e.size(); i++)
		entities.push_back(new ENEMY(TextureEnemy2, lvl, "easyEnemy2", e[i].rect.left, e[i].rect.top, 180, 170));

#pragma endregion

#pragma region Coin

	Image ImageItem;
	ImageItem.loadFromFile("Image\\tilemap.png");
	ImageItem.createMaskFromColor(Color::White);

	Texture TextureCoin;
	TextureCoin.loadFromImage(ImageItem);

	e = lvl.GetObjects("Coin");

	for (int i = 0; i < e.size(); i++)
		entities.push_back(new ITEM(TextureCoin, lvl, "Coin", e[i].rect.left, e[i].rect.top, 46, 46));
#pragma endregion

#pragma region Kristall

	Texture TextureKristall;
	TextureKristall.loadFromImage(ImageItem);

	e = lvl.GetObjects("Kristall");

	for (int i = 0; i < e.size(); i++)
		entities.push_back(new ITEM(TextureKristall, lvl, "Kristall", e[i].rect.left, e[i].rect.top, 28, 46));
#pragma endregion

#pragma region Key

	Texture TextureKey;
	TextureKey.loadFromImage(ImageItem);

	e = lvl.GetObjects("Key");

	for (int i = 0; i < e.size(); i++)
		entities.push_back(new ITEM(TextureKey, lvl, "Key", e[i].rect.left, e[i].rect.top, 33, 65));
#pragma endregion

#pragma region MovingPlatform
	Image ImageMovingPlatform;
	ImageMovingPlatform.loadFromFile("Image\\Platform.png");
	ImageMovingPlatform.createMaskFromColor(Color::White);

	Texture TextureMovingPlatform;
	TextureMovingPlatform.loadFromImage(ImageMovingPlatform);
	e = lvl.GetObjects("Platform");

	for (int i = 0; i < e.size(); i++)
		entities.push_back(new MOVINFPLATFORM(TextureMovingPlatform, lvl, "Platform", e[i].rect.left, e[i].rect.top, 90 * 5, 45));
#pragma endregion

#pragma region RegulationPlayer;
	Image ImageRegulationPlayer;
	ImageRegulationPlayer.loadFromFile("Image\\Управление.png");
	ImageRegulationPlayer.createMaskFromColor(Color::White);

	Texture TextureRegulationPlayer;
	TextureRegulationPlayer.loadFromImage(ImageRegulationPlayer);

	Sprite Regilation(TextureRegulationPlayer);

	e = lvl.GetObjects("RegulationsPlayer");

	Regilation.setTextureRect(IntRect(0, 0, 1220, 200));

	Regilation.setPosition(e[0].rect.left, e[0].rect.top + 20);

#pragma endregion

#pragma region RegilationMenu;
	Image ImageRegilationMenu;
	ImageRegilationMenu.loadFromFile("Image\\Управление меню.png");
	ImageRegilationMenu.createMaskFromColor(Color::White);

	Texture TextureRegilationMenu;
	TextureRegilationMenu.loadFromImage(ImageRegilationMenu);

	Sprite RegilationMenu(TextureRegilationMenu);

	RegilationMenu.setTextureRect(IntRect(0, 0, 350, 190));

#pragma endregion

	e = lvl.GetObjects("End");

	LifeBar lifebar;
	Score score;

	Clock clock;
	
	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();

		time = time / 1000;

		if (time > 20) time = 20;

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				Exit();
				view.reset(FloatRect(0, 0, 500, 500)); //камера вида
				view.setSize(2500.f, 1250.f);
			}
				
			if (event.type == sf::Event::Resized)//при изменении окна, не меняется размер игравого поля, а просто меняется камера вида
			{
				FloatRect visibleArea(0.f, 0.f, event.size.width, event.size.height);
				window.setView(View(visibleArea));
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			Exit();
			view.reset(FloatRect(0, 0, 500, 500)); //камера вида
			view.setSize(2500.f, 1250.f);
		}

		if(Keyboard::isKeyPressed(Keyboard::Tab))
		{
			int result = menu(1);
			view.reset(FloatRect(0, 0, 500, 500)); //камера вида
			view.setSize(2500.f, 1250.f);
			if (result == 1)
			{
				Exit();
			}
		}

		window.setView(view);

		p.update(time);
		view.setCenter(p.rect.left, p.rect.top + 80);
		iterectionsEntitis(p, entities, time);
		
		window.clear(Color::White);

		if (!p.life || (p.rect.intersects(e[0].rect) && p.key))
		{
			Vector2f center = window.getView().getCenter();
			Text GameOver(L"Конец игры", font, 50);
			if (p.key) GameOver.setString(L"Конец игры \n Вы выиграли");
			GameOver.setOutlineColor(Color::Black);
			GameOver.setFillColor(Color::Black);
			GameOver.setPosition(center.x - 100, center.y - 50);
			window.draw(GameOver);
			window.display();
			chrono::seconds dura(3);
			this_thread::sleep_for(dura);
			PlayerScore = "" + to_string(p.playerScore);
			return 1;
		}
		else
		{
			lvl.Draw(window);
			for (it = entities.begin(); it != entities.end(); it++)
				window.draw((*it)->sprite); //рисуем entities объекты
			
			window.draw(p.sprite);
			window.draw(Regilation);
		
			Vector2f center = window.getView().getCenter();

			lifebar.draw(p.health, center, window);
			score.draw(p.playerScore, center);
			RegilationMenu.setPosition(center.x - RegilationMenu.getGlobalBounds().width - 750, center.y - 600);
			window.draw(RegilationMenu);
		}
		window.display();
	}
}

int main()
{
	window.create(VideoMode(1500, 800), "Mario");
	font.loadFromFile("Font.ttf");
	while (true)
	{
		int rezult = menu(0);
		if(rezult != 0) return 0;
		rezult = startGame();
		if (rezult == 0) return 0;
		if (rezult == 1)
		{
			fstream f;
			f.open("Таблица рекордов.txt");
			if (!f.is_open())
				cout << "Файл не может быть открыт!\n";
			else
			{
				f << NamePlayer << ' ' << PlayerScore << '\n';
			}
			f.close();
		}
	}
	return 0;
}