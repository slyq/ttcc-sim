# ttcc-sim
A simulator of TTCC's battle mechanics (WIP)

Command format for one liners:
- [number/strategy] <gag(s)> <target> ...[[number/strategy] <gag(s)> <target>]
Do not specify a target for group lure and sound.
You do not need to specify a target for a cog set of one cog.
Gag order does not matter, but the command order for each gag does matter.
Target can be a defined position or a cog's level name. In the case where a level name is supplied, the target will be the leftmost (first) occurrence.

Quickhand strategy format:
- consists of only '-', 'x', or 'X'
    - '-' specifies that the respective cog is not targeted
    - 'x' specifies that the respective cog is targeted once
    - 'X' specifies that the respective cog is targeted twice
- is the length of the cog set
- must be followed by the number of gags specified
For example, if x-x- or -X-- is specified, two valid gags must follow it.

Command format for individual commands:
- <gag> <target>
Target can be a 0-indexed position or a defined position.

Defined positions:
- left = leftmost cog
- mid-left = second cog
- mid = second cog
- mid-right = third cog
- right = rightmost cog (automatically updates based on cog set size)
