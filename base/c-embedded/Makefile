# ──────────────────────────────────────────────────────────
# Convenience Makefile for base/c-embedded
# Usage:
#   make          → build (host, debug)
#   make test     → build + run tests
#   make release  → release build
#   make clean    → remove build directory
# ──────────────────────────────────────────────────────────

BUILD_DIR := build
BUILD_DIR_REL := build_release

.PHONY: all test release clean help

all: $(BUILD_DIR)/Makefile
	cmake --build $(BUILD_DIR) --parallel

$(BUILD_DIR)/Makefile:
	cmake -B $(BUILD_DIR) \
	      -DCMAKE_BUILD_TYPE=Debug \
	      -DBUILD_FOR_HOST=ON \
	      -DBUILD_TESTS=ON

test: all
	ctest --test-dir $(BUILD_DIR) --output-on-failure -V

release:
	cmake -B $(BUILD_DIR_REL) \
	      -DCMAKE_BUILD_TYPE=Release \
	      -DBUILD_FOR_HOST=ON \
	      -DBUILD_TESTS=OFF
	cmake --build $(BUILD_DIR_REL) --parallel

clean:
	rm -rf $(BUILD_DIR) $(BUILD_DIR_REL)

help:
	@echo "Targets:"
	@echo "  all      - Debug build for host (default)"
	@echo "  test     - Build and run all unit tests"
	@echo "  release  - Release build for host"
	@echo "  clean    - Remove build directories"
