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
- `--strip` - Strip comments from bundled file
- `--help` - Show help message

### Examples
```bash
# Validate all .as files in a directory
as_bundler src/

# Bundle and output to a file
as_bundler -o bundle.as src/

# Bundle, strip comments and output to a file
as_bundler --strip -o bundle.as src/

# Bundle specific files
as_bundler -o output.as main.as utils.as
```

## Editor Integration

### VSCode / VSCodium

Move `as_bundler` to .vscode folder.

Create or edit `.vscode/tasks.json`:

```json
{
  "version": "2.0.0",
  "tasks": [
    {
      "label": "Validate AngelScript",
      "type": "shell",
      "command": "./as_bundler",
      "args": ["${workspaceFolder}/src"],
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
      	"${workspaceFolder}/src"
      ],
      "group": "build",
      "presentation": {
        "reveal": "always",
        "panel": "new"
      }
    },
    {
      "label": "Build AngelScript stripped (error locations)",
      "type": "shell",
      "command": "./as_bundler",
      "args": [
      	"--strip",
      	"-o",
       	"C:/Users/username/Documents/My Games/output.as",
      	"${workspaceFolder}/src"
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

### Zed

Move `as_bundler` to .zed folder.

Create or edit `.zed/tasks.json`:

```json
[
	{
		"label": "Validate AngelScript",
		"command": ".zed/as_bundler $ZED_WORKTREE_ROOT/source",
	},
	{
		"label": "Build AngelScript",
		"command": ".zed/as_bundler -o 'C:/Users/username/Documents/My Games/output.as' $ZED_WORKTREE_ROOT/source",
	},
	{
		"label": "Build AngelScript stripped (error locations)",
		"command": ".zed/as_bundler --strip -o 'C:/Users/username/Documents/My Games/output.as' $ZED_WORKTREE_ROOT/source",
	},
]
```

Modify your windows username and output filename.

To run tasks: `Ctrl + Shift + P` → "Tasks: Run Task"
