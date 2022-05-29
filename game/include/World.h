#pragma once

#include <SFML/System.hpp>
#include <vector>

enum class CollisionShapeType
{
	AxisAlignedBoundingBox,
	Sphere,
	Line
};

struct AABB
{
	float left, right, top, bottom;
};

inline AABB make_aabb(float left, float top, float width, float height)
{
	return { left, left + width, top, top + height };
}

inline void offset_aabb(AABB &aabb, float x, float y)
{
	aabb.left += x;
	aabb.right += x;
	aabb.top += y;
	aabb.bottom += y;
}

inline void offset_aabb(AABB& aabb, float value)
{
	offset_aabb(aabb, value, value);
}

struct Sphere
{
	float x, y, radius;
};

struct Line
{
	float x1, y1, x2, y2;
};

struct CollisionShape
{
	CollisionShapeType type;
	union
	{
		AABB aabb;
		Sphere sphere;
		Line line;
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
	sf::Vector2f point;
	sf::Vector2f normal;
	const CollisionItem *item1, *item2;
};

class World
{
public:
	World();

	void add(CollisionItem* collision_item);
	void remove(CollisionItem* collision_item);
	bool checkSingleCollision(const CollisionItem* object, CollisionInfo& result) const;
	bool checkSingleCollision(sf::Vector2f line_p1, sf::Vector2f line_p2, CollisionInfo& result) const;
	bool checkSingleCollision(const AABB& aabb, CollisionInfo& result) const;
	int checkManyCollisions(const AABB& aabb, CollisionInfo* results, int results_max) const;

private:
	std::vector<CollisionItem*> m_collision_items;
   const sf::Time m_time_per_frame;
};