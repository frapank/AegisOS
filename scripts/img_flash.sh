#!/usr/bin/env bash
set -Eeuo pipefail
trap 'log_error "Error at line ${LINENO}: ${BASH_COMMAND} (exit code: $?)"' ERR

log() {
    local level="$1"
    shift
    echo "[$(date '+%F %T')] [$level] $*"
}

log_info()    { log INFO "$@"; }
log_success() { log OK "$@"; }
log_error()   { log ERROR "$@" >&2; }

error_exit() {
    log_error "Error: $1"
    exit 1
}

detect_priv_cmd() {
    if [[ $EUID -eq 0 ]]; then
        PRIV_CMD=""
    elif command -v sudo >/dev/null 2>&1; then
        PRIV_CMD="sudo"
    elif command -v doas >/dev/null 2>&1; then
        PRIV_CMD="doas"
    else
        error_exit "Neither sudo nor doas found"
    fi
}

run_root() {
    if [[ -n "${PRIV_CMD:-}" ]]; then
        $PRIV_CMD bash -c "$*"
    else
        "$@"
    fi
}

usage() {
    log_info "Usage: $0 <disk.img> <stage1.bin> <stage2.bin> <stage3.bin> <kernel>"
    exit 1
}

check_tools() {
    REQUIRED_TOOLS=(dd losetup mount umount)

    for tool in "${REQUIRED_TOOLS[@]}"; do
        command -v "$tool" >/dev/null 2>&1 || error_exit "Required tool '$tool' not found."
    done
}

check_files() {
    IMAGE="$1"
    STAGE1="$2"
    STAGE2="$3"
    STAGE3="$4"
    KERNEL="$5"

    [[ -f "$IMAGE"  ]] || error_exit "Disk image '$IMAGE' not found."
    [[ -f "$STAGE1" ]] || error_exit "Stage1 binary '$STAGE1' not found."
    [[ -f "$STAGE2" ]] || error_exit "Stage2 binary '$STAGE2' not found."
    [[ -f "$STAGE3" ]] || error_exit "Stage3 binary '$STAGE3' not found."
    [[ -f "$KERNEL" ]] || error_exit "Kernel binary '$KERNEL' not found."
}

prepare_disk() {
    dd if="$STAGE1" of="$IMAGE" bs=446 count=1 conv=notrunc status=none
    dd if="$STAGE2" of="$IMAGE" bs=512 seek=2048 conv=notrunc status=none
    dd if="$STAGE3" of="$IMAGE" bs=512 seek=8192 conv=notrunc status=none
    log_success "Stages loaded"
}

setup_loop_device() {
    log_info "Setting up device loop. Root required:"
    LOOPDEV=$(run_root losetup -fP --show "$IMAGE")
    log_success "Loop device ready"
}

mount_and_copy_kernel() {
    log_info "Mounting and copying the image. Root required:"

    rm -rf tmp/
    mkdir -p tmp/

    run_root mount "${LOOPDEV}p2" tmp/

    run_root mkdir -p tmp/boot
    run_root cp "$KERNEL" tmp/boot/lain_kernel.bin

    run_root umount tmp/
    rm -rf tmp/
    run_root losetup -d "$LOOPDEV"

    log_success "Copied the image successfully"
}

main() {
    detect_priv_cmd
    check_tools

    if [[ $# -ne 5 ]]; then
        usage
    fi

    IMAGE="$1"
    STAGE1="$2"
    STAGE2="$3"
    STAGE3="$4"
    KERNEL="$5"

    check_files "$IMAGE" "$STAGE1" "$STAGE2" "$STAGE3" "$KERNEL"
    prepare_disk
    setup_loop_device
    mount_and_copy_kernel
}

main "$@"
