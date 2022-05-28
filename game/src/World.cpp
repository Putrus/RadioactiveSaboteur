#include "../include/World.h"
#include <cassert>

World::World()
{
   m_collision_items.reserve(32);
}

void World::add(const CollisionItem& shape)
{
   m_collision_items.emplace_back(shape);
}

void World::remove(const CollisionItem& collision_item)
{
   auto it = std::find(m_collision_items.begin(), m_collision_items.end(), &collision_item);
   assert(it != m_collision_items.end());
   if (it != m_collision_items.end())
   {
      m_collision_items.erase(it);
   }
}

bool handleCollisionOfTwoAABB(const CollisionItem& aabb1, const CollisionItem& aabb2, CollisionInfo& result)
{
   if (aabb1.shape.aabb.rigth > aabb2.shape.aabb.left && aabb1.shape.aabb.rigth < aabb2.shape.aabb.rigth)
      return true;
   if (aabb1.shape.aabb.left > aabb2.shape.aabb.left && aabb1.shape.aabb.left < aabb2.shape.aabb.rigth)
      return true;
   if (aabb1.shape.aabb.bottom > aabb2.shape.aabb.top && aabb1.shape.aabb.bottom < aabb2.shape.aabb.bottom)
      return true;
   if (aabb1.shape.aabb.top > aabb2.shape.aabb.top && aabb1.shape.aabb.top < aabb2.shape.aabb.bottom)
      return true;

   return false;
}

bool handleCollisionOfTwoSpheres(const CollisionItem& sphere1, const CollisionItem& sphere2, CollisionInfo& result)
{
   assert(false);//not implemented
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

bool World::checkSingleCollision(const CollisionItem* object, CollisionInfo& result) const
{
   bool collision = false;
   for (const CollisionItem& item : m_collision_items)
   {
      if (&item == object)
         continue;

      // todo performance, use 2d array of test functions
      bool local_collision = false;
      if (object->shape.type == CollisionShapeType::AxisAlignedBoundingBox)
      {
         if (item.shape.type == CollisionShapeType::AxisAlignedBoundingBox)
            local_collision = handleCollisionOfTwoAABB(*object, item, result);
         else if (item.shape.type == CollisionShapeType::Sphere)
            local_collision = handleCollisionSphereVsAABB(item, *object, result);
         else
            assert(false);//not implemented
      }
      else if (object->shape.type == CollisionShapeType::Sphere)
      {
         if (item.shape.type == CollisionShapeType::AxisAlignedBoundingBox)
            local_collision = handleCollisionSphereVsAABB(*object, item, result);
         else if (item.shape.type == CollisionShapeType::Sphere)
            local_collision = handleCollisionOfTwoSpheres(*object, item, result);
         else
            assert(false);//not implemented
      }
      else
         assert(false);//not implemented

      // todo don't stop but test other items for closer collision
      if (local_collision)
      {
         result.item1 = object;
         result.item2 = &item;
         collision = true;
         break;
      }
   }

   return collision;
}
