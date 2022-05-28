#pragma once

#include <SFML/Graphics.hpp>
#include "ResourceHolder.h"

enum TextureID {
   HERO1,
   HERO2
};

typedef ResourceHolder<sf::Texture, TextureID> TextureHolder;