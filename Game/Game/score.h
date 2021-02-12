#include <SFML/Graphics.hpp>
#include <string>

using namespace sf;
using namespace std;

class Score
{
public:
	Font font;
	Text textScore;
	Score()
	{
		font.loadFromFile("Font.ttf");
		textScore.setFont(font);
		textScore.setCharacterSize(50);
		textScore.setOutlineColor(Color::Black);
		textScore.setFillColor(Color::Black);
	}

	void update(int k, float x, float y)
	{
		textScore.setString(L"—чет: " + to_string(k));
		textScore.setPosition(x + 880, y - 599);
	}

	void draw(int k, Vector2f &center)
	{
		update(k, center.x, center.y);
		window.draw(textScore);
	}

};