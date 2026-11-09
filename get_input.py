"""Download an Advent of Code input as inputs/day<N>.inc, ready to #include.

Usage: python get_input.py <day> [--force]

Reads your session cookie from the git-ignored .aoc_session file next to this script.
"""

import argparse
import sys
import urllib.error
import urllib.request
from pathlib import Path

YEAR = 2026
BASE_URL = "https://adventofcode.com"
USER_AGENT = "github.com/Brochu/advent-of-code-2026 get_input.py"

ROOT = Path(__file__).resolve().parent
INPUT_DIR = ROOT / "inputs"
SESSION_FILE = ROOT / ".aoc_session"

COLS = 18


def to_inc(data: bytes) -> str:
    # The trailing NUL matters: solutions build their strview with strlen(in).
    data = data.replace(b"\r", b"") + b"\0"
    rows = (data[i : i + COLS] for i in range(0, len(data), COLS))
    return "".join("".join(f"0x{b:02x}," for b in row) + "\n" for row in rows)


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("day", type=int, choices=range(1, 26), metavar="day", help="puzzle day (1-25)")
    parser.add_argument("-f", "--force", action="store_true", help="download again if the file exists")
    args = parser.parse_args()

    target = INPUT_DIR / f"day{args.day}.inc"
    if target.exists() and not args.force:
        print(f"{target.name} already exists, use --force to download it again")
        return

    if not SESSION_FILE.exists():
        sys.exit(f"No session cookie: create {SESSION_FILE}")

    request = urllib.request.Request(
        f"{BASE_URL}/{YEAR}/day/{args.day}/input",
        headers={"Cookie": f"session={SESSION_FILE.read_text().strip()}", "User-Agent": USER_AGENT},
    )
    try:
        with urllib.request.urlopen(request, timeout=30) as response:
            data = response.read()
    except urllib.error.URLError as e:
        sys.exit(f"Download failed: {e} (404: day not unlocked yet, 400 or 500: bad or expired session cookie)")

    INPUT_DIR.mkdir(exist_ok=True)
    target.write_bytes(to_inc(data).encode("ascii"))
    print(f"Wrote {target.name} ({len(data)} bytes of input)")


if __name__ == "__main__":
    main()
