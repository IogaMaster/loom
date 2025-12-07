mod core;
mod modules;
use std::{io::Write, thread::sleep};

use crate::{core::game_loop::game_loop, modules::*};
use mlua::prelude::*;
use simplelog::*;
use std::{fs::File, path::Path};

fn main() -> LuaResult<()> {
    let lua = init();
    lua.load(Path::new("main.lua")).exec()?;

    match lua.load("loom.init()").exec() {
        Ok(()) => (),
        Err(_) => (),
    };

    game_loop(
        &60.0,
        || match lua.load("loom.tick()").exec() {
            Ok(()) => (),
            Err(_) => (),
        },
        |dt| match lua.load(format!("loom.frame({})", dt)).exec() {
            Ok(()) => (),
            Err(_) => (),
        },
    );

    match lua.load("loom.shutdown()").exec() {
        Ok(()) => (),
        Err(_) => (),
    };

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
