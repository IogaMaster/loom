mod modules;
use crate::modules::*;
use mlua::prelude::*;
use simplelog::*;
use std::{fs::File, path::Path};

fn main() -> LuaResult<()> {
    let lua = init();
    lua.load(Path::new("main.lua")).exec()?;
    Ok(())
}

fn init() -> Lua {
    CombinedLogger::init(vec![
        TermLogger::new(
            LevelFilter::Info,
            ConfigBuilder::new()
                .set_time_offset_to_local()
                .unwrap()
                .build(),
            TerminalMode::Mixed,
            ColorChoice::Auto,
        ),
        WriteLogger::new(
            LevelFilter::Trace,
            ConfigBuilder::new()
                .set_time_format_rfc2822()
                .set_time_offset_to_local()
                .unwrap()
                .build(),
            File::create("latest.log").unwrap(),
        ),
    ])
    .unwrap();

    let lua = unsafe { Lua::unsafe_new() };
    init_modules(&lua);

    lua
}
