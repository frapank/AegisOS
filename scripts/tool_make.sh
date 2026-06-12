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

BINUTILS_VER="2.40"
GCC_VER="15.2.0"

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
        "$PRIV_CMD" "$@"
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

require_lib() {
    local name="$1" pkg="$2" gentoo="$3" debian="$4"
    if pkg-config --exists "$pkg" 2>/dev/null; then
        return 0
    fi
    log_error "Libreria '$name' non trovata."
    log_error "  Gentoo: emerge $gentoo"
    log_error "  Debian: apt install $debian"
    exit 1
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

    require_lib gmp  "gmp"   "dev-libs/gmp"        "libgmp-dev"
    require_lib mpfr "mpfr"  "dev-libs/mpfr"        "libmpfr-dev"
    require_lib mpc  "mpc"   "dev-libs/mpc"         "libmpc-dev"
}

afe_rm_rf() {
    local path="${1:?safe_rm_rf: dir not found}"
    if [[ "$path" == "/" || "$path" == "$HOME" ]]; then
        log_error "can't remove: '$path'"
        return 1
    fi
    rm -rf -- "$path"
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
    if ! wget -c --timeout=60 --tries=3 \
            "https://ftp.gnu.org/gnu/binutils/binutils-${BINUTILS_VER}.tar.xz"; then
        log_error "Failed to download binutils."
        exit 1
    fi
    if ! wget -c --timeout=60 --tries=3 \
            "https://ftp.gnu.org/gnu/gcc/gcc-${GCC_VER}/gcc-${GCC_VER}.tar.xz"; then
        log_error "Failed to download gcc."
        exit 1
    fi

    if ! tar -xf "binutils-${BINUTILS_VER}.tar.xz"; then
        log_error "Failed to extract binutils."
        exit 1
    fi
    if ! tar -xf "gcc-${GCC_VER}.tar.xz"; then
        log_error "Failed to extract gcc."
        exit 1
    fi
    log_success "Get done"
}

build_bin_utils() {
    log_info "Building binutils..."
    mkdir -p build-binutils
    pushd build-binutils > /dev/null
    "../binutils-${BINUTILS_VER}/configure" \
        --target="$TARGET" --prefix="$BIN_PATH" \
        --disable-nls --disable-werror
    make -j"$NPROC"
    make install
    popd > /dev/null
    log_success "Bin utils done"
}

build_gcc() {
    export PATH="$BIN_PATH/bin:$PATH"
    log_info "Building GCC (stage 1)..."
    mkdir -p build-gcc
    pushd build-gcc > /dev/null
    CXXFLAGS="-fno-char8_t" "../gcc-${GCC_VER}/configure" \
        --target="$TARGET" --prefix="$BIN_PATH" --disable-nls \
        --enable-languages=c --without-headers --disable-multilib
    make -j"$NPROC" all-gcc
    make install-gcc
    popd > /dev/null
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

    if [ $EXIT_CODE -eq 0 ]; then
        log_info "Cleaning temporary files..."
        rm -rf "$TEMP_PATH"
        log_success "Cleaning done!"
    else
        log_error "Build failed, preserving $TEMP_PATH"
    fi
}

main() {
    detect_priv_cmd
    require_tools
    init
    get
    build_bin_utils
    build_gcc
}

main
