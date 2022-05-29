#pragma once

#include <SFML/Graphics.hpp>
#include "ResourceHolder.h"

enum TextureID {
   TEX_HERO1, TEX_HERO2, TEX_TREE1, TEX_BACKGROUND, TEX_BARREL, TEX_MENU
};

enum FontID {
   FONT_PIXEL
};

typedef ResourceHolder<sf::Texture, TextureID> TextureHolder;
typedef ResourceHolder<sf::Font, FontID> FontHolder;

enum MaterialType
{
   MT_Hero = 0,

   // pickable objects
   MT_Barrel = 5,

   // non-moving objects
   MT_Tree = 20,
   MT_Wall,
   MT_Water = 69
};
