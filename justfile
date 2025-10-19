alias r:=run
alias b:=build
alias c:=clean

default: build

# Run an example, defaults to testbed
run target="testbed": build
    ./build/{{target}}

# Build the project
build:
    meson setup build --reconfigure
    ninja -C build

# Clean build artifacts
clean:
    rm -rf build
