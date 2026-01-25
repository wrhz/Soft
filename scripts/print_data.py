from enum import Enum

class Color(Enum):
    RED = "31"
    GREEN = "32"
    YELLOW = "33"
    BLUE = "34"
    MAGENTA = "35"
    CYAN = "36"
    WHITE = "37"

def print_data(data, color: Color):
    print(f"\033[{color.value}m{data}\033[0m")