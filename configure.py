#!/usr/bin/env python3

###
# Generates build files for the project.
# This file also includes the project configuration,
# such as compiler flags and the object matching status.
#
# Usage:
#   python3 configure.py
#   ninja
#
# Append --help to see available options.
###

import argparse
import sys
from pathlib import Path
from typing import Any, Dict, List

from tools.project import (
    Object,
    ProjectConfig,
    calculate_progress,
    generate_build,
    is_windows,
)

# Game versions
DEFAULT_VERSION = 0
VERSIONS = [
    "RSBE01",  # 0
]

parser = argparse.ArgumentParser()
parser.add_argument(
    "mode",
    choices=["configure", "progress"],
    default="configure",
    help="script mode (default: configure)",
    nargs="?",
)
parser.add_argument(
    "--version",
    choices=VERSIONS,
    type=str.upper,
    default=VERSIONS[DEFAULT_VERSION],
    help="version to build",
)
parser.add_argument(
    "--build-dir",
    metavar="DIR",
    type=Path,
    default=Path("build"),
    help="base build directory (default: build)",
)
parser.add_argument(
    "--binutils",
    metavar="BINARY",
    type=Path,
    help="path to binutils (optional)",
)
parser.add_argument(
    "--compilers",
    metavar="DIR",
    type=Path,
    help="path to compilers (optional)",
)
parser.add_argument(
    "--map",
    action="store_true",
    help="generate map file(s)",
)
parser.add_argument(
    "--no-asm",
    action="store_true",
    help="don't incorporate .s files from asm directory",
)
parser.add_argument(
    "--debug",
    action="store_true",
    help="build with debug info (non-matching)",
)
if not is_windows():
    parser.add_argument(
        "--wrapper",
        metavar="BINARY",
        type=Path,
        help="path to wibo or wine (optional)",
    )
parser.add_argument(
    "--dtk",
    metavar="BINARY | DIR",
    type=Path,
    help="path to decomp-toolkit binary or source (optional)",
)
parser.add_argument(
    "--sjiswrap",
    metavar="EXE",
    type=Path,
    help="path to sjiswrap.exe (optional)",
)
parser.add_argument(
    "--verbose",
    action="store_true",
    help="print verbose output",
)
args = parser.parse_args()

config = ProjectConfig()
config.version = args.version
version_num = VERSIONS.index(config.version)

# Apply arguments
config.build_dir = args.build_dir
config.dtk_path = args.dtk
config.binutils_path = args.binutils
config.compilers_path = args.compilers
config.debug = args.debug
config.generate_map = args.map
config.sjiswrap_path = args.sjiswrap
if not is_windows():
    config.wrapper = args.wrapper
if args.no_asm:
    config.asm_dir = None

# Tool versions
config.binutils_tag = "2.42-1"
config.compilers_tag = "20240706"
config.dtk_tag = "v1.3.0"
config.objdiff_tag = "v2.4.0"
config.sjiswrap_tag = "v1.2.0"
config.wibo_tag = "0.6.11"

# Project
config.config_path = Path("config") / config.version / "config.yml"
config.check_sha_path = Path("config") / config.version / "build.sha1"
config.asflags = [
    "-mgekko",
    "--strip-local-absolute",
    "-I include",
    f"-I build/{config.version}/include",
    f"--defsym version={version_num}",
]
config.ldflags = [
    "-fp hardware",
    "-nodefaults",
    # "-listclosure", # Uncomment for Wii linkers
]

# Base flags, common to most GC/Wii games.
# Generally leave untouched, with overrides added below.
cflags_base = [
    "-nodefaults",
    "-proc gekko",
    "-align powerpc",
    "-enum int",
    "-fp hardware",
    "-Cpp_exceptions off",
    # "-W all",
    "-O4,p",
    "-inline auto",
    '-pragma "cats off"',
    '-pragma "warn_notinlined off"',
    "-maxerrors 1",
    "-nosyspath",
    "-RTTI off",
    "-fp_contract on",
    "-str reuse",
    "-enc SJIS",  # For Wii compilers, replace with `-enc SJIS`
    "-i include",
    f"-i build/{config.version}/include",
    f"-DVERSION={version_num}",
]

cflags_common = [
    *cflags_base,
    "-DMATCHING",
    "-Iinclude/st_starfox",
    "-Iinclude/lib/PowerPC_EABI_Support/Runtime/Inc",
    "-Iinclude/lib/BrawlHeaders/Brawl/Include",
    "-Iinclude/lib/BrawlHeaders/nw4r/include",
    "-Iinclude/lib/BrawlHeaders/OpenRVL/include",
    "-Iinclude/lib/BrawlHeaders/OpenRVL/include/MetroTRK",
    "-Iinclude/lib/BrawlHeaders/OpenRVL/include/revolution",
    "-Iinclude/lib/BrawlHeaders/OpenRVL/include/RVLFaceLib",
    "-Iinclude/lib/BrawlHeaders/OpenRVL/include/stl",
    "-Iinclude/lib/BrawlHeaders/utils/include",
    "-RTTI on",
    "-ipa file",
    "-inline on,noauto"
]

# Debug flags
if config.debug:
    cflags_base.extend(["-sym on", "-DDEBUG=1"])
else:
    cflags_base.append("-DNDEBUG=1")

# Metrowerks library flags
cflags_runtime = [
    *cflags_base,
    "-use_lmw_stmw on",
    "-str reuse,pool,readonly",
    "-gccinc",
    "-common off",
    "-inline auto",
]

# REL flags
config.rel_strip_partial = False
cflags_rel = [
    *cflags_common,
    "-sdata 0",
    "-sdata2 0",
]

config.linker_version = "GC/3.0a5.2"

Matching = True
NonMatching = False

config.warn_missing_config = True
config.warn_missing_source = False
config.libs = [
    {
        "lib": "Runtime.PPCEABI.H",
        "mw_version": config.linker_version,
        "cflags": cflags_runtime,
        "host": False,
        "objects": [
            Object(NonMatching, "Runtime.PPCEABI.H/global_destructor_chain.c"),
            Object(NonMatching, "Runtime.PPCEABI.H/__init_cpp_exceptions.cpp"),
        ],
    },
    {
        "lib": "ft_captain",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "ft_dedede",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "ft_diddy",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "ft_donkey",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "ft_falco",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "ft_fox",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "ft_gamewatch",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "ft_ganon",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "ft_iceclimber",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "ft_ike",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "ft_kirby",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "ft_koopa",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "ft_link",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "ft_lucario",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "ft_lucas",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "ft_luigi",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "ft_mario",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "ft_marth",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "ft_metaknight",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "ft_ness",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "ft_peach",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "ft_pikachu",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "ft_pikmin",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "ft_pit",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "ft_poke",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "ft_purin",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "ft_robot",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "ft_samus",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "ft_snake",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "ft_sonic",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "ft_toonlink",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "ft_wario",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "ft_wolf",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "ft_yoshi",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "ft_zako",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "ft_zelda",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "sora_adv_menu_difficulty",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "sora_adv_menu_ending",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "sora_adv_menu_game_over",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "sora_adv_menu_name",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "sora_adv_menu_result",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "sora_adv_menu_save_load",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "sora_adv_menu_save_point",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "sora_adv_menu_seal",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "sora_adv_menu_sel_char",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "sora_adv_menu_sel_map",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "sora_adv_menu_telop",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "sora_adv_menu_visual",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "sora_adv_stage",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "sora_enemy",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "sora_melee",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "sora_menu_boot",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "sora_menu_challenger",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "sora_menu_collect_viewer",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "sora_menu_edit",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "sora_menu_event",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "sora_menu_fig_get_demo",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "sora_menu_friend_list",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "sora_menu_game_over",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "sora_menu_intro",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "sora_menu_main",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "sora_menu_name",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "sora_menu_qm",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "sora_menu_replay",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "sora_menu_rule",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "sora_menu_sel_char",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "sora_menu_sel_char_access",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "sora_menu_sel_stage",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "sora_menu_simple_ending",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "sora_menu_snap_shot",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "sora_menu_time_result",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "sora_menu_title",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "sora_menu_title_sunset",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "sora_menu_tour",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "sora_menu_watch",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "sora_minigame",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "sora_scene",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_battle",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_battles",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_config",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_crayon",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_croll",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_dolpic",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_donkey",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_dxbigblue",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_dxcorneria",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_dxgarden",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_dxgreens",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_dxonett",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_dxpstadium",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_dxrcruise",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_dxshrine",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_dxyorster",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_dxzebes",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_earth",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_emblem",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_famicom",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_final",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_fzero",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_greenhill",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_gw",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_halberd",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_heal",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_homerun",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_ice",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_jungle",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_kart",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_madein",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_mansion",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_mariopast",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_metalgear",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_newpork",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_norfair",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_oldin",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_orpheon",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_otrain",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_palutena",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_pictchat",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_pirates",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_plankton",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_stadium",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_stageedit",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_starfox",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "global_destructor_chain.c"),
            Object(Matching, "mo_stage/st_starfox/st_starfox.cpp"),
            Object(Matching, "mo_stage/mo_stage.cpp"),
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_tbreak",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_tengan",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
    {
        "lib": "st_village",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "home_button_icon.cpp"),
        ],
    },
]

if args.mode == "configure":
    # Write build.ninja and objdiff.json
    generate_build(config)
elif args.mode == "progress":
    # Print progress and write progress.json
    config.progress_each_module = args.verbose
    calculate_progress(config)
else:
    sys.exit("Unknown mode: " + args.mode)
