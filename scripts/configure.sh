#!/usr/bin/env bash
set -Eeuo pipefail
trap 'log_error "Error at line ${LINENO}: ${BASH_COMMAND} (exit code: $?)"' ERR

MK=config.mk
DEBUG=0
KSHELL=1

ASM_DEFAULT=nasm
CAT_DEFAULT=cat
GCC_DEFAULT=tools/bin/i386-elf-gcc
LD_DEFAULT=tools/bin/i386-elf-ld
OBJCOPY_DEFAULT=tools/bin/i386-elf-objcopy
VMRAM_DEFAULT=512M
VMDISK_DEFAULT=1024

ASM="$ASM_DEFAULT"
CAT="$CAT_DEFAULT"
GCC="$GCC_DEFAULT"
LD="$LD_DEFAULT"
OBJCOPY="$OBJCOPY_DEFAULT"
VMRAM="$VMRAM_DEFAULT"
VMDISK="$VMDISK_DEFAULT"

GCC_FLAGS="-nostdlib -nodefaultlibs -ffreestanding -m32 -O2 -fno-omit-frame-pointer -g \
-fno-stack-protector -fno-builtin -fno-common -march=i686 \
-mtune=i686 -Wall -Wextra -Wpedantic -pipe"

log() {
    local level="$1"
    shift
    echo "[$(date '+%F %T')] [$level] $*"
}

log_info()    { log INFO "$@"; }
log_success() { log OK "$@"; }
log_error()   { log ERROR "$@" >&2; }

show_help() {
    cat <<'EOF'
Usage: ./configure [options]

Options:
  --set-vmram=M/G      Ram for test qemu vm (default 512M)
  --set-vmdisk=<NUM>    Size of the vm in mb no suffix (default 1024)
  --set-gcc=PATH       Use custom GCC compiler (i386-elf-gcc default)
  --set-ld=PATH        Use custom LD linker (i386-elf-ld default)
  --set-objcopy=PATH   Use custom OBJCOPY tool (i386-elf-objcopy default)
  --set-asm=PATH       Use custom ASM assembler (nasm default)
  --set-cat=PATH       Use custom CAT tool (cat default)
  --enable-debug       Enable debug flags
  --disable-kshell     Disable kshell
  --help, -h           Show this help message
EOF
}

parse_args() {
    while (($#)); do
        case "$1" in
            --set-vmram=*)    VMRAM="${1#--set-vmram=}"; shift ;;
            --set-vmdisk=*)   VMDISK="${1#--set-vmdisk=}"; shift ;;
            --set-gcc=*)      GCC="${1#--set-gcc=}"; shift ;;
            --set-ld=*)       LD="${1#--set-ld=}"; shift ;;
            --set-objcopy=*)  OBJCOPY="${1#--set-objcopy=}"; shift ;;
            --set-asm=*)      ASM="${1#--set-asm=}"; shift ;;
            --set-cat=*)      CAT="${1#--set-cat=}"; shift ;;
            --enable-debug)   DEBUG=1; shift ;;
            --disable-kshell) KSHELL=0; shift ;;
            --help|-h)        show_help; exit 0 ;;
            *)                echo "Unknown option: $1" >&2; exit 1 ;;
        esac
    done
}

check_tool() {
    local name="$1"
    local cmd="$2"

    if ! command -v "$cmd" >/dev/null 2>&1; then
        log_error "Required tool '$cmd' not found. Please run './make_tools' first." >&2
        rm -f "$MK"
        exit 1
    fi

    case "$name" in
        CC|LD|OBJCOPY)
            printf "%s := \$(ROOT)/%s\n" "$name" "$cmd" >> "$MK"
            ;;
        *)
            printf "%s := %s\n" "$name" "$cmd" >> "$MK"
            ;;
    esac

    log_success "$name selected: $cmd"
}

test_gcc() {
    cat > test.c <<'EOF'
int main(){}
EOF

    if ! "$GCC" -ffreestanding -c test.c -o test.o >/dev/null 2>&1; then
        log_error "GCC compiler '$GCC' failed to compile a test program" >&2
        rm -f test.c "$MK"
        exit 1
    fi

    if "$GCC" -ffreestanding -std=c17 -c test.c -o test.o >/dev/null 2>&1; then
        GCC_FLAGS="$GCC_FLAGS -std=c17"
    else
        log_error "$GCC doesn't support -std=c17" >&2
        rm -f test.o test.c "$MK"
        exit 1
    fi
    rm -f test.o test.c
}

set_debug_flags() {
    if [ "$DEBUG" -eq 1 ]; then
        log_info "Debug enabled"
        GCC_FLAGS="$GCC_FLAGS -g -O0 -DDEBUG"
    else
        log_info "Debug not enabled"
        GCC_FLAGS="$GCC_FLAGS -O2 -DNDEBUG"
    fi
}

set_kshell_flags() {
    if [ "$KSHELL" -eq 1 ]; then
        log_info "KShell enabled"
        GCC_FLAGS="$GCC_FLAGS -DKSHELL"
    else
        log_info "Kshell disabled"
        GCC_FLAGS="$GCC_FLAGS -DNKSHELL"
    fi
}

write_config() {
    printf "VMRAM  := %s\n" "$VMRAM" >> "$MK"
    log_info "VM ram: $VMRAM"

    printf "VMDISK  := %s\n" "$VMDISK" >> "$MK"
    log_info "VM disk: $VMDISK"

    printf "FLAGS := %s\n" "$GCC_FLAGS" >> "$MK"

    log_info "config.mk successfully generated"
}

main() {
    parse_args "$@"

    rm -f "$MK"

    check_tool "ASM" "$ASM"
    check_tool "CAT" "$CAT"
    check_tool "CC" "$GCC"
    check_tool "LD" "$LD"
    check_tool "OBJCOPY" "$OBJCOPY"

    test_gcc

    set_debug_flags
    set_kshell_flags

    write_config
}

main "$@"
