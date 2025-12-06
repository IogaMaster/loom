mod modules;
use crate::modules::*;
use mlua::prelude::*;
use simplelog::*;
use std::fs::File;

fn main() {
    let lua = init();
    lua.load("loom.log.info('tester')").exec();
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
    let lua = Lua::new();
    init_modules(&lua);

    lua
}
