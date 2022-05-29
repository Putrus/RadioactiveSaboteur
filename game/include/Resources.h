#pragma once

#include <SFML/Graphics.hpp>
#include "ResourceHolder.h"

enum TextureID {
   TEX_HERO1, TEX_HERO2, TEX_TREE1, TEX_BACKGROUND, TEX_BARREL
};

typedef ResourceHolder<sf::Texture, TextureID> TextureHolder;

enum MaterialType
{
   MT_Hero = 0,

   // pickable objects
   MT_Barrel = 5,

   // non-moving objects
   MT_Tree = 20,
   MT_Wall,
   MT_Water
};
