#!/usr/bin/env bash
set -Eeuo pipefail
trap 'log_error "Error at line ${LINENO}: ${BASH_COMMAND} (exit code: $?)"' ERR
EXIT_CODE=0
INTERRUPTED=0
trap 'EXIT_CODE=$?; cleanup' EXIT
trap 'INTERRUPTED=1' INT

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

usage() {
    log_info "Usage: $0 <disk.img> <size_mb>"
    exit 1
}

check_tools() {
    REQUIRED_TOOLS=(dd losetup fdisk mkfs.ext2 lsblk)
    for tool in "${REQUIRED_TOOLS[@]}"; do
        if ! command -v "$tool" >/dev/null 2>&1; then
            log_error "Required tool '$tool' not found"
            exit 1
        fi
    done
}

parse_args() {
    if [[ $# -ne 2 ]]; then
        usage
    fi

    IMAGE="$1"
    SIZE_MB="$2"
}

create_image() {
    log_info "Creating ${SIZE_MB}MB disk image..."
    dd if=/dev/zero of="$IMAGE" bs=1M count="$SIZE_MB" status=none
}

attach_loop() {
    log_info "Attaching loop device. Root required:"
    LOOPDEV=$(run_root losetup -fP --show "$IMAGE")
    log_success "Loop device attached: $LOOPDEV"
}

partition_disk() {
    log_info "Partitioning disk. Root required:"
    run_root fdisk "$LOOPDEV" >/dev/null 2>&1 <<EOF
o
n
p
1
2048
+8M
n
p
2


w
EOF
    log_success "Disk partitioned"
}

reload_loop() {
    log_info "Reloading loop device to detect partitions. Root required:"
    run_root losetup -d "$LOOPDEV"
    LOOPDEV=$(run_root losetup -fP --show "$IMAGE")
    log_info "Loop device reattached: $LOOPDEV"
}

format_partition() {
    PART2="${LOOPDEV}p2"
    log_info "Formatting $PART2 as ext2. Root required:"
    run_root mkfs.ext2 -q "$PART2"
    log_success "EXT2 partition created"
}

verify_layout() {
    log_info "Verifying partition layout..."
    lsblk "$LOOPDEV"
}

permission_set() {
    chmod 777 "$IMAGE"
}

cleanup() {
    log_info "Cleaning up with code $EXIT_CODE"
    if [[ "$INTERRUPTED" -eq 1 ]]; then
        EXIT_CODE=130
    fi

    if [ $EXIT_CODE -ne 0 ]; then
        log_error "Cleaning up files: $IMAGE"
        rm -f "$IMAGE"
    fi

    if losetup "$LOOPDEV" &>/dev/null; then
        log_info "Removing loop device. Root required:"
        if ! run_root losetup -d "$LOOPDEV"; then
            log_error "Can't detach loop device"
        fi
    else
        log_info "No loop device detected. Skipping..."
    fi
    log_success "Cleaning done!"
}

main() {
    detect_priv_cmd
    parse_args "$@"
    check_tools
    create_image
    attach_loop
    partition_disk
    reload_loop
    format_partition
    verify_layout
    permission_set
}

main "$@"
