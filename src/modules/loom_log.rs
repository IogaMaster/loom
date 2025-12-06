use log::*;
use mlua::prelude::*;

pub fn init_loom_log(lua: &Lua) -> LuaResult<LuaTable> {
    let exports = lua.create_table()?;

    let trace = lua.create_function(|_, msg: String| -> LuaResult<()> {
        trace!("[Lua] {}", msg);
        Ok(())
    })?;
    exports.set("trace", trace)?;

    let debug = lua.create_function(|_, msg: String| -> LuaResult<()> {
        debug!("[Lua] {}", msg);
        Ok(())
    })?;
    exports.set("debug", debug)?;

    let info = lua.create_function(|_, msg: String| -> LuaResult<()> {
        info!("[Lua] {}", msg);
        Ok(())
    })?;
    exports.set("info", info)?;

    let warn = lua.create_function(|_, msg: String| -> LuaResult<()> {
        warn!("[Lua] {}", msg);
        Ok(())
    })?;
    exports.set("warn", warn)?;

    let error = lua.create_function(|_, msg: String| -> LuaResult<()> {
        error!("[Lua] {}", msg);
        Ok(())
    })?;
    exports.set("error", error)?;

    Ok(exports)
}
