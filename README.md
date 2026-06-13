# AegisOS

> [!WARNING]
> This README describes planned and conceptual features.
> The bootloader is currently being rewritten. As result, the kernel will not boot from the current main branch.
> If you want to run the kernel, use the codebase before the `rewrite/core` changes.

AegisOS is a microkernel operating system built as a fully modular ecosystem of components: bootloader, kernel, drivers, and user-space services.

At its core, the kernel does as little as possible. It exists only to manage memory and isolate processes, while everything else: filesystems, networking, and system services, runs independently in user space.

The goal of AegisOS is to explore a more composable and secure operating system design, where every subsystem is replaceable, testable, and isolated by default.

## File System
The file system is simple:

```text
/
├─ /dev       ( devices )
├─ /home      ( user homes )
├─ /bin       ( binary files )
├─ /tmp       ( temporary files )
├─ /srv       ( init system files )
├─ /etc       ( configurations )
├─ /cmp       ( capability system )
│  ├─ /set    ( capability sets )
│  ├─ /usr    ( capability users )
│  └─ /fin    ( capability final )
└─ /proc      ( process information )
````

## Capability Permission
In traditional Unix systems we have: users, groups, and permissions.
Instead, AegisOS only uses capabilities assigned to users.

"Capabilities" are the permissions available to a specific process, such as: READ, WRITE, EXECUTE and NETWORK.

These are defined inside a set stored in `/cmp/set/` with the `.cset` suffix.

By default, a user is itself a set of capabilities.
When a user registers, they receive default capability sets taken from `/cmp/set/default.cset` plus additional capability sets defined in their user file located at `/usr/user-name.cuser`.

Every process inherits the `.cset` files from its parent process.
There will be syscalls that allow removing permissions from child processes, but never adding them.

The initial `capabilities` are loaded by `/cmp/capability_init*`; this program is started by the init system.

A `.cset` file is structured like this:
```text
{
    READ: "/home/example/path/*"
    WRITE: "/home/example/path/*"
    EXECUTE: "/home/example/path/*"

    READ: "/etc/random.conf"
    READ: "/tmp"

    NET_CONNECT: TCP 1.2.3.0/24 80 443
    NET_BIND: TCP * 1024-65535
    NET_LISTEN: TCP 80
    NET_RAW: ETHERNET
}
```

A `.cuser` file is structured like this:
```text
{
    ADD: "/cmp/set/default.cset"
    REMOVE: "/cmp/set/etest.cset"
    ADD: "/cmp/set/audio.cset"
    ADD: "/cmp/set/video.cset"
    ADD: {
       READ: "/home/example/path/*"
    }
}
```

It is important to remember that the prefix of a `.cset` file must match the username.

These files can be modified and updated through syscalls.
The syscall will read the file and write it at a low level using inode objects inside `/cmp/fin`.

With user-space programs, a cache system in `/cmp/fin/cache` will allow an inode object to update its high-level file position if its location changes.

In the end, everything will become compiled and cached inside `/cmp/fin/`, where every object will be represented as either an `inode_range` or an `inode`.

## Init System
The init system is the only program started by the kernel. It is located in the directory `/bin/AgInit*`. This file reads a configuration file in `/etc/init.aegis` and starts each process in order.
When starting a process, it also sets the `cap` using a `.cset` for the launched process.

An example of how `/etc/init.aegis` will be structured:

```text
service filesystem {
    exec: "/bin/fsd"
    caps: "/cmp/set/fs.cset"
    restart: never
}

service network {
    exec: "/bin/netd"
    caps: "/cmp/set/net.cset"
    depends: filesystem
}

service shell {
    exec: "/bin/sh"
    caps: "/cmp/set/user.cset"
    depends: filesystem network
}
```

## Roadmap

**Done:** 
- ISR/IRQ, PIC, VGA, Keyboard, Kernel Shell, PMM

**In Progress:** 
- EXT2 Filesystem

**Planned:**
- 64-bit kernel, Virtual Memory, Timer, Scheduler, Context Switch, Syscalls, User Mode, ELF Loader, Init System, User Shell, Logging, Full FS

## License
This project is licensed under the GNU GPL v3.0 – see the LICENSE file for details.
