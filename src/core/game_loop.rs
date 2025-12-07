use std::time::{Duration, Instant};

pub fn game_loop<T: FnMut(), F: FnMut(f32)>(ticks_per_second: &f32, mut tick: T, mut frame: F) {
    let mut time_step = 1.0 / ticks_per_second;
    let mut delta_time = 0.0;
    let mut accumulator = 0.0;

    let mut last_time = Instant::now();

    loop {
        let mut current_time = Instant::now();
        delta_time = (current_time - last_time).as_secs_f32();
        last_time = current_time;
        accumulator += delta_time;
        while accumulator >= time_step {
            tick();
            accumulator -= time_step;
        }
        frame(delta_time)
    }
}
