
#include "loom/utils/sparse_set.hpp"

#include <bitset>
#include <cstdint>
#include <memory>
#include <typeindex>
#include <unordered_map>
namespace loom::ecs {
constexpr size_t maxComponents = 2048;
using Entity = uint64_t;
using ComponentMask = std::bitset<maxComponents>;

template <typename... Components> class View;
class World;

class World {
  utils::SparseSet<Entity> entities;
  std::vector<Entity> availableEntities;
  std::vector<std::unique_ptr<utils::ISparseSet>> componentPool;
  utils::SparseSet<ComponentMask> entityMasks;

  static auto get_next_component_index() -> size_t {
    static size_t next = 0;
    return next++;
  }

  template <typename T> auto get_component_index() -> size_t {
    static size_t id = get_next_component_index(); // NOLINT
    return id;
  }

  template <typename T> auto get_or_create_pool() -> utils::SparseSet<T> & {
    auto index = get_component_index<T>();

    if (index >= componentPool.size()) {
      componentPool.resize(index + 1);
    }

    if (!componentPool[index]) {
      componentPool[index] = std::make_unique<utils::SparseSet<T>>();
    }

    return *static_cast<utils::SparseSet<T> *>(componentPool[index].get());
  }

public:
  auto create_entity() -> Entity {
    Entity entity;
    if (!availableEntities.empty()) {
      entity = availableEntities.back();
      availableEntities.pop_back();
    } else {
      entity = entities.size();
    }

    entities.insert(entity, entity);
    entityMasks.insert(entity, {});

    return entity;
  }

  auto delete_entity(Entity entity) -> void {
    for (auto &pool : componentPool) {
      if (pool) {
        pool->remove(entity);
      }
    }

    entityMasks.remove(entity);
    entities.remove(entity);
    availableEntities.push_back(entity);
  }

  template <typename... ComponentType>
  auto add(Entity entity, ComponentType &&...components) -> void {
    (
        [&](auto &&component) -> void {
          using Comp = std::decay_t<decltype(component)>;
          utils::SparseSet<Comp> &pool = get_or_create_pool<Comp>();
          pool.insert(entity, std::forward<decltype(component)>(component));

          // Keep exactly the same style as your original single-component
          // version
          auto *maskPtr = *entityMasks.get(entity);
          maskPtr->set(get_component_index<Comp>(), true);
        }(std::forward<ComponentType>(components)),
        ...);
  }

  template <typename... ComponentType> auto remove(Entity entity) -> void {
    (
        [&]() -> void {
          using Comp = ComponentType;
          utils::SparseSet<Comp> &pool = get_or_create_pool<Comp>();
          pool.remove(entity);

          auto *maskPtr = *entityMasks.get(entity);
          maskPtr->set(get_component_index<Comp>(), false);
        }(),
        ...);
  }
  template <typename... Components> auto view() -> View<Components...> {
    return View<Components...>(this);
  }

  auto reset() -> void {
    entities.clear();
    availableEntities.clear();
    componentPool.clear(); // deletes the SparseSet objects on the heap too
    entityMasks.clear();
  }

  // --------- VIEW ---------

  template <typename... Components> class View {
    World *world;
    std::vector<size_t> withoutComponents;
    bool onlyFlag = false;

  public:
    View(World *wrld) : world(wrld) {}

    template <typename... Excluded> auto without() -> View & {
      withoutComponents = {world->get_component_index<Excluded>()...};
      return *this;
    }

    auto only() -> View & {
      onlyFlag = true;
      return *this;
    }

    // TODO: each
  };
};

}; // namespace loom::ecs
