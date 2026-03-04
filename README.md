# AngelScript Bundler

A bundler and validator for AngelScript (.as) files that combines multiple files into a single output, validates the code, and optionally preprocesses, obfuscates, and minifies it.

## Building

### Requirements
- **CMake** 3.10+
- **C++ compiler** (GCC, Clang, or MSVC)

### Linux/macOS
```bash
mkdir build && cd build
cmake ..
make
```

The binary will be created at `build/as_bundler`.

### Windows (with Visual Studio)
```cmd
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

The binary will be created at `build/Release/as_bundler.exe`.

## Usage

```
as_bundler [OPTIONS] <source_dir|file.as> [file2.as ...]
```

### Options

| Option | Description |
|--------|-------------|
| `-o <file>` | Output bundled code to specified file (without this, only validates) |
| `--strip`, `-s` | Strip comment-only lines from output |
| `--no-preprocess`, `-n` | Skip the C preprocessor step |
| `--prepend`, `-p <file>` | Prepend file before preprocessing (macro definitions) |
| `--header`, `-H <file>` | Prepend raw content to final output (skips preprocessing) |
| `-D<NAME>[=VALUE]` | Define a preprocessor macro (repeatable) |
| `--obfuscate`, `-O` | Rename user-defined identifiers with short names; keeps all API / type names |
| `--remove-newlines`, `-R` | Collapse whitespace to minimum spaces (best combined with `--obfuscate`) |
| `--help` | Show help message |

### Build Timestamp Macros

Replaced before all other steps:

| Macro | Value |
|-------|-------|
| `__BUILD_TIMESTAMP_STR__` | String: `"YYYY-MM-DD HH:MM:SS"` |
| `__BUILD_DATE_STR__` | String: `"YYYY-MM-DD"` |
| `__BUILD_TIME_STR__` | String: `"HH:MM:SS"` |
| `__BUILD_YEAR__` | Integer: e.g. `2026` |
| `__BUILD_MONTH__` | Integer: e.g. `3` |
| `__BUILD_DAY__` | Integer: e.g. `4` |
| `__BUILD_HOUR__` | Integer: e.g. `9` |
| `__BUILD_MINUTE__` | Integer: e.g. `5` |
| `__BUILD_SECOND__` | Integer: e.g. `7` |
| `__BUILD_UNIX_TS__` | Integer: Unix timestamp |

### FNV Hash Macros

Evaluated before the C preprocessor:

| Macro | Description |
|-------|-------------|
| `STRHASH("text")` | FNV-1a 32-bit decimal hash |
| `STRHASH64("text")` | FNV-1a 64-bit decimal hash |

### Examples

```bash
# Validate all .as files in a directory
as_bundler src/

# Bundle and output to a file
as_bundler -o bundle.as src/

# Bundle, strip comments and output
as_bundler -o bundle.as --strip src/

# Bundle with a macro definitions file and license header
as_bundler -o out.as -p macros.h -H license.txt src/

# Bundle with extra preprocessor define
as_bundler -o out.as --strip -DDEBUG src/

# Skip the C preprocessor entirely
as_bundler -o out.as --no-preprocess src/

# Obfuscate and minify (rename identifiers + remove newlines)
as_bundler -o out.as --obfuscate --remove-newlines src/
```

## Editor Integration

### VSCode / VSCodium

Create or edit `.vscode/tasks.json`:

```json
{
  "version": "2.0.0",
  "tasks": [
    {
      "label": "Validate AngelScript",
      "type": "shell",
      "command": "./as_bundler",
      "args": ["${workspaceFolder}/source"],
      "group": "build",
      "presentation": {
        "reveal": "always",
        "panel": "new"
      }
    },
    {
      "label": "Build AngelScript",
      "type": "shell",
      "command": "./as_bundler",
      "args": [
        "-o",
        "C:/Users/username/Documents/My Games/output.as",
        "${workspaceFolder}/source"
      ],
      "group": "build",
      "presentation": {
        "reveal": "always",
        "panel": "new"
      }
    },
    {
      "label": "Build and Scramble AngelScript",
      "type": "shell",
      "command": "./as_bundler",
      "args": [
        "--obfuscate",
        "--remove-newlines",
        "-o",
        "C:/Users/username/Documents/My Games/output_scrambled.as",
        "${workspaceFolder}/source"
      ],
      "group": "build",
      "presentation": {
        "reveal": "always",
        "panel": "new"
      }
    }
  ]
}
```

Adjust the path to `as_bundler.exe` and the output file to match your setup.

### Zed

Create or edit `.zed/tasks.json`:

```json
[
  {
    "label": "Validate AngelScript",
    "command": ".zed/as_bundler $ZED_WORKTREE_ROOT/source"
  },
  {
    "label": "Build AngelScript",
    "command": ".zed/as_bundler -o 'C:/Users/username/Documents/My Games/output.as' $ZED_WORKTREE_ROOT/source"
  },
  {
    "label": "Build AngelScript stripped",
    "command": ".zed/as_bundler --strip -o 'C:/Users/username/Documents/My Games/output.as' $ZED_WORKTREE_ROOT/source"
  },
  {
    "label": "Build and Scramble AngelScript",
    "command": ".zed/as_bundler --obfuscate --remove-newlines -o 'C:/Users/username/Documents/My Games/output_scrambled.as' $ZED_WORKTREE_ROOT/source"
  }
]
```

Modify your path to `as_bundler` and output filename as needed.

To run tasks: `Ctrl + Shift + P` → "Tasks: Run Task"
