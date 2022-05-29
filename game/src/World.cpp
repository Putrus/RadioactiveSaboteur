#include "../include/World.h"
#include "../include/Math.h"
#include <cassert>

World::World()
{
   m_collision_items.reserve(32);
}

void World::add(CollisionItem* shape)
{
   m_collision_items.push_back(shape);
}

void World::remove(CollisionItem* collision_item)
{
   //auto it = std::find_if(m_collision_items.begin(), m_collision_items.end(), &collision_item,
   auto it = std::find(m_collision_items.begin(), m_collision_items.end(), collision_item);
   assert(it != m_collision_items.end());
   if (it != m_collision_items.end())
   {
      m_collision_items.erase(it);
   }
}

inline bool handleCollisionOfTwoAABB(const AABB& aabb1, const AABB& aabb2, CollisionInfo& result)
{
   return std::max(aabb1.left, aabb2.left) <= std::min(aabb1.right, aabb2.right) &&
      std::max(aabb1.top, aabb2.top) <= std::min(aabb1.bottom, aabb2.bottom);
   // wrong!!!
   //if (aabb1.right > aabb2.left && aabb1.right < aabb2.right)
   //   return true;
   //if (aabb1.left > aabb2.left && aabb1.left < aabb2.right)
   //   return true;
   //if (aabb1.bottom > aabb2.top && aabb1.bottom < aabb2.bottom)
   //   return true;
   //if (aabb1.top > aabb2.top && aabb1.top < aabb2.bottom)
   //   return true;

   return false;
}

bool handleCollisionOfTwoAABB(const CollisionItem& aabb1, const CollisionItem& aabb2, CollisionInfo& result)
{
   return handleCollisionOfTwoAABB(aabb1.shape.aabb, aabb2.shape.aabb, result);
}

bool handleCollisionOfTwoSpheres(const CollisionItem& sphere1, const CollisionItem& sphere2, CollisionInfo& result)
{
   assert(false);//not implemented
   return false;
   //sf::Vector2f offset = sphere2.shape.sphere.center - sphere1.shape.sphere.center;
   //if (length(offset) < sphere1.shape.sphere.radius + sphere2.shape.sphere.radius)
   //{
   //   return true;
   //}
   //else
   //   return false;
}

bool handleCollisionSphereVsAABB(const CollisionItem& sphere, const CollisionItem& aabb, CollisionInfo& result)
{
   assert(false);//not implemented
   return false;
}

bool handleCollisionAABBvsLine(const CollisionItem& sphere, sf::Vector2f line_p1, sf::Vector2f line_p2, CollisionInfo& result)
{
   assert(false);
   return false;
}

bool handleCollisionSphereVsLine(sf::Vector2f position, float radius, sf::Vector2f line_p1, sf::Vector2f line_p2, CollisionInfo& result)
{
   sf::Vector2f u = line_p2 - line_p1;
   float dist1 = math::distance(line_p1, position);
   float dist2 = math::distance(line_p2, position);
   float min_dist = std::min(dist1, dist2);
   if (min_dist < radius)
   {
      sf::Vector2f offset = u;
      math::set_length(u, std::max(min_dist - 1, 0.f));
      result.point = line_p1 + u;
      return true;
   }
   else
      return false;

   //// u = p1 - p0
   //// q0 - point
   //// q1 - closest point on u
   //// p0 + (p1 - p0) * t
   //// t = - ((p0 - q0) dot (p1 - p0)) / u dot u
   //// jeœli t z poza zakreso 0-1 to mniejsza odleg³oœæ punktu z koñcem odcinka - NIEEEE
   //sf::Vector2f u = line_p2 - line_p1;
   //assert(math::length(u) > 0);
   //float t = -math::dot_product((line_p1 - position), u) / math::dot_product(u, u);

   //if (t <= 0.0f)
   //{
   //   if (math::distance(line_p1, position) < radius)
   //   {
   //      result.point = line_p1;
   //      return true;
   //   }
   //   else
   //      return false;
   //}
   //else if (t >= 1.0f)
   //{
   //   if (math::distance(line_p2, position) < radius)
   //   {
   //      result.point = line_p2;
   //      return true;
   //   }
   //   else
   //      return false;
   //}
   //else
   //{
   //   result.point = line_p1 + u * t;
   //   return true;
   //}
}

bool handleCollisionSphereVsLine(const CollisionItem& sphere, sf::Vector2f line_p1, sf::Vector2f line_p2, CollisionInfo& result)
{
   return handleCollisionSphereVsLine(sf::Vector2f(sphere.shape.sphere.x, sphere.shape.sphere.y), sphere.shape.sphere.radius,
      line_p1, line_p2, result);
}

bool World::checkSingleCollision(const CollisionItem* object, CollisionInfo& result) const
{
   bool collision = false;
   for (const CollisionItem* item : m_collision_items)
   {
      if (item == object)
         continue;

      // todo performance, use 2d array of test functions
      bool local_collision = false;
      if (object->shape.type == CollisionShapeType::AxisAlignedBoundingBox)
      {
         if (item->shape.type == CollisionShapeType::AxisAlignedBoundingBox)
            local_collision = handleCollisionOfTwoAABB(*object, *item, result);
         else if (item->shape.type == CollisionShapeType::Sphere)
            local_collision = handleCollisionSphereVsAABB(*item, *object, result);
         else
            assert(false);//not implemented
      }
      else if (object->shape.type == CollisionShapeType::Sphere)
      {
         if (item->shape.type == CollisionShapeType::AxisAlignedBoundingBox)
            local_collision = handleCollisionSphereVsAABB(*object, *item, result);
         else if (item->shape.type == CollisionShapeType::Sphere)
            local_collision = handleCollisionOfTwoSpheres(*object, *item, result);
         else
            assert(false);//not implemented
      }
      else
         assert(false);//not implemented

      // todo don't stop but test other items for closer collision
      if (local_collision)
      {
         result.item1 = object;
         result.item2 = item;
         collision = true;
         break;
      }
   }

   return collision;
}

bool World::checkSingleCollision(sf::Vector2f line_p1, sf::Vector2f line_p2, CollisionInfo& result) const
{
   bool collision = false;
   for (const CollisionItem* item : m_collision_items)
   {
      // todo performance, use 2d array of test functions
      bool local_collision = false;
      if (item->shape.type == CollisionShapeType::AxisAlignedBoundingBox)
         local_collision = handleCollisionAABBvsLine(*item, line_p1, line_p2, result);
      else if (item->shape.type == CollisionShapeType::Sphere)
         local_collision = handleCollisionSphereVsLine(*item, line_p1, line_p2, result);
      else
         assert(false);//not implemented

      // todo don't stop but test other items for closer collision
      if (local_collision)
      {
         result.item1 = item;
         result.item2 = nullptr;
         collision = true;
         break;
      }
   }

   return collision;
}

bool World::checkSingleCollision(const AABB& aabb, CollisionInfo& result) const
{
   bool collision = false;
   for (const CollisionItem* item : m_collision_items)
   {
      // todo performance, use 2d array of test functions
      bool local_collision = false;
      if (item->shape.type == CollisionShapeType::AxisAlignedBoundingBox)
         local_collision = handleCollisionOfTwoAABB(aabb, item->shape.aabb, result);
      //else if (item->shape.type == CollisionShapeType::Sphere)
      //   local_collision = handleCollisionSphereVsLine(*item, line_p1, line_p2, result);
      else
         assert(false);//not implemented

      // todo don't stop but test other items for closer collision
      if (local_collision)
      {
         result.item1 = item;
         result.item2 = nullptr;
         collision = true;
         break;
      }
   }

   return collision;
}
