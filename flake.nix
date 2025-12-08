{
  inputs = {
    flake-utils.url = "github:numtide/flake-utils";
    naersk.url = "github:nix-community/naersk";
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
    fenix.url = "github:nix-community/fenix";

    mdbook.url = "github:pbar1/nix-mdbook";
  };

  outputs = { self, flake-utils, naersk, nixpkgs, fenix, mdbook }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = (import nixpkgs) {
          inherit system;
          overlays = [ fenix.overlays.default ];
        };

        naersk' = pkgs.callPackage naersk { };
      in rec {
        defaultPackage = naersk'.buildPackage { src = ./.; };

        devShell = pkgs.mkShell {
          nativeBuildInputs = with pkgs; [
            just

            alejandra
            (pkgs.fenix.stable.withComponents [
              "cargo"
              "clippy"
              "rust-src"
              "rustc"
              "rustfmt"
              "rust-analyzer"
            ])
            cargo-wizard
            cargo-audit
            cargo-bloat

            pkg-config
            mold
          ];

          buildInputs = with pkgs; [
            luajit
            # glfw stuff
            glfw
            xorg.libX11
            wayland
          ];

          shellHook = ''
            export LD_LIBRARY_PATH=${pkgs.luajit}/lib:${pkgs.glfw}/lib:$LD_LIBRARY_PATH
          '';
        };

        packages = {
          book = mdbook.lib.buildMdBookProject {
            inherit (pkgs) system;
            src = ./docs;
          };
        };

      });
}
