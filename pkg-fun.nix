{ lib, stdenv, nix }: stdenv.mkDerivation {
  pname                 = "nix-cpp";
  version               = "0.1.0";
  include               = builtins.path { path = ./include; };
  lib                   = builtins.path { path = ./lib;     };
  propagatedBuildInputs = [nix nix.dev];
  unpackPhase           = ":";
  dontPatch             = true;
  dontConfigure         = true;
  dontBuild             = true;
  installPhase          = ''
    mkdir -p "$out/lib/pkgconfig";
    cp -r "$include" "$out/include";
    sed -e "s,@PREFIX@,$out," -e "s,@VERSION@,$version,"                   \
        "$lib/pkgconfig/nix-cpp.pc.in" > "$out/lib/pkgconfig/nix-cpp.pc";
  '';
  meta.license = lib.licenses.gpl3;
}
