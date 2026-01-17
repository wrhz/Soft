from soft.style import Style

class Element:
    __element: dict = {}

    def __init__(self, tag: str, text: str = "", children: dict = {}, style: Style = Style(), **kwargs):
        self.__element = { "tag": tag, "text": text, "children": children, "style": style.style, **kwargs }

    @property
    def get_element(self) -> dict:
        return self.__element
    
class Text(Element):
    def __init__(self, text: str = "", children: dict = {}, style: Style = Style()):
        super().__init__("text", text=text, children=children, style=style)