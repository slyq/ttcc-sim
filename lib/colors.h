#ifndef COLORS_H
#define COLORS_H

#define TRAPPED rang::fgB::red << rang::style::bold
#define LURED rang::bgB::green << rang::style::bold
#define SOAKED rang::fgB::blue << rang::style::bold
#define ZAPPED rang::fgB::yellow << rang::style::bold
#define THROWN rang::fgB::gray << rang::style::bold
#define DROPPED rang::fgB::cyan << rang::style::bold

#define DEAD rang::bgB::red << rang::style::bold
#define PRESLURED "*"
#define ATTACKED "|"
#define NOTATTACKED " "

#define CONFIG rang::fgB::cyan << rang::style::bold
#define PROMPT rang::fgB::magenta << rang::style::bold

#endif
