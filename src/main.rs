mod core;
mod modules;
use std::{io::Write, thread::sleep};

use crate::{core::game_loop::game_loop, modules::*};
use glfw::{Action, Context, Key};
use log::*;
use mlua::prelude::*;
use simplelog::*;
use std::{fs::File, path::Path};

fn main() -> LuaResult<()> {
    let lua = init();
    lua.load(Path::new("main.lua")).exec()?;

    match lua.load("loom.init()").eval() {
        Ok(()) => (),
        Err(_) => debug!("No init function found, skipping..."),
    };

    let mut glfw = glfw::init(glfw::fail_on_errors).unwrap();
    let (mut window, events) = glfw
        .create_window(1080, 720, "", glfw::WindowMode::Windowed)
        .expect("Failed to create GLFW window.");

    window.set_key_polling(true);
    window.set_resizable(true);
    window.make_current();

    game_loop(
        &60.0,
        || {
            let _ = lua.load("loom.tick()").exec();
        },
        |dt| {
            let _ = lua.load(format!("loom.frame({})", dt)).exec();

            glfw.poll_events();
            for (_, event) in glfw::flush_messages(&events) {
                handle_window_event(&mut window, event);
            }
            window.swap_buffers();
            window.should_close()
        },
    );

    match lua.load("loom.shutdown()").exec() {
        Ok(()) => (),
        Err(_) => debug!("No shutdown function found, skipping..."),
    };

    Ok(())
}

fn handle_window_event(window: &mut glfw::Window, event: glfw::WindowEvent) {
    match event {
        glfw::WindowEvent::Key(Key::Escape, _, Action::Press, _) => window.set_should_close(true),
        _ => {}
    }
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
