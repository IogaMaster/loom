mod loom_debugger;
mod loom_log;

use log::*;
use mlua::prelude::*;

pub fn init_modules(lua: &Lua) {
    let loom = lua
        .create_table()
        .expect("Failed to create global `loom` table");

    match loom_log::init_loom_log(lua) {
        Ok(exports) => {
            info!("Module {} loaded", "loom_log");
            let _ = loom.set("log", exports);
        }
        Err(error) => error!(
            "Module {} failed to load. With error: {}",
            "loom_log", error
        ),
    };

    match loom_debugger::init_loom_debugger(lua) {
        Ok(exports) => {
            info!("Module {} loaded", "loom_debugger");
            let _ = loom.set("debugger", exports);
        }
        Err(error) => error!(
            "Module {} failed to load. With error: {}",
            "loom_debugger", error
        ),
    };

    let _ = lua.globals().set("loom", loom);
}
