#pragma once

#include <cassert>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>

template <typename Resource, typename Identifier>
class ResourceHolder {
public:
   Resource* load(Identifier id, const char* filename) {
      //create and load resource
      std::unique_ptr<Resource> resource(new Resource());
      if (!resource->loadFromFile(filename)) {
         std::string str_filename(filename);
         throw std::runtime_error("ResourceHolder::load - Failed to load " + str_filename);
      }
      //insert resource to map
      insertResource(id, std::move(resource));
      return m_resource_map[id].get();
   }

   template <typename Parameter>
   Resource* load(Identifier id, const char* filename, const Parameter& second_param) {
      //create and load resource
      std::unique_ptr<Resource> resource(new Resource());
      if (!resource->loadFromFile(filename, second_param)) {
         std::string str_filename(filename);
         throw std::runtime_error("ResourceHolder::load - Failed to load " + str_filename);
      }
      //insert resource to map
      insertResource(id, std::move(resource));
      return m_resource_map[id].get();
   }

   Resource& get(Identifier id) {
      return const_cast<Resource&>(const_cast<const ResourceHolder*>(this)->get(id));
   }

   const Resource& get(Identifier id) const {
      auto found = m_resource_map.find(id);
      assert(found != m_resource_map.end());

      return *found->second;
   }

private:
   void insertResource(Identifier id, std::unique_ptr<Resource> resource) {
      auto inserted = m_resource_map.insert(std::make_pair(id, std::move(resource)));
      assert(inserted.second);
   }

private:
   std::map<Identifier, std::unique_ptr<Resource>>	m_resource_map;
};