# Filesystem

## VFS layout

```
/
├── bin/        executables
├── dev/        device nodes
├── etc/        configuration files
├── home/       user home directories
├── proc/       process information (read-only, kernel-generated)
├── srv/        init system service files
├── tmp/        temporary files (cleared on boot)
└── cmp/        capability system
    ├── set/    capability set definitions (.cset files)
    ├── usr/    user capability files (.cuser files)
    └── fin/    compiled/cached capability objects (inodes)
```

---

## Capability system

Traditional Unix uses users, groups, and rwx permissions.
AegisOS replaces this with **capabilities**. Explicit, scoped permissions
attached to processes, not users.

### Capability sets - `.cset`

Stored in `/cmp/set/`. Each file defines a named set of permissions:

```text
{
    READ:  "/home/alice/*"
    WRITE: "/home/alice/*"

    READ:  "/etc/random.conf"
    READ:  "/tmp"

    NET_CONNECT: TCP 1.2.3.0/24 80 443
    NET_BIND:    TCP * 1024-65535
    NET_LISTEN:  TCP 80
    NET_RAW:     ETHERNET
}
```

Available capability types:

| Type | Description |
|---|---|
| `READ` | Read a file or directory tree |
| `WRITE` | Write a file or directory tree |
| `EXECUTE` | Execute a binary |
| `NET_CONNECT` | Open outbound TCP/UDP connections |
| `NET_BIND` | Bind to a port |
| `NET_LISTEN` | Accept incoming connections |
| `NET_RAW` | Raw Ethernet/packet access |

### User files - `.cuser`

Stored in `/cmp/usr/`. Each user has one file that composes their
effective capability set from existing `.cset` files:

```text
{
    ADD:    "/cmp/set/default.cset"
    ADD:    "/cmp/set/audio.cset"
    ADD:    "/cmp/set/video.cset"
    REMOVE: "/cmp/set/restricted.cset"
    ADD: {
        READ: "/home/alice/private/*"
    }
}
```

Rules:
- A user's filename prefix must match their username.
- Processes inherit their parent's capabilities.
- Syscalls can only **remove** capabilities from child processes, never add.
- Default capabilities come from `/cmp/set/default.cset`.

### Compiled objects - `/cmp/fin/`

At runtime the capability system compiles `.cset` and `.cuser` sources into
inode objects cached under `/cmp/fin/`. A cache layer at `/cmp/fin/cache/`
tracks inode positions so objects can be updated when files move.

All capability initialisation is handled by `/bin/capability_init`, which
is launched by the init system at boot.

---

## Disk filesystem

The physical disk uses **ext2** (revision 1, dynamic inodes).
The bootenv stage reads `boot/lain_kernel.bin` from partition 2 before
the kernel starts. The kernel's own filesystem driver will handle the
rest of the tree at runtime.

Driver limitations (current bootenv loader):
- Supports block sizes 1 KiB and 4 KiB
- Supports direct blocks (0–11) and single-indirect (block 12)
- Max file size ~4 MiB (sufficient for the kernel binary)
- Single block group descriptor assumed to fit in one block
