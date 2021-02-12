#include <SFML/Graphics.hpp>
#include <string>

using namespace sf;
using namespace std;

class MENU
{
public:
	Text text;
	MENU(String textMenu, float x, float y)
	{
		text.setFont(font);
		text.setString(textMenu);
		text.setCharacterSize(25);
		text.setOrigin(text.getGlobalBounds().width/2, text.getGlobalBounds().height / 2);
		text.setPosition(x, y);

	}
	void update(Color color)
	{
		text.setOutlineColor(color);
		text.setFillColor(color);
	}

	IntRect getRectPunctMenu()
	{
		return IntRect(text.getGlobalBounds());
	}

};