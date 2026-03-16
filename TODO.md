# Todos

Optional changes from bundler/validator to multi tool

## Sub commands

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

### restore

Only needs mapping for obfuscated code (might never add tbh...)
For obfuscated scripts perhaps add option to export a build.map which holds original source names

`./pcx restore <bundled.as>`

`./pcx update`

## Extending

### Ignore list

Custom ignore list for the bundler, includes:
- variables
- functions
- namespaces

### Obfuscation area

Specify areas you want to obfuscate


-----

# Obfuscation Improvements — TODO

This file documents planned obfuscation enhancements for the bundler.
All transforms must produce valid plain `.as` source that the host
application’s AngelScript compiler can accept without modification.

Each item describes what to build, where in the codebase it lives, and
enough implementation detail to get started without needing prior
context on the codebase.

-----

## 1. Numeric Literal Obfuscation

**What it does:**
Replaces integer constants in the source with equivalent XOR expressions
that evaluate to the same value at compile time but look like runtime
noise to a human reader. For example `100` becomes `(0x6B ^ 0x1F)`.

**Why it matters:**
Currently `OT_NUMBER` tokens are emitted verbatim in `obfuscate_content`
inside `obfuscator.c`. This is the last category of plain readable
values after identifier renaming and string scrambling are applied.

**Where to implement:**
Inside `obfuscate_content` in `src/obfuscator/obfuscator.c`, in the
`switch(tk->type)` reconstruction loop. There is already a `case OT_NUMBER:` block that currently just emits the token verbatim — this
is where the transform goes.

**How to implement:**

1. Parse the token text into an integer value using `strtoll` (handle
   decimal, `0x` hex, and suffix variants like `u`, `f`, `L` which are
   already lexed by the tokenizer).
1. Pick a random mask `A` using `rand()` seeded from the build unix
   timestamp (already available via `apply_timestamp_macros` — pass the
   seed into the obfuscator or seed once in `main.c` before calling
   `obfuscate_content`).
1. Compute `B = A ^ value`. Emit `(A ^ B)` as the replacement.
1. Float literals should be left alone or wrapped in a trivial identity
   like `(val * 1.0f)` — avoid breaking precision-sensitive code.
1. Skip very small constants (0, 1, -1) where wrapping adds more noise
   than benefit and may confuse the reader of the TODO less.
1. Add this as a new CLI flag `--obfuscate-numbers` / `-N` following the
   same pattern as the existing flags in `main.c` and the global options
   in `common/common.h`. Gate it behind `g_obfuscate_numbers` just as
   string scrambling is gated behind `g_scramble_strings`.

-----

## 2. Opaque Predicates (Dead Code Injection)

**What it does:**
Inserts `if` blocks into the output that always evaluate to false and
never execute, but look meaningful to a human reader. The dead branch
contains plausible-looking but misleading code.

**Why it matters:**
After all other transforms, the overall *structure* of the code — its
statement count, the number of branches — is still legible. Opaque
predicates destroy that legibility with minimal runtime cost (the AS
compiler constant-folds them away).

**Where to implement:**
Add a new source file `src/obfuscator/opaque.c` with a corresponding
`opaque.h`, following the same file layout as the existing obfuscator
files. Call it as a post-pass in `main.c` after `obfuscate_content`,
just before the final write in Step 11.

**How to implement:**

1. Tokenize the already-obfuscated output (reuse `obf_tokenize` from
   `obfuscator.c` — expose it via the header or factor it into a shared
   `tokenizer.c`).
1. Walk the token stream looking for statement boundaries: a `;` token
   followed by a newline (or, if `--remove-newlines` is active, a `;`
   followed by an identifier or `}`).
1. At a random subset of those boundaries (e.g. inject at roughly every
   5th–10th statement, using `rand()`), insert a dead block. A good
   template:
   
   ```
   {int _XX=BUILD_UNIX_TS;if((_XX*_XX+1)==0){<dead body>;}}
   ```
   
   `_XX` should use a freshly generated obfuscated name (call
   `gen_obf_name` with a counter that continues from where
   `obfuscate_content` left off to avoid name collisions).
   `BUILD_UNIX_TS` is already substituted to a numeric literal by the
   time this pass runs, so it looks like a variable to a reader but is
   a compile-time constant — the predicate is always false.
1. The dead body can be a call to one of the already-emitted obfuscated
   functions with dummy arguments, or an assignment to `_XX`. Keep it
   short — one statement is enough.
1. Add CLI flag `--opaque-predicates` / `-P` in `main.c` and a global
   `g_opaque_predicates` in `common/common.h`. Add the protected names
   for any injected temporaries to `OBF_KEYWORDS` in `obfuscator.c` so
   a second obfuscation pass would not rename them.

-----

## 3. XOR Key Expression Obfuscation (String Scrambler Hardening)

**What it does:**
The current `scramble_strings` in `obfuscator.c` emits the XOR decrypt
key as a plain byte literal, e.g. `}, 0x5a));`. This item replaces that
literal with a constant-folding expression so the key is not immediately
readable.

**Why it matters:**
Someone reading the output can trivially XOR the byte arrays back to
plaintext once they identify the key. Hiding the key as an expression
adds one more step even if the structure of `__dec` is understood.

**Where to implement:**
In `scramble_strings` inside `src/obfuscator/obfuscator.c`, in section 2
(the prologue builder) where the line
`snprintf(buf, sizeof(buf), "},0x%02x", (unsigned)s->xor_key);`
is written.

**How to implement:**

1. Split the key `k` into two halves: pick random byte `r` using
   `rand()`, then compute `s = r ^ k`. Emit `(uint8(r)^uint8(s))`
   instead of `0xNN`. Both `r` and `s` are compile-time constants so
   AS folds them, but the key is no longer a single readable literal.
1. Optionally go one step further and derive `r` from the FNV-32 hash
   of the string’s hash key modulo 256 — this makes each key expression
   unique per string entry rather than just random per build.
1. No new CLI flag needed; this is purely an internal hardening of the
   existing `--scramble-strings` path.

-----

## 4. String Table Init Shuffling

**What it does:**
Randomises the order in which `__str_table.set(...)` calls are emitted
inside `__init_str_table`, and interleaves the dead `if` blocks from
item 2 between them.

**Why it matters:**
Currently the init function is a perfectly sequential, readable list.
The insertion order has no effect on the `hash_map` at runtime, so
shuffling it is free.

**Where to implement:**
In `scramble_strings` in `src/obfuscator/obfuscator.c`, in section 2
(prologue builder), after `scrarr_push` has built the full `ScrArr`.

**How to implement:**

1. Before emitting the `for` loop over `strs.d`, perform a Fisher-Yates
   shuffle on the `ScrArr`. Use `rand()` seeded from the build unix
   timestamp (seed once in `main.c` and let it propagate — the existing
   `apply_timestamp_macros` call happens before `scramble_strings` so
   the timestamp is already resolved to an integer in the content, but
   you still have the C-side `time_t` from `processing.c` available if
   you store it in a global).
1. If opaque predicates (item 2) are enabled, after every N entries
   (e.g. every 3–5) insert a dead `if` statement into the prologue
   string using `dstr_push_str`. The dead statement can reuse the same
   `__gs(key)` pattern: calling `__gs` with a key that does not exist
   in the table returns an empty string harmlessly.
1. No new CLI flag needed; shuffling is always active when
   `--scramble-strings` is used, and the interleaving only triggers if
   `--opaque-predicates` is also set.

-----

## 5. Identifier Generation Order Shuffling

**What it does:**
The current `gen_obf_name` in `obfuscator.c` assigns names in a fixed
lexicographic sequence: `_0, _1, _2 … _a, _b …`. This means that
`_0` is always the identifier declared earliest in the file, making
the relative order of declarations trivially recoverable.

**Why it matters:**
A reader can reconstruct declaration order just by sorting the obfuscated
names, which undoes much of the renaming effort.

**Where to implement:**
In `obfuscate_content` in `src/obfuscator/obfuscator.c`, in section 4
(assign short obfuscated names), where `gen_obf_name` is called in a
loop over `idents.d`.

**How to implement:**

1. Pre-generate a pool of N candidate names (where N is the number of
   identifiers to rename plus a small buffer) by calling `gen_obf_name`
   sequentially into a `char[][12]` array.
1. Fisher-Yates shuffle that pool using `rand()` seeded from the build
   timestamp.
1. Assign names from the shuffled pool instead of calling `gen_obf_name`
   in order. Skip any name that collides with the protected set (the
   existing `while(pset_contains(...))` guard still applies).
1. This means every build produces a different mapping, so two bundles
   built from the same source are not trivially diff-able. This is a
   small but free win.
1. No new CLI flag needed; active whenever `--obfuscate` is used.

-----

## 6. Long String Splitting

**What it does:**
Splits long string literals into two halves, stores each half as a
separate entry in the scrambled string table, and emits a concatenation
`__gs(key_a) + __gs(key_b)` instead of a single `__gs(key)` call.

**Why it matters:**
Any string long enough to be recognisable (function names, format
strings, log messages) is stored as a single traceable hash key. Splitting
breaks that single-key lookup and forces an attacker to reassemble both
halves.

**Where to implement:**
In `scramble_strings` in `src/obfuscator/obfuscator.c`, in section 1
(collect unique string literals), add a length threshold check, and in
section 3 (rebuild source), emit the split form when the threshold was
exceeded.

**How to implement:**

1. Define a threshold, e.g. strings longer than 8 decoded bytes are
   candidates for splitting. Make the threshold a compile-time constant
   or an optional CLI argument (`--split-threshold <N>`).
1. When a string exceeds the threshold, split its decoded byte array at
   the midpoint. Compute FNV-64 hashes and XOR keys for each half
   independently and push both into `ScrArr`.
1. In the init prologue, both halves get their own `__str_table.set`
   call as normal — no special casing needed there.
1. In the source reconstruction pass (section 3), when you would emit
   `__gs(full_key)`, instead emit `(__gs(key_a)+__gs(key_b))`.
1. Be careful with the empty-string guard (`if (blen == 0) continue`)
   — after splitting, neither half should be empty, so only split when
   each half is at least 1 byte.

-----

## Implementation Order Recommendation

The items above are roughly ordered from easiest to most work. A
suggested implementation order that delivers visible improvement at
each step:

1. **Item 5** — Identifier shuffle. Touches only a few lines in
   `obfuscate_content`, zero new files, immediate entropy improvement.
1. **Item 3** — XOR key expression. A two-line change in the prologue
   builder, no new flags.
1. **Item 4** — String table init shuffling. A Fisher-Yates loop before
   the emit loop, no new flags.
1. **Item 1** — Numeric literal obfuscation. New case in the token
   reconstruction switch, one new flag, moderate complexity.
1. **Item 6** — Long string splitting. Moderate complexity, touches
   multiple sections of `scramble_strings`.
1. **Item 2** — Opaque predicates. Most involved: new source file, new
   flag, needs careful name collision avoidance with existing identifiers.
