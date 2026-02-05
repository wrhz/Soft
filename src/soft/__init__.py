from soft.elements import Element

class Soft:
    title: str = "Soft Windows"
    __size = (800, 600)
    __home: None | Element = None
    __routes: dict = {}
    __default_font_family: str = "Arial"

    def __init__(self, home, title: str = "Soft Windows", size: tuple[int, int] = (800, 600), font_family: str = "Arial"):
        self.__home = home
        self.__size = size
        self.title = title
        self.__default_font_family = font_family

    def set_home(self, home):
        self.__home = home

    def register(self, route, page_function):
        self.__routes[route] = page_function

    @property
    def home(self):
        return self.__home

    @property
    def routes(self) -> dict:
        return self.__routes
    
    @property
    def size(self) -> tuple:
        return self.__size
    
    @property
    def height(self) -> int:
        return self.__size[0]
    
    @property
    def width(self) -> int:
        return self.__size[1]
    
    @property
    def font_family(self) -> str:
        return self.__default_font_family
