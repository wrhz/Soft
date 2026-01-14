from src.soft.elements import Text
from src.soft.style import Style

def hello():
    style = Style(
        horizontal_align = "center",
        vertical_align = "middle",
    )

    return Text(
        "Hello Soft!",
        style = style
    )