
#include "loom/utils/sparse_set.hpp"
#include <bitset>
#include <cstdint>
#include <memory>
#include <vector>

namespace loom::ecs {

/**
 * @brief Maximum number of unique component types allowed in a World.
 */
constexpr size_t maxComponents = 2048;

using Entity = uint64_t;
using ComponentMask = std::bitset<maxComponents>;

/**
 * @class World
 * @brief Represents a self-contained ECS world.
 *
 * Each World manages its own set of entities, component pools, and entity
 * masks. Multiple worlds can coexist without interfering with each other.
 */
class World {
  /// Sparse set of active entities
  utils::SparseSet<Entity> entities;

  /// Stack of available (deleted) entity IDs for reuse
  std::vector<Entity> availableEntities;

  /// Component pools, stored as type-erased SparseSets
  std::vector<std::unique_ptr<utils::ISparseSet>> componentPool;

  /// Component mask per entity
  utils::SparseSet<ComponentMask> entityMasks;

  /**
   * @brief Returns the next unique component index.
   *
   * Used internally to assign a unique bit for each component type.
   *
   * @return Unique index for a new component type.
   */
  static auto get_next_component_index() -> size_t {
    static size_t next = 0;
    return next++;
  }

  /**
   * @brief Get or assign a unique index for a component type.
   *
   * Each component type is associated with a unique bit in ComponentMask.
   *
   * @tparam T Component type
   * @return Unique index for this component type
   */
  template <typename T> auto get_component_index() -> size_t {
    static size_t iden = get_next_component_index();
    return iden;
  }

  /**
   * @brief Get or create the SparseSet pool for a component type.
   *
   * Allocates a pool if one doesn't exist for the requested type.
   *
   * @tparam T Component type
   * @return Reference to the pool of components of type T
   */
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
  /**
   * @brief Create a new entity.
   *
   * Reuses a deleted entity ID if available, otherwise allocates a new ID.
   * Initializes a blank component mask for the entity.
   *
   * @return ID of the new entity
   */
  auto create_entity() -> Entity {
    Entity entity = 0;
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

  /**
   * @brief Delete an entity and remove all its components.
   *
   * Also adds the entity ID to the reuse list.
   *
   * @param entity The entity ID to delete
   */
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

  /**
   * @brief Add one or more components to an entity.
   *
   * Updates the entity's component mask.
   *
   * @tparam ComponentType Variadic component types
   * @param entity The entity to add components to
   * @param components Components to add
   */
  template <typename... ComponentType>
  auto add(Entity entity, ComponentType &&...components) -> void {
    (
        [&](auto &&component) -> void {
          using Comp = std::decay_t<decltype(component)>;
          utils::SparseSet<Comp> &pool = get_or_create_pool<Comp>();
          pool.insert(entity, std::forward<decltype(component)>(component));

          auto *maskPtr = *entityMasks.get(entity);
          maskPtr->set(get_component_index<Comp>(), true);
        }(std::forward<ComponentType>(components)),
        ...);
  }

  /**
   * @brief Remove one or more components from an entity.
   *
   * Updates the entity's component mask.
   *
   * @tparam ComponentType Variadic component types
   * @param entity The entity to remove components from
   */
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

  /**
   * @brief Reset the world to an empty state.
   *
   * Clears all entities, component pools, masks, and available entity list.
   */
  auto reset() -> void {
    entities.clear();
    availableEntities.clear();
    componentPool.clear();
    entityMasks.clear();
  }

  // --------- VIEW ---------

  /**
   * @class View
   * @brief Query entities that match a set of components in the World.
   *
   * Supports optional filters like `without` (exclude certain components) and
   * `only` (match entities with exactly the requested components).
   *
   * @tparam Components... The component types to query
   */
  template <typename... Components> class View {
    World *world;                          ///< Pointer to the parent World
    std::vector<size_t> withoutComponents; ///< Components to exclude
    bool onlyFlag = false;                 ///< Only match exact component sets

  public:
    View(World *wrld) : world(wrld) {}

    /**
     * @brief Exclude entities that have any of the specified components.
     *
     * @tparam Excluded... Component types to exclude
     * @return Reference to this View (for chaining)
     */
    template <typename... Excluded> auto without() -> View & {
      withoutComponents = {world->get_component_index<Excluded>()...};
      return *this;
    }

    /**
     * @brief Only match entities that have exactly the requested components.
     *
     * @return Reference to this View (for chaining)
     */
    auto only() -> View & {
      onlyFlag = true;
      return *this;
    }

    /**
     * @brief Iterate over all matching entities and invoke a function.
     *
     * Precomputes a list of entities matching the requested components,
     * optionally excluding or restricting to exact component sets.
     *
     * @tparam Func Callable type accepting references to the components
     * @param func Function to call for each matching entity
     */
    template <typename Func> auto each(Func func) const -> void {
      std::vector<Entity> matchedEntities;

      for (auto entity : world->entities.get_dense()) {
        ComponentMask mask = **world->entityMasks.get(entity);
        bool hasAll =
            (mask.test(world->get_component_index<Components>()) && ...);
        if (!hasAll) {
          continue;
        }

        bool hasExcluded = false;
        for (auto idx : withoutComponents) {
          if (mask.test(idx)) {
            hasExcluded = true;
            break;
          }
        }
        if (hasExcluded) {
          continue;
        }

        if (onlyFlag && mask.count() != sizeof...(Components)) {
          continue;
        }

        matchedEntities.push_back(entity);
      }

      for (auto entity : matchedEntities) {
        func(entity, (world->get_or_create_pool<Components>()
                          .get_ref(entity)
                          ->get())...);
      }
    }
  };

  /**
   * @brief Create a view for the requested component types.
   *
   * @tparam Components... Component types to query
   * @return View object for iteration
   */
  template <typename... Components> auto view() -> View<Components...> {
    return View<Components...>(this);
  }
};

}; // namespace loom::ecs
