from src.soft.elements import Text
from src.soft.style import FormatStyle

def hello():
    format_style = FormatStyle(
        vertical_align = "middle",
        horizontal_align = "center",
    )

    return Text(
        "Hello Soft!",
        format_style = format_style
    )
