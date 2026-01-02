class FormatStyle:
    __format_style: dict = {}

    def __init__(self, **kwargs) -> None:
        self.__format_style = kwargs

    def set_style(self, **kwargs) -> None:
        for key, value in kwargs.items():
            self.__format_style[key] = value

    @property
    def style(self) -> dict:
        return self.__format_style
