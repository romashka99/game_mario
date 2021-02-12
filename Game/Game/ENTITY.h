#include <SFML/Graphics.hpp>
#include "level.h"
using namespace sf;
using namespace std;

class ENTITY
{
public:
	float dx, dy, speed;
	int health;
	bool life, onGround;
	vector<Object> obj;//вектор объектов карты
	FloatRect rect;
	Sprite sprite;
	Texture texture;
	String name;
	ENTITY(Texture &texture, String Name, float X, float Y, int W, int H)
	{
		this->texture = texture;
		rect = FloatRect(X, Y, W, H);
		name = Name;
		speed = 0; health = 100; dx = 0; dy = 0;
		life = true; onGround = false;
		sprite.setTexture(texture);
	}

	FloatRect getRect()
	{ //ф-ция получения прямоугольника. его коорд,размеры (шир,высот).
		return rect; //эта ф-ция нужна для проверки столкновений 
	}
	virtual void update(float time) = 0;
	virtual void Collision(float Dx, float Dy) = 0;
};

class PLAYER : public ENTITY
{
private:
	enum statusPlayer { motion, jump, stay };
public:
	bool key;
	statusPlayer state;
	float currentFrame;
	int playerScore;

	PLAYER(Texture &texture, Level &lev, String Name, float X, float Y, int W, int H) : ENTITY(texture, Name, X, Y, W, H)
	{
		playerScore = 0; state = stay; obj = lev.GetAllObjects();//инициализируем.получаем все объекты для взаимодействия персонажа с картой
		currentFrame = 0;
		if (name == "Player1") {
			sprite.setTextureRect(IntRect(0, 0, rect.width, rect.height));
			health = 210;
			speed = dx = 0;
			key = false;
		}
	}


	void update(float time)
	{
		control();

		rect.left += speed * time;
		Collision(speed, 0);


		if (!onGround) dy = dy + 0.004*time;
		rect.top += dy * time;
		onGround = false;
		Collision(0, dy);

		currentFrame += time * 0.015;;

		sprite.setPosition(rect.left, rect.top);

		speed = 0;
		switch (state)
		{
		case stay:
		{
			if (dx > 0)
			{
				sprite.setTextureRect(IntRect(0, 0, rect.width, rect.height));
			}
			if (dx < 0)
			{
				sprite.setTextureRect(IntRect(0 + rect.width, 0, -rect.width, rect.height));
			}
		}
		break;
		case motion:
		{
			if (currentFrame > 3) currentFrame = 1;

			if (dx > 0)
			{
				sprite.setTextureRect(IntRect(rect.width * int(currentFrame), 0, rect.width, rect.height));
			}
			if (dx < 0)
			{
				sprite.setTextureRect(IntRect(rect.width * int(currentFrame) + rect.width, 0, -rect.width, rect.height));
			}
		}
		break;
		case jump:
		{
			if (dx > 0)
			{
				sprite.setTextureRect(IntRect(rect.width * 4, 0, rect.width, rect.height));
			}
			if (dx < 0)
			{
				sprite.setTextureRect(IntRect(rect.width * 5, 0, -rect.width, rect.height));
			}
		}
		break;
		default:
			break;
		}
		sprite.setPosition(rect.left, rect.top);
		if (health <= 0) { life = false; }
	}

	void control()
	{

		if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left))
		{
			speed = dx = -0.9;
			if (state == stay) state = motion;
		}

		if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right))
		{
			speed = dx = 0.9;
			if (state == stay) state = motion;
		}

		if ((Keyboard::isKeyPressed(Keyboard::Space)|| Keyboard::isKeyPressed(Keyboard::Up)) && onGround )
		{
			dy = -1.9;
			state = jump;
			onGround = false;
		}
	}


	void Collision(float Dx, float Dy)
	{
		for (int i = 0; i < obj.size(); i++)//проходимся по объектам
		{
			if (getRect().intersects(obj[i].rect))
			{
				if (obj[i].name == "Solid")//если встретили препятствие
				{
					if (Dy < 0) { rect.top = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
					if (Dy > 0) { rect.top = obj[i].rect.top - rect.height;  dy = 0; onGround = true; state = stay;}
					if (Dx > 0) { rect.left = obj[i].rect.left - rect.width; }
					if (Dx < 0) { rect.left = obj[i].rect.left + obj[i].rect.width; }
				}
				if (obj[i].name == "End")
				{
					if (!key)
					{
						if (Dy < 0) { rect.top = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
						if (Dy > 0) { rect.top = obj[i].rect.top - rect.height;  dy = 0; onGround = true; state = stay;  }
						if (Dx > 0) { rect.left = obj[i].rect.left - rect.width; }
						if (Dx < 0) { rect.left = obj[i].rect.left + obj[i].rect.width; }
					}
				}
				if (FloatRect(rect.left, rect.top + rect.height, rect.width, 2).intersects(obj[i].rect) && obj[i].name == "SolidPlat")
				{
					if (dy > 0) { rect.top = obj[i].rect.top - rect.height; dy = 0; onGround = true; state = stay; }
				}
			}
		}	
	}
};

class ENEMY : public ENTITY
{
public:
	float currentFrame;
	float moveTimer;

	ENEMY(Texture &texture, Level &lvl, String Name, float X, float Y, int W, int H) : ENTITY(texture, Name, X, Y, W, H)
	{
		obj = lvl.GetAllObjects();//инициализируем.получаем объекты для взаимодействия врага с картой
		if (name == "easyEnemy1") {
			sprite.setTextureRect(IntRect(0, 0, rect.width, rect.height));
			dx = 0.4;
			currentFrame = 0;
		}
		if (name == "easyEnemy2") {
			sprite.setTextureRect(IntRect(0, 0, rect.width, rect.height));
			dy = 0.4;
			currentFrame = 0;
			moveTimer = 0;
		}

	}

	void update(float time)
	{
		if (name == "easyEnemy1") {
			rect.left += dx * time;
			Collision(dx, 0);
			currentFrame += time * 0.015;
			sprite.setPosition(rect.left, rect.top);
			if (currentFrame > 3) currentFrame -= 3;
			if (dx < 0)
				sprite.setTextureRect(IntRect(rect.width * int(currentFrame) + rect.width, 0, -rect.width, rect.height));
			if (dx > 0)
				sprite.setTextureRect(IntRect(rect.width * int(currentFrame), 0, rect.width, rect.height));
			if (health <= 0) { life = false; }
		}

		if (name == "easyEnemy2") {
			moveTimer += time;//наращиваем таймер
			if (moveTimer > 200) { dy *= -1; moveTimer = 0; }
			rect.top += dy * time;
			Collision(0, dy);
			currentFrame += time * 0.015;
			sprite.setPosition(rect.left, rect.top);
			if (currentFrame > 14) currentFrame -= 14;
			sprite.setTextureRect(IntRect(rect.width * int(currentFrame), 0, rect.width, rect.height));
			if (health <= 0) { life = false; }

		}
	}

	void Collision(float Dx, float Dy)
	{
		for (int i = 0; i < obj.size(); i++)//проходимся по объектам
			if (getRect().intersects(obj[i].rect))//проверяем пересечение игрока с объектом
			{
				if (obj[i].name == "Solid" || obj[i].name == "Limit" || obj[i].name == "SolidPlat")
				{
					if (Dy > 0) { rect.top = obj[i].rect.top - rect.height;  dy *= -1; moveTimer = 0; }
					if (Dy < 0) { rect.top = obj[i].rect.top + obj[i].rect.height;   dy *= -1; moveTimer = 0; }
					if (Dx > 0) { rect.left = obj[i].rect.left - rect.width;  dx *= -1; }
					if (Dx < 0) { rect.left = obj[i].rect.left + obj[i].rect.width; dx *= -1; }

				}
			}
	}
};

class MOVINFPLATFORM : public ENTITY {//класс движущейся платформы
private:
	float moveTimer;
public:
	MOVINFPLATFORM(Texture &texture, Level &lvl, String Name, float X, float Y, int W, int H) :ENTITY(texture, Name, X, Y, W, H) {
		obj = lvl.GetAllObjects();//инициализируем.получаем объекты для взаимодействия врага с картой
		sprite.setColor(Color::Black);
		sprite.setTextureRect(IntRect(0, 0, W, H));//прямоугольник 
		dx = 0.2;//изначальное ускорение по Х
	}

	void update(float time)//функция обновления платформы.
	{
		rect.left += dx * time;//реализация движения по горизонтали
		moveTimer += time;//наращиваем таймер
		Collision(dx, 0);
		if (moveTimer > 20000) { dx *= -1; moveTimer = 0; }//если прошло примерно 2 сек, то меняется направление движения платформы,а таймер обнуляется
		sprite.setPosition(rect.left, rect.top);//задаем позицию спрайту
	}

	void Collision(float Dx, float Dy)
	{
		for (int i = 0; i < obj.size(); i++)//проходимся по объектам
			if (getRect().intersects(obj[i].rect))
			{
				if (obj[i].name == "Solid" || obj[i].name == "SolidPlat")
				{
					if (Dx > 0) { rect.left = obj[i].rect.left - rect.width;  dx *= -1; moveTimer = 0; }
					if (Dx < 0) { rect.left = obj[i].rect.left + obj[i].rect.width; dx *= -1; moveTimer = 0; }
				}
			}
	}
};

class ITEM : public ENTITY
{
public:
	ITEM(Texture &texture, Level &lvl, String Name, float X, float Y, int W, int H) : ENTITY(texture, Name, X, Y, W, H)
	{
		if (name == "Coin") {
			sprite.setTextureRect(IntRect(21, 472, rect.width, rect.height));
		}
		if (name == "Kristall") {
			sprite.setTextureRect(IntRect(300, 381, rect.width, rect.height));
		}
		if (name == "Key") {
			sprite.setTextureRect(IntRect(209, 373, rect.width, rect.height));
		}
		dx = 0;
	}

	void update(float time)
	{
		sprite.setPosition(rect.left, rect.top);
		if (health <= 0) { life = false; }
	}

	void Collision(float Dx, float Dy)
	{
	}
};

