#pragma once

#include <SFML/System.hpp>
#include <vector>

enum class CollisionShapeType
{
	AxisAlignedBoundingBox,
	Sphere,
	Line
};

struct CollisionShape
{
	CollisionShapeType type;
	union
	{
		struct AABB
		{
			float left, rigth, top, bottom;
		} aabb;
		struct Sphere
		{
			float x, y, radius;
		} sphere;
		struct Line
		{
			float x1, y1, x2, y2;
		} line;
	};
};

struct CollisionItem
{
	CollisionShape shape;
	int material_type;
	void* custom_data;
};

struct CollisionInfo
{
	sf::Vector2f position;
	sf::Vector2f collisionVector;
	const CollisionItem *item1, *item2;
};

class World
{
public:
	World();

	void add(const CollisionItem& collision_item);
	void remove(const CollisionItem& collision_item);
	bool checkSingleCollision(const CollisionItem* object, CollisionInfo& result) const;

private:
	std::vector<CollisionItem> m_collision_items;
   const sf::Time m_time_per_frame;
};