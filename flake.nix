{
  inputs.nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  inputs.mdbook.url = "github:pbar1/nix-mdbook";

  outputs = { nixpkgs, mdbook, ... }:
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
            luajit

            doxygen
          ];

          buildInputs = with pkgs; [ ];

          shellHook = ''
            export CC=clang
            export CXX=clang++
            export SPDLOG_LEVEL=trace
          '';
        };
      });

      packages = forAllSystems (pkgs: {
        book = mdbook.lib.buildMdBookProject {
          inherit (pkgs) system;
          src = ./docs;
        };
      });
    };
}
