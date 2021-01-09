# ttcc-sim
A simulator of TTCC's battle mechanics

## Command format for one liners
`[pres] [quickhand-strat and/or cross] [number] [pres] <gag>[s] (target) ...[[number] [pres] <gag>[s] (target)]`

A valid command may have any number of the command block defined above. Each command block may have multiple gag blocks (`[number] [pres] <gag>[s] (target)`). The order of each command block does not matter, but the order of gag blocks within each command block may matter.

The command is parsed left to right, but gags will be automatically applied in the conventional order (Fire, Toonup, Trap, Lure, Sound, Squirt, Zap, Throw, Drop). By default, when equivalent gags are used, *the cog furthest to the right is attacked first* (straight ordering). A cross ordering option is also provided.

Consider the following: `hypno pres wreckingball left 2 anvil left` is valid and can be separated into three blocks in the correct order: `pres wreckingball left`, `hypno`, `2 anvil left`.

The order between the quickhand strategy and `cross` does not matter.

To simulate all toons passing, the user can simply supply an empty command.

For one-liners, a quickhand strategy can be used for efficiency.

### Quickhand strategy format:
* the quickhand strategy consists of only `-`, `x`, or `X`
    * `-` specifies that the respective cog is not targeted
    * `x` specifies that the respective cog is targeted once
    * `X` specifies that the respective cog is targeted twice
    * should be the same length as the cog set
* the quickhand strategy must be followed by the gags relevant to it in the format `[number] [pres] <gag>[s]`
    * gags are parsed left to right
    * example 1: if `x-x-` or `-X--` is specified, two valid gags must follow it
    * example 2: `-X-x` will assume the next two gags target mid-left and the third gag targets right
* if preceded by `pres`, all gags that are part of the quickhand strategy are prestiged
    * note: if followed by `pres`, the prestige only applies to the gag following it
* if neighboring a `cross`, supplied gags will be mapped to apply left to right
* if a gag following the strategy is pluralized, the gag will be "spread" out to apply to every position not already assigned; however, pluralized gags with a number in front will only spread out to the amount specified
    * this means that `x-x- tvs` will spread out to `tv mid-right` and `tv left`
* if the number of gags is supplied, they will be spread out from right to left (or left to right if crossed) until all are expended

### Crossing gags
When the user specifies `cross` before gags, gags of the same kind and damage will cross so that they are applied left to right.
* `x-x- pres tv tv` translates to `tv mid-right` first, then `pres tv left`
* `x-x- cross pres tv tv` translates to `tv left` first, then `pres tv mid-right`

If `cross` is specified without the quickhand strategy, crossing will only be applied until gags are not equivalent both in kind and damage. As an example, `cross tesla mid-right tv left` will disregard crossing and instead simply translate to `tv left` first, then `tesla mid-right` (since weaker gags go first).

### Complex one-liner examples:
* `pres -xXx 3 buzzer flower` translates to `pres flower right`, then `pres 2 buzzer mid-right` and `pres buzzer mid-left`
* `xx-X cross 2 pres anvil 2 anvils` translates to `2 pres anvil right`, `anvil mid-left`, and `anvil left` (the pluralized `anvils` did not matter here)
* `cross x-Xx 2 teslas pres clouds` translates to `pres cloud left` and `pres cloud mid-right` (cloud spreads out to the remaining targets), then `tesla mid-right` and `tesla right` (teslas are assigned targets first because they are specified first, and they take the two rightmost targets because of `cross`)

At a certain point, one liners may become too complex. Therefore, the program also provides parsing by toon, as explained below.

## Command format for individual commands
`[pres] <gag> <target>`

Gags will be automatically applied as expected in-game. If multiple toons use the same gag, the rightmost toon's gag goes first.

## Special notes about commands

Do not specify a target for group lure, sound, or SOS, or when the cog set only consists of one cog.

For cog sets larger than 1, the specified target can be a 0-indexed position (prepended with `!`), a defined position, or a cog's level name. In the case where a level name is supplied, the target will be the leftmost (first) occurrence.

Accuracies are calculated with the assumption that all gag tracks are maxed, so the "track experience" factor is always 70.

### Defined positions:
| name        | meaning       |
|-------------|---------------|
| `left`      | leftmost cog  |
| `mid-left`  | second cog    |
| `mid`       | second cog    |
| `mid-right` | third cog     |
| `right`     | rightmost cog |

### Other commands:
| command                   | meaning                                                    |
|---------------------------|------------------------------------------------------------|
| `UNDO`                    | undo last toon's choice (**only for individual commands**) |
| `PASS`                    | pass a toon's turn (**only for individual commands**)      |
| `HISTORY`                 | list history of turns                                      |
| `REVERT [#]`              | revert a turn - if # supplied, reverts to before that turn |
| `ALLPASS`                 | all toons pass                                             |
| `fire <target>`           | fire the targeted cog (can treat this like a normal gag)   |
| `DELETE`/`FIREALL`/`SKIP` | fire all cogs                                              |
| `END`                     | quit the program                                           |

## Configuration
The program is able to detect a configuration file named `conf.txt` in the same directory. If not present, the program will ask if the user wants to create one. Based on the response, it may store the battle configurations that it queries the user for in a new `conf.txt`.

### `conf.txt` format
The text file can have the configurations in any order, but each configuration must be in the format `[configuration_name]:[option]`. Whitespace between configurations and between the ':' and option is optional.

Below is the list of configurations that are parsed:

| configuration name | description                                       | allowed values                         | default state |
|--------------------|---------------------------------------------------|----------------------------------------|---------------|
| all_prestige       | All gags are prestige                             | `0`, `1`, `true`, `false`, `on`, `off` | enabled       |
| perfect_acc        | Gags always hit                                   | `0`, `1`, `true`, `false`, `on`, `off` | enabled       |
| no_decay           | Lure and soak rounds do not count down            | `0`, `1`, `true`, `false`, `on`, `off` | enabled       |
| line_input         | Accept input on one line instead of for each toon | `0`, `1`, `true`, `false`, `on`, `off` | enabled       |
| gag_file           | Relative file path for the gags specifications    | a valid file path                      | gags.txt      |

Any configurations not specified in the file will assume the default settings.

If the program cannot find the gags specifications file, it will assume the standard maximum default for all gags.
