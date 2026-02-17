# AngelScript Bundler

A bundler and validator for AngelScript (.as) files that combines multiple files into a single output and validates the code.

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

```bash
as_bundler [OPTIONS] <source_dir|file.as> [file2.as ...]
```

### Options
- `-o <file>` - Output bundled code to specified file (without this, only validates)
- `--help` - Show help message

### Examples
```bash
# Validate all .as files in a directory
as_bundler src/

# Bundle and output to a file
as_bundler -o bundle.as src/

# Bundle specific files
as_bundler -o output.as main.as utils.as
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
      "command": "./build/as_bundler",
      "args": ["${workspaceFolder}/src"],
      "group": "build",
      "presentation": {
        "reveal": "always",
        "panel": "new"
      }
    }
  ]
}
```

### Zed

Create or edit `.zed/tasks.json`:

```json
{
  "tasks": [
    {
      "label": "Validate AngelScript",
      "command": "./build/as_bundler",
      "args": ["$PROJECT/src"]
    }
  ]
}
```

To run tasks: `Ctrl + Shift + P` → "Tasks: Run Task"
