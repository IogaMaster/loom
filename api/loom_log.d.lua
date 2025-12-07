---@meta _

--- The `loom.log` library provides a native logging solution
--- @class loom.log
loom.log = {}

--- Log at the trace level
---@param msg string The log message to provide
function loom.log.trace(msg) end

--- Log at the debug level
---@param msg string The log message to provide
function loom.log.debug(msg) end

--- Log at the info level
---@param msg string The log message to provide
function loom.log.info(msg) end

--- Log at the warn level
---@param msg string The log message to provide
function loom.log.warn(msg) end

--- Log at the error level
---@param msg string The log message to provide
function loom.log.error(msg) end
