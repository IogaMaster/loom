use log::*;
use mlua::prelude::*;

pub fn init_loom_debug(lua: &Lua) -> LuaResult<LuaTable> {
    let exports = lua.create_table()?;
    let debugger: LuaTable = lua.load(include_str!("./debugger.lua")).eval()?;
    exports.set("debugger", debugger)?;
    Ok(exports)
}
