from soft.elements import Element

class Soft:
    title: str = ""
    __home: None | Element = None
    __routes: dict = {}

    def __init__(self, home, title: str = ""):
        self.__home = home
        self.title = title

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
