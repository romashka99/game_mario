#include <SFML/Graphics.hpp>
using namespace sf;

class LifeBar
{
public:
	Image image;
	Texture t;
	Sprite s;
	int max;
	RectangleShape bar;

	LifeBar()
	{
		image.loadFromFile("Image\\LifeBar.png");
		image.createMaskFromColor(Color(255, 255, 255));
		t.loadFromImage(image);
		s.setTexture(t);
		s.setTextureRect(IntRect(0, 0, 90, 310));
		bar.setFillColor(Color::Green);//зеленый прямоугольник
		max = 210;
	}

	void update(int k, float x, float y)// k-текущее здоровье
	{
		if (k >= 0 && k <= max)
			bar.setSize(Vector2f(88, k));//если не отрицательно и при этом меньше максимума, то устанавливаем новое значение (новый размер) для зеленого прямоугольника
		s.setPosition(x - 1200, y - 600);
		bar.setPosition(x - 1199, y - 599);
	}

	void draw(int k, Vector2f &center, RenderWindow &window)
	{
		update(k, center.x, center.y);
		window.draw(s);//сначала рисуем полоску здоровья
		window.draw(bar);//поверх неё уже черный прямоугольник, он как бы покрывает её
	}
};
