from soft.style import Style

class Element:
    __element: dict = {}

    def __init__(self, tag: str, src = "", text: str = "", children: list = [], style: Style = Style()) -> None:
        self.__element = { "tag": tag, "src": src, "text": text, "children": children, "style": style.style}

    @property
    def element(self) -> dict:
        return self.__element
    
class View(Element):
    def __init__(self, *children, style: Style = Style()) -> None:
        super().__init__("view", children=[child.element for child in children], style=style)
    
class Text(Element):
    def __init__(self, text: str = "", children: list = [], style: Style = Style()) -> None:
        for child in children:
            if child.element["tag"] in ["text", "image"]:
                raise ValueError(f"Text element can only have {child.element['tag']} children")

        super().__init__("text", text=text, children=children, style=style)

class Image(Element):
    def __init__(self, src: str, style: Style = Style()) -> None:
        super().__init__("image", src = src, style = style)