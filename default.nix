{ pkgs ? import <nixpkgs> {} }:

with pkgs;

{
  fgui = stdenv.mkDerivation {
    name = "fgui-dev";
    src = ./.;
    buildInputs = [
      pkgconfig  # for getting compiler flags to YouCompleteMe
      cmake
      SDL
      pythonFull
      graphicsmagick
    ];
  };
}
