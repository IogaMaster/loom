---@meta

--- @class loom
loom = {}

---
--- Callback function used once to initialize the game.
--- WARNING: Must be overridden by user code.
---
--- @alias loom.init fun()
--- @type loom.init
loom.init = nil

---
--- Callback function used to update on a fixed time scale
--- WARNING: Must be overridden by user code.
---
--- @alias loom.tick fun()
--- @type loom.tick
loom.tick = nil

---
--- Callback function used to draw to the screen
--- WARNING: must be overridden by user code.
---
--- @alias loom.draw fun(dt: number)
--- @type loom.draw
loom.draw = nil

---
--- Callback function used to shutdown and free resources
--- WARNING: Must be overridden by user code
---
--- @alias loom.shutdown fun()
--- @type loom.shutdown
loom.shutdown = nil
