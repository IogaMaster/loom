#include <iostream>
#include <loom/loom.hpp>
#include <ranges>

struct Position {
  int x = 0;
  int y = 0;
};

struct Velocity {
  int x = 0;
  int y = 0;
};

auto main() -> int {
  auto world = loom::ecs::World();

  for (int i : std::ranges::views::iota(0, 100000)) {
    auto entity = world.create_entity();
    world.add(entity, Position{}, Velocity{});
  }

  auto movementSystem = [view = world.view<Position, Velocity>()]() -> void {
    view.each(
        [](loom::ecs::Entity entity, Position &pos, Velocity &vel) -> void {
          pos.x += vel.x;
          pos.y += vel.y;
        });
  };
  movementSystem();
}
