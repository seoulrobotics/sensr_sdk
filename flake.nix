{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-23.05";
    flake-parts.url = "github:hercules-ci/flake-parts";
  };

  outputs = inputs@{ flake-parts, ... }:
    flake-parts.lib.mkFlake { inherit inputs; } {
      systems = [ "x86_64-linux" ];
      perSystem = { config, self', inputs', pkgs, system, ... }:
        let
          version = "1.1.0";
          protobuf = pkgs.protobuf3_20; # 3.21 is not found by CMake.

          mkSample = { program, src }: pkgs.stdenv.mkDerivation {
            inherit src version;

            pname = "sensr_sdk_sample_${builtins.baseNameOf src}";

            buildInputs = [ config.packages.sdk ];
            nativeBuildInputs = [ pkgs.cmake protobuf ];

            installPhase = ''
              mkdir -p $out/bin
              cp ${program} $out/bin/sensr_sdk_sample_${builtins.baseNameOf src}
            '';
          };
        in
        {
          packages.sdk = pkgs.stdenv.mkDerivation {
            inherit version;

            pname = "sensr_sdk";

            src = pkgs.lib.sourceByRegex ./. [
              "^CMakeLists.txt$"
              "^cmake(|/.+)$"
              "^include(|/.+)$"
              "^proto(|/.+)$"
              "^src(|/.+)$"
            ];

            buildInputs = [ protobuf ] ++ (with pkgs; [ asio openssl websocketpp zlib ]);
            nativeBuildInputs = [ protobuf ] ++ (with pkgs; [ cmake ]);

            meta = {
              description = "SENSR SDK";
              homepage = "https://github.com/seoulrobotics/sensr_sdk";
            };
          };

          packages.console-output-sample = mkSample {
            src = ./samples/console_output;
            program = "console_output_sample";
          };

          packages.js-sdk = pkgs.buildNpmPackage {
            inherit version;

            pname = "sensr_javascript_sdk";

            src = pkgs.lib.sourceByRegex ./javascript/javascript_sdk [
              "^package.json$"
              "^package-lock.json$"
              "^src(|/.+\\.js)$"
            ];

            postPatch = ''
              substituteInPlace package.json \
                --replace '"postinstall": "./gen_proto.sh"' '"build": "true"'
            '';

            postBuild = ''
              mkdir -p $out/js_proto

              ${protobuf}/bin/protoc \
                --proto_path=${./proto} \
                --js_out=import_style=commonjs,binary:$out/js_proto \
                ${./proto}/*/*.proto
            '';

            # nix run nixpkgs#prefetch-npm-deps javascript/javascript_sdk/package-lock.json
            npmDepsHash = "sha256-mJrgf5iH3kdJDa695ms4GQhrxVFYJG2flPHDF3BT2QY=";
            npmPackFlags = [ "--ignore-scripts" ];
          };

          packages.default = config.packages.sdk;

          devShells.default = pkgs.mkShell {
            packages = [ pkgs.nodejs protobuf ];
          };
        };
    };
}
