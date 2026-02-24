from soft.elements import Text, View
from soft.style import Style

def hello():
    style = Style(
        display = "flex",
        flex_direction = "column",
        align_items = "center",
        justify_content = "center",
    )

    return View(
        Text(
            "Hello Soft!"
        ),
        Text(
            "Hello Soft!"
        ),
        style=style,
    )