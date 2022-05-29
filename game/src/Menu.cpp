#include "../include/Menu.h"

Menu::Menu(sf::Texture& texture) :
   m_start(new Button(sf::IntRect(0, 0, 192, 64), texture, sf::Vector2f(864.f, 316.f))),
   m_credits(new Button(sf::IntRect(0, 64, 192, 64), texture, sf::Vector2f(864.f, 390.f))),
   m_exit(new Button(sf::IntRect(0, 128, 192, 64), texture, sf::Vector2f(864.f, 460.f))),
   m_is_visible(true)
{
   

}
void Menu::draw(sf::RenderWindow& window) {
   window.draw(m_start->getSprite());
   window.draw(m_exit->getSprite());
   window.draw(m_credits->getSprite());
}

void Menu::update(const sf::Vector2i mouse_position) {
   m_start->advise(mouse_position);
   m_credits->advise(mouse_position);
   m_exit->advise(mouse_position);
}

bool Menu::isVisible() {
   return m_is_visible;
}

void Menu::setVisible(bool visible) {
   m_is_visible = visible;
}