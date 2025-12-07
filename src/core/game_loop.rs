use std::time::{Duration, Instant};

pub fn game_loop<T: FnMut(), F: FnMut(f32)>(ticks_per_second: &f32, mut tick: T, mut frame: F) {
    let mut tick_delta = 1.0 / ticks_per_second;
    let mut tick_timer = Instant::now();

    let mut delta_time = 0.0;

    loop {
        let frame_timer = Instant::now();
        if tick_timer.elapsed().as_secs_f32() > tick_delta {
            tick();
            tick_delta = 1.0 / ticks_per_second;
            tick_timer = Instant::now();
        }
        frame(delta_time);
        delta_time = frame_timer.elapsed().as_secs_f32();
    }
}
