#!/usr/bin/env bash
set -euo pipefail

project_dir="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"
framework_dir="${SERVICELIB_SOURCE_DIR:-/opt/servicelib}"
build_type="${1:-Release}"
output_dir="${2:-$project_dir/build/conan-${build_type,,}}"

if [[ ! -x "$framework_dir/scripts/conan-install.sh" ]]; then
  echo "cppcoroservicelib Conan entrypoint is missing: $framework_dir/scripts/conan-install.sh" >&2
  exit 2
fi

# The generated project owns its tests, while the framework source remains
# configured with CPPCOROSERVICELIB_BUILD_TESTS=OFF by CMake. Requesting the
# Conan test dependency here makes GTest available without FetchContent.
# Keep Conan's package identity consistent with the generated CMake presets.
export CPP_CORO_IO_BACKEND="epoll"
export CPPCOROSERVICELIB_BUILD_TESTS="${CPPCOROSERVICELIB_BUILD_TESTS:-True}"
export CPPCOROSERVICELIB_ENABLE_GRPC=True
export CPPCOROSERVICELIB_ENABLE_KAFKA=True
export CPPCOROSERVICELIB_ENABLE_OTEL=True

"$framework_dir/scripts/conan-install.sh" "$build_type" \
  --output-folder="$output_dir" \
  "${@:3}"

toolchain="$output_dir/build/$build_type/generators/conan_toolchain.cmake"
if [[ ! -f "$toolchain" ]]; then
  echo "Conan toolchain is missing: $toolchain" >&2
  exit 2
fi
printf '%s\n' "$toolchain" > "$output_dir/toolchain.path"