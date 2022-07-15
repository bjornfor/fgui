{ pkgs ? import <nixpkgs> {} }:

with pkgs;

{
  fgui = stdenv.mkDerivation {
    name = "fgui-dev";
    src = ./.;
    buildInputs = [
      cmake
      clang-tools
      SDL
      graphicsmagick
    ];
  };
}
