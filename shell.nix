{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  buildInputs = with pkgs; [
    gcc
    pkg-config
    SDL2
    SDL2_mixer
  ];
}
