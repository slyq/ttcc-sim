# ttcc-sim
A simulator of TTCC's battle mechanics (WIP)

## Command format for one liners
`[number/strategy] [pres] <gag(s)> <target> ...[[number/strategy] [pres] <gag(s)> <target>]`

The command is parsed left to right, but gags will be automatically applied in the conventional order (Fire, Toonup, Trap, Lure, Sound, Squirt, Zap, Throw, Drop). By default, when equivalent gags are used, *the cog furthest to the right is attacked first*. For more precise commands, use the other command format.

The command order for each gag matters. However, blocks of gag commands can be specified in any order.

Consider the following: `hypno pres wreckingball left 2 anvil left` is valid and can be separated into three blocks in the correct order: `pres wreckingball left`, `hypno`, `2 anvil left`.

For one-liners, a quickhand strategy can be used for efficiency.

### Quickhand strategy format:
* the quickhand strategy consists of only '-', 'x', or 'X'
    * '-' specifies that the respective cog is not targeted
    * 'x' specifies that the respective cog is targeted once
    * 'X' specifies that the respective cog is targeted twice
    * should be the same length as the cog set
* the quickhand strategy must be followed by the gags relevant to it in the format `[pres] <gag>`
    * gags are parsed left to right
    * example 1: if x-x- or -X-- is specified, two valid gags must follow it
    * example 2: -X-x will assume the next two gags target mid-left and the third gag targets right
* if preceded by "pres", all gags that are part of the quickhand strategy are prestiged
    * note: if followed by "pres", the prestige only applies to the gag following it

### Crossing for zap
When equivalent zap gags are used, the cog furthest to the right is zapped first. When you specify `cross` anywhere, zap gags of the same damage will cross so that they are applied left to right.
* `x-x- pres tv tv` translates to `tv mid-right` first, then `pres tv left`
* `x-x- cross pres tv tv` translates to `pres tv left` first, then `tv mid-right`


## Command format for individual commands
`[pres] <gag> <target>`

Gags will be automatically applied as expected in-game. If multiple toons use the same gag, the rightmost toon's gag goes first.

## Special notes about commands

Do not specify a target for group lure, sound, or SOS, or when the cog set only consists of one cog.

For cog sets larger than 1, the specified target can be a 0-indexed position (prepended with `!`), a defined position, or a cog's level name. In the case where a level name is supplied, the target will be the leftmost (first) occurrence.

### Defined positions:
| name        | meaning       |
|-------------|---------------|
| `left`      | leftmost cog  |
| `mid-left`  | second cog    |
| `mid`       | second cog    |
| `mid-right` | third cog     |
| `right`     | rightmost cog |

### Non-gag commands:
| command                   | meaning                                                                       |
|---------------------------|-------------------------------------------------------------------------------|
| `UNDO`                    | undo last toon's choice (**only for individual commands**)                    |
| `PASS`                    | pass (**only for individual commands** - line equivalent is an empty command) |
| `FIRE <target>`           | fire the targeted cog                                                         |
| `DELETE`/`FIREALL`/`SKIP` | fire all cogs                                                                 |
| `END`                     | quit the program                                                              |

## Configuration
The program is able to detect a configuration file named `conf.txt` in the same directory. If not present, the program will instead query the user for each configuration for the battle.

### `conf.txt` format
The text file can have the configurations in any order, but they must be separated by newlines and be in the format `configuration_name: option`.

Below is the list of configurations that are parsed:

| configuration name | description                                       | allowed values                         | default state |
|--------------------|---------------------------------------------------|----------------------------------------|---------------|
| all_prestige       | All gags are prestige                             | `0`, `1`, `true`, `false`, `on`, `off` | enabled       |
| no_miss            | Gags always hit                                   | `0`, `1`, `true`, `false`, `on`, `off` | enabled       |
| round_decay        | Lure and soak rounds count down                   | `0`, `1`, `true`, `false`, `on`, `off` | disabled      |
| line_input         | Accept input on one line instead of for each toon | `0`, `1`, `true`, `false`, `on`, `off` | enabled       |
| gag_file           | Relative file path for the gags specifications    | a valid file path                      | gags.txt      |

Any configurations not specified in the file will assume the default settings.
