alias b:=build
alias t:=test
alias d:=docs
alias c:=clean

default: build

# Build the project
build: 
    meson compile -C build 

# Generate api docs with doxygen
docs: setup
    meson compile -C build docs

# Run tests
test: setup
    meson test -C build --verbose

# Quick recipe for other tasks, but you can use it, just configures the build
setup: 
    meson setup build --reconfigure

# Clean build artifacts
clean:
    rm -rf build
