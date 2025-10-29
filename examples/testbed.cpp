#include <loom/loom.hpp>

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
  auto entity1 = world.create_entity();

  world.add(entity1, Position{}, Velocity{});

  auto movementSystem = [view = world.view<Position, Velocity>()]() -> void {
    view.each([](Position &pos, Velocity &vel) -> void {
      pos.x += vel.x;
      pos.y += vel.y;
    });
  };
  movementSystem();
}
