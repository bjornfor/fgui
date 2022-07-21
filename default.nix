{ pkgs ? import <nixpkgs> {} }:

with pkgs;

stdenv.mkDerivation {
  name = "fgui-dev";
  src = lib.cleanSource ./.;
  buildInputs = [
    cmake
    clang-tools
    SDL
    graphicsmagick
  ];
}
