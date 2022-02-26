#!/usr/bin/env bash
set -x
# LLVM must have been installed or compiled from source
DIR="$HOME/antlr4"
if [ -d "$DIR" ]; then
  # Take action if $DIR exists. #
  echo "Skipping Dependencies Installation ${DIR} found... (Delete Directory if want to force install deps)"
else
  # Download antlr4
  mkdir "$DIR" && cd "$DIR" && wget https://www.antlr.org/download/antlr-4.9.3-complete.jar

  # download pkg-config and uuid
  mkdir -p ~/deps/debs
  mkdir -p ~/deps/tools
  cd ~/deps/debs && apt download pkg-config && dpkg -x pkg-config*.deb ~/deps/tools
  cd ~/deps/debs && apt download uuid-dev && dpkg -x uuid-dev*.deb ~/deps/tools

  echo "export PATH$HOME/deps/tools/usr/bin:\$PATH" >>~/.bashrc
  echo "export PKG_CONFIG_PATH=$HOME/deps/tools/usr/lib/x86_64-linux-gnu/pkgconfig" >>~/.bashrc
fi

if [ "$1" == "--benchmark" ]; then
  cd "$HOME"/jit-regex && cmake -S benchmark -B build/benchmark -G Ninja -DANTLR_EXECUTABLE="$HOME/antlr4/antlr-4.9.3-complete.jar"
  cd "$HOME"/jit-regex && cmake --build build/benchmark
  echo "Usage: $HOME/jit-regex/build/benchmark/ZRegex_BM"
else
  cd "$HOME"/jit-regex && cmake -S standalone -B build/standalone -G Ninja -DANTLR_EXECUTABLE="$HOME/antlr4/antlr-4.9.3-complete.jar"
  cd "$HOME"/jit-regex && cmake --build build/standalone
  echo "Usage: $HOME/jit-regex/build/standalone/ZRegex -r Regex -b \"llvm\""
fi
