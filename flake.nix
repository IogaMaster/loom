{
  inputs.nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";

  outputs = { nixpkgs, ... }:
    let
      inherit (nixpkgs) lib;
      forAllSystems = function:
        nixpkgs.lib.genAttrs [ "x86_64-linux" "aarch64-linux" ]
        (system: function nixpkgs.legacyPackages.${system});
    in rec {
      devShells = forAllSystems (pkgs: {
        default = pkgs.mkShell {
          nativeBuildInputs = with pkgs; [
            clang-tools
            clang
            gdb

            # Build system
            just
            meson
            ninja
            pkg-config
            fd

            doxygen
          ];

          buildInputs = with pkgs; [
            sdl3
            sdl3-image
            sdl3-ttf

            # Runtime deps for sdl3
            libpng
            freetype
            dbus
            wayland
          ];

          shellHook = ''
            export CC=clang
            export CXX=clang++
          '';

        };
      });
    };
}
