# ECS

## Views
Views are the way that loom shows the systems entities. So you can iterate over a view to process them, they have a simple api.
```cpp
auto view = ecs.view<Position, Velocity>(); // This defines a list of all entities that have Position and Velocity. They could have other components as well.
auto view_without = ecs.view<Position, Velocity>().without<NoMoveTag>(); // All entities that have a Position and Velocity, but do NOT have NoMoveTag.
auto view_only = ecs.view<Position, Velocity>().only(); // All entities that ONLY have Position and Velocity.

view.each([=](auto &pos, auto &vel){
        pos.x += vel.x;
})
```

## Systems
Loom uses lambdas interface for building a system.
They are built from two parts:
- The outer (builder) lambda. It builds a view upfront.
- The inner (logic) lambda. It is the code that is run on each entity.

This api allows for an expressive way to bundle a system with the query it depends on.

```cpp
auto movement_system = [view = ecs.view<Position, Velocity>()]() {
    view.each([=](auto& p, auto& v) {
        p.x += v.x;
        p.y += v.y;
    });
};
// Register the system so the engine can handle calling it in the main loop.
ecs.register_system(loom::ecs::TICK, movement_system) // Runs per tick
ecs.register_system(loom::ecs::FRAME, movement_system) // Runs per frame
```

There are a few other ways you can run a system.

```cpp
// Inline the system builder
[view = ecs.view<Position, Velocity>()]() {
    view.each([=](auto& p, auto& v) {
        p.x += v.x;
        p.y += v.y;
    });
}(); // Call the builder and run the system now
```

```cpp
// Manually build the view and run the system
// NOTE: This is not preferred, as it decouples the system and view it depends on.
auto view = ecs.view<Position, Velocity>();
view.each([](auto& p, auto& v) {
    p.x += v.x;
    p.y += v.y;
});
```


