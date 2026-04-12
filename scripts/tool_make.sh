#!/usr/bin/env bash
set -Eeuo pipefail
trap 'log_error "Error at line ${LINENO}: ${BASH_COMMAND} (exit code: $?)"' ERR
EXIT_CODE=0
INTERRUPTED=0
trap 'EXIT_CODE=$?; cleanup' EXIT
trap 'INTERRUPTED=1' INT

ROOT_DIR="$(pwd)"
BIN_PATH="$ROOT_DIR/tools"
TEMP_PATH="$ROOT_DIR/temp"
TARGET="i386-elf"
NPROC=4

log() {
    local level="$1"
    shift
    echo "[$(date '+%F %T')] [$level] $*"
}

log_info()    { log INFO "$@"; }
log_success() { log OK "$@"; }
log_error()   { log ERROR "$@" >&2; }

detect_priv_cmd() {
    if [[ $EUID -eq 0 ]]; then
        PRIV_CMD=""
    elif command -v sudo >/dev/null 2>&1; then
        PRIV_CMD="sudo"
    elif command -v doas >/dev/null 2>&1; then
        PRIV_CMD="doas"
    else
        log_error "Neither sudo nor doas found"
        exit 1
    fi
}

run_root() {
    if [[ -n "${PRIV_CMD:-}" ]]; then
        $PRIV_CMD bash -c "$*"
    else
        "$@"
    fi
}

require() {
    if ! command -v "$1" &>/dev/null; then
        log_error "Command '$1' is required but not found."
        exit 1
    fi
}

require_tools() {
    require wget
    require tar
    require make
    require gcc
    require g++
    require ld
    require ar
    require ranlib
    require strip
}

init() {
    log_info "Installing in: $BIN_PATH"
    log_info "Require root for chown: "
    mkdir -p "$BIN_PATH"
    run_root chown "$(id -un)":"$(id -gn)" "$BIN_PATH"

    mkdir -p "$TEMP_PATH"
    cd "$TEMP_PATH"
    log_success "Init done"
}

get() {
    log_info "Downloading binutils and gcc..."
    if ! wget -c https://ftp.gnu.org/gnu/binutils/binutils-2.40.tar.xz; then
        log_error "Failed to download binutils."
        exit 1
    fi
    if ! wget -c https://ftp.gnu.org/gnu/gcc/gcc-13.2.0/gcc-13.2.0.tar.xz; then
        log_error "Failed to download gcc."
        exit 1
    fi

    if ! tar -xf binutils-2.40.tar.xz; then
        log_error "Failed to extract binutils."
        exit 1
    fi
    if ! tar -xf gcc-13.2.0.tar.xz; then
        log_error "Failed to extract gcc."
        exit 1
    fi
    log_success "Get done"
}

build_bin_utils() {
    log_info "Building binutils..."
    mkdir -p build-binutils
    cd build-binutils
    ../binutils-2.40/configure --target=$TARGET --prefix=$BIN_PATH --disable-nls --disable-werror
    make -j$NPROC
    make install
    cd ..
    log_success "Bin utils done"
}

prepare() {
    cd gcc-13.2.0
    ./contrib/download_prerequisites
    cd ..
}

build_gcc() {
    export PATH="$BIN_PATH/bin:$PATH"
    log_info "Building GCC (stage 1)..."
    mkdir -p build-gcc
    cd build-gcc
    ../gcc-13.2.0/configure --target=$TARGET --prefix=$BIN_PATH --disable-nls \
        --enable-languages=c --without-headers --disable-multilib
    make -j$NPROC all-gcc
    make install-gcc
    log_success "GCC done"
}

cleanup() {
    log_info "Cleaning up with code $EXIT_CODE"
    if [[ "$INTERRUPTED" -eq 1 ]]; then
        EXIT_CODE=130
    fi

    if [ $EXIT_CODE -ne 0 ]; then
        log_error "Error detected, removing $BIN_PATH..."
        rm -rf "$BIN_PATH"
    else
        log_success "Build completed, keeping $BIN_PATH"
    fi

    log_info "Cleaning temporary files..."
    rm -rf "$TEMP_PATH"
    log_success "Cleaning done!"
}

main() {
    detect_priv_cmd
    require_tools
    init
    get
    build_bin_utils
    prepare
    build_gcc
}

main
