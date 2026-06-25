# Init System

The init system is the first and only process started directly by the kernel.
Its binary lives at `/bin/AgInit`.

At startup it reads `/etc/init.aegis`, starts each declared service in
dependency order, and attaches a capability set to each process.

---

## `/etc/init.aegis` format

```text
service filesystem {
    exec:    "/bin/fsd"
    caps:    "/cmp/set/fs.cset"
    restart: never
}

service network {
    exec:    "/bin/netd"
    caps:    "/cmp/set/net.cset"
    depends: filesystem
}

service shell {
    exec:    "/bin/sh"
    caps:    "/cmp/set/user.cset"
    depends: filesystem network
}
```

### Fields

| Field | Required | Description |
|---|---|---|
| `exec` | yes | Path to the service binary |
| `caps` | yes | `.cset` file attached to the process |
| `restart` | no | Restart policy: `never`, `on-failure`, `always` |
| `depends` | no | Space-separated list of services that must be running first |

### Startup order

Services without `depends` start first. Services with dependencies are
started once every dependency has reached a running state. Circular
dependencies are a configuration error and halt the boot.

---

## Relationship with the capability system

When AgInit launches a service it:
1. Forks a new process
2. Loads the `.cset` file specified by `caps`
3. Applies it via the capability syscall before `execve`

The child process receives exactly those capabilities and cannot
acquire more. Only a privileged parent can grant additional ones,
and AgInit itself only holds what it needs to start services.
