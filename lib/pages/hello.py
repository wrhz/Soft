from soft.elements import Text, View
from soft.style import Style

import example_module

def hello():
    style = Style(
        display = "flex",
        flex_direction = "column",
        align_items = "center",
        justify_content = "center",
    )

    print(example_module.hello())

    return View(
        Text(
            "Hello Soft!"
        ),
        style=style,
    )