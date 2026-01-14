class Style:
    __style: dict = {}

    def __init__(self, **kwargs) -> None:
        self.__style = kwargs

    def set_style(self, **kwargs) -> None:
        for key, value in kwargs.items():
            self.__style[key] = value

    @property
    def style(self) -> dict:
        return self.__style