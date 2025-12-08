mod loom_debug;
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

    match loom_debug::init_loom_debug(lua) {
        Ok(exports) => {
            info!("Module {} loaded", "loom_debug");
            let _ = loom.set("debug", exports);
        }
        Err(error) => error!(
            "Module {} failed to load. With error: {}",
            "loom_debug", error
        ),
    };

    let _ = lua.globals().set("loom", loom);
}
