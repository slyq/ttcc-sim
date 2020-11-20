# ttcc-sim
A simulator of TTCC's battle mechanics (WIP)

## Command format for one liners:
`[number/strategy] [pres] <gag(s)> <target> ...[[number/strategy] [pres] <gag(s)> <target>]`
Do not specify a target for group lure and sound.

You do not need to specify a target for a cog set of one cog.

Gags will be automatically applied in the conventional order.

The command order for each gag matters. You can move around blocks of gag commands any way you like, however.

Target can be a defined position or a cog's level name. In the case where a level name is supplied, the target will be the leftmost (first) occurrence.

## Quickhand strategy format:
- consists of only '-', 'x', or 'X'
    - '-' specifies that the respective cog is not targeted
    - 'x' specifies that the respective cog is targeted once
    - 'X' specifies that the respective cog is targeted twice
- is the length of the cog set
- must be followed by the number of gags specified
- if preceded by "pres", gags that are part of the quickhand strategy are prestiged

For example, if x-x- or -X-- is specified, two valid gags must follow it.

## Command format for individual commands:
`[pres] <gag> <target>`

Target can be a 0-indexed position, a defined position, or a cog's level name. In the case where a level name is supplied, the target will be the leftmost (first) occurrence.

## Defined positions:
- left = leftmost cog
- mid-left = second cog
- mid = second cog
- mid-right = third cog
- right = rightmost cog (automatically updates based on cog set size)
