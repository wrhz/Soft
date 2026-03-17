from soft.elements import Text,View
from soft.style import Style
import os,example_module
def hello():A='center';B=Style(display='flex',flex_direction='column',align_items=A,justify_content=A);print(os.path.dirname(os.path.abspath(__file__)));print(example_module.hello());return View(Text('Hello Soft!'),style=B)