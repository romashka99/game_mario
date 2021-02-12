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
		bar.setFillColor(Color::Green);//������� �������������
		max = 210;
	}

	void update(int k, float x, float y)// k-������� ��������
	{
		if (k >= 0 && k <= max)
			bar.setSize(Vector2f(88, k));//���� �� ������������ � ��� ���� ������ ���������, �� ������������� ����� �������� (����� ������) ��� �������� ��������������
		s.setPosition(x - 1200, y - 600);
		bar.setPosition(x - 1199, y - 599);
	}

	void draw(int k, Vector2f &center, RenderWindow &window)
	{
		update(k, center.x, center.y);
		window.draw(s);//������� ������ ������� ��������
		window.draw(bar);//������ �� ��� ������ �������������, �� ��� �� ��������� �
	}
};
