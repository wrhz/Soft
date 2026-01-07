from src.soft.elements import Text
from src.soft.style import FormatStyle

def hello():
    format_style = FormatStyle(
        horizontal_align = "center",
        vertical_align = "middle",
    )

    return Text(
        "Hello Soft!",
        format_style = format_style
    )