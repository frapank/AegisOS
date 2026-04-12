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
    log_error "Error: $1" >&2
    exit 1
}

check_qemu() {
    if ! command -v qemu-system-x86_64 >/dev/null 2>&1; then
        error_exit "qemu-system-x86_64 is not installed."
    fi
}

usage() {
    log_info "Usage: $0 <disk.img> <ram>"
    exit 1
}

validate_ram() {
    if ! [[ "$1" =~ ^[0-9]+[MG]$ ]]; then
        error_exit "RAM must be a number followed by M or G (examples: 512M, 1G)."
    fi
}

main() {
    check_qemu

    if [[ $# -ne 2 ]]; then
        usage
    fi

    DISK_IMG="$1"
    VMRAM="$2"

    if [[ ! -f "$DISK_IMG" ]]; then
        error_exit "Disk image '$DISK_IMG' not found."
    fi

    validate_ram "$VMRAM"

    log_info "Starting vm"
    exec qemu-system-x86_64 \
        -machine pc \
        -drive file="$DISK_IMG",format=raw \
        -m "$VMRAM" \
        -enable-kvm \
        -boot c \
        -serial stdio
}

main "$@"
