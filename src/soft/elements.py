from soft.style import FormatStyle

class Element:
    __element: dict = {}

    def __init__(self, tag: str,**kwargs):
        self.__element = { "tag": tag, **kwargs }

    @property
    def get_element(self) -> dict:
        return self.__element
    
class Text(Element):
    def __init__(self, text: str = "", children: dict = {}, format_style: FormatStyle | None = None):
        super().__init__("text", text=text, children=children, format_style=format_style)
