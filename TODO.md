# Todos

Optional changes from bundler/validator to multi tool

## Sub commands

### help

`./pcx <command> --help`

`./pcx --help`


### setup

Setup a project with files and demo code from templates

`./pcx setup <setup type>`

Example:
```
.
└── source
    ├── attachment.as
    ├── caching
    │   ├── player_cache.as
    │   └── player_info.as
    ├── main.as
    ├── rendering
    │   ├── elements
    │   │   ├── rect.as
    │   │   └── text.as
    │   └── fonts.as
    ├── thread.as
    └── utility
        ├── bounds.as
        ├── process.as
        ├── string.as
        ├── types
        │   ├── color.as
        │   └── player.as
        └── w2s.as
```

### watch

Bundle all source files automatically when save is detected in any of the source files

`./pcx watch <bundler options>`

### restore

Restore original project structure from bundled file.
For obfuscated scripts perhaps add option to export a build.map which holds original source names

`./pcx restore <bundled.as>`

### install

Setup and install gcc, register pcx as command in PATH

`./pcx install`

### update

Update from the latest github release, potential for update check when launched.
Also checks for gcc and registers in PATH

`./pcx update`

## Extending

### Ignore list

Custom ignore list for the bundler, includes:
- variables
- functions
- namespaces

### Obfuscation area

Specify areas you want to obfuscate
