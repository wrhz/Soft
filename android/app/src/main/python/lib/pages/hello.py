from soft.elements import Text,View
from soft.style import Style
def hello():B='Hello Soft!';A='center';C=Style(display='flex',flex_direction='column',align_items=A,justify_content=A);return View(Text(B),Text(B),style=C)