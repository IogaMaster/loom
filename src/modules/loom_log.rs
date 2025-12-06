use log::*;
use mlua::prelude::*;

pub fn init_loom_log(lua: &Lua) -> LuaResult<LuaTable> {
    let exports = lua.create_table()?;
    let info = lua.create_function(|_, msg: String| -> LuaResult<()> {
        info!("[Lua] {}", msg);
        Ok(())
    })?;

    exports.set("info", info);
    Ok(exports)
}
