alias r:=run
alias b:=build
alias c:=clean

default: build

# Run an example, defaults to testbed
run target="testbed": build
    ./build/{{target}}

# Generate api docs with doxygen
docs: setup
    meson compile -C build docs

# Build the project
build: setup
    ninja -C build clang-tidy
    meson compile -C build 

# Quick recipe for other tasks, but you can use it, just configures the build
setup: 
    meson setup build --reconfigure

# Clean build artifacts
clean:
    rm -rf build
