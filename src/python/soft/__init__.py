class Soft:
    __home = None
    __routes: dict = {}
    __default_font_family: str = "Arial"

    def __init__(self, home, font_family: str = "Arial"):
        self.__home = home
        self.__default_font_family = font_family

    def register(self, route, page_function):
        self.__routes[route] = page_function

    @property
    def home(self):
        return self.__home

    @property
    def routes(self) -> dict:
        return self.__routes
    
    @property
    def default_font_family(self) -> str:
        return self.__default_font_family
