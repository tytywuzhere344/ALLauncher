{
  description = "A custom launcher for Minecraft that allows you to easily manage multiple installations of Minecraft at once (Fork of MultiMC)";

  nixConfig = {
    extra-substituters = [ "https://allauncher.cachix.org" ];
    extra-trusted-public-keys = [
      "allauncher.cachix.org-1:9/n/FGyABA2jLUVfY+DEp4hKds/rwO+SCOtbOkDzd+c="
    ];
  };

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";

    libnbtplusplus = {
      url = "github:ALLauncher/libnbtplusplus";
      flake = false;
    };

    qrcodegenerator = {
      url = "github:nayuki/QR-Code-generator";
      flake = false;
    };
  };

  outputs =
    {
      self,
      nixpkgs,
      libnbtplusplus,
      qrcodegenerator,
    }:

    let
      inherit (nixpkgs) lib;

      # While we only officially support aarch and x86_64 on Linux and MacOS,
      # we expose a reasonable amount of other systems for users who want to
      # build for most exotic platforms
      systems = lib.systems.flakeExposed;

      forAllSystems = lib.genAttrs systems;
      nixpkgsFor = forAllSystems (system: nixpkgs.legacyPackages.${system});
    in

    {
      checks = forAllSystems (
        system:

        let
          pkgs = nixpkgsFor.${system};
          llvm = pkgs.llvmPackages_19;
        in

        {
          formatting =
            pkgs.runCommand "check-formatting"
              {
                nativeBuildInputs = with pkgs; [
                  deadnix
                  llvm.clang-tools
                  markdownlint-cli
                  nixfmt-rfc-style
                  statix
                ];
              }
              ''
                cd ${self}

                echo "Running clang-format...."
                clang-format --dry-run --style='file' --Werror */**.{c,cc,cpp,h,hh,hpp}

                echo "Running deadnix..."
                deadnix --fail

                echo "Running markdownlint..."
                markdownlint --dot .

                echo "Running nixfmt..."
                find -type f -name '*.nix' -exec nixfmt --check {} +

                echo "Running statix"
                statix check .

                touch $out
              '';
        }
      );

      devShells = forAllSystems (
        system:

        let
          pkgs = nixpkgsFor.${system};
          llvm = pkgs.llvmPackages_19;

          packages' = self.packages.${system};

          welcomeMessage = ''
            Welcome to the Prism Launcher repository! 🌈

            We just set some things up for you. To get building, you can run:

            ```
            $ cd "$cmakeBuildDir"
            $ ninjaBuildPhase
            $ ninjaInstallPhase
            ```

            Feel free to ask any questions in our Discord server or Matrix space:
              - https://allauncher.org/discord
              - https://matrix.to/#/#allauncher:matrix.org

            And thanks for helping out :)
          '';

          # Re-use our package wrapper to wrap our development environment
          qt-wrapper-env = packages'.allauncher.overrideAttrs (old: {
            name = "qt-wrapper-env";

            # Required to use script-based makeWrapper below
            strictDeps = true;

            # We don't need/want the unwrapped Prism package
            paths = [ ];

            nativeBuildInputs = old.nativeBuildInputs or [ ] ++ [
              # Ensure the wrapper is script based so it can be sourced
              pkgs.makeWrapper
            ];

            # Inspired by https://discourse.nixos.org/t/python-qt-woes/11808/10
            buildCommand = ''
              makeQtWrapper ${lib.getExe pkgs.runtimeShellPackage} "$out"
              sed -i '/^exec/d' "$out"
            '';
          });
        in

        {
          default = pkgs.mkShell {
            name = "prism-launcher";

            inputsFrom = [ packages'.allauncher-unwrapped ];

            packages = with pkgs; [
              ccache
              llvm.clang-tools
            ];

            cmakeBuildType = "Debug";
            cmakeFlags = [ "-GNinja" ] ++ packages'.allauncher.cmakeFlags;
            dontFixCmake = true;

            shellHook = ''
              echo "Sourcing ${qt-wrapper-env}"
              source ${qt-wrapper-env}

              git submodule update --init --force

              if [ ! -f compile_commands.json ]; then
                cmakeConfigurePhase
                cd ..
                ln -s "$cmakeBuildDir"/compile_commands.json compile_commands.json
              fi

              echo ${lib.escapeShellArg welcomeMessage}
            '';
          };
        }
      );

      formatter = forAllSystems (system: nixpkgsFor.${system}.nixfmt-rfc-style);

      overlays.default = final: prev: {
        allauncher-unwrapped = prev.callPackage ./nix/unwrapped.nix {
          inherit
            libnbtplusplus
            qrcodegenerator
            self
            ;
        };

        allauncher = final.callPackage ./nix/wrapper.nix { };
      };

      packages = forAllSystems (
        system:

        let
          pkgs = nixpkgsFor.${system};

          # Build a scope from our overlay
          prismPackages = lib.makeScope pkgs.newScope (final: self.overlays.default final pkgs);

          # Grab our packages from it and set the default
          packages = {
            inherit (prismPackages) allauncher-unwrapped allauncher;
            default = prismPackages.allauncher;
          };
        in

        # Only output them if they're available on the current system
        lib.filterAttrs (_: lib.meta.availableOn pkgs.stdenv.hostPlatform) packages
      );

      # We put these under legacyPackages as they are meant for CI, not end user consumption
      legacyPackages = forAllSystems (
        system:

        let
          packages' = self.packages.${system};
          legacyPackages' = self.legacyPackages.${system};
        in

        {
          allauncher-debug = packages'.allauncher.override {
            allauncher-unwrapped = legacyPackages'.allauncher-unwrapped-debug;
          };

          allauncher-unwrapped-debug = packages'.allauncher-unwrapped.overrideAttrs {
            cmakeBuildType = "Debug";
            dontStrip = true;
          };
        }
      );
    };
}
