_B='text'
_A='tag'
from soft.style import Style
class Element:
	__element:dict={}
	def __init__(A,tag,text='',children=[],style=Style()):A.__element={_A:tag,_B:text,'children':children,'style':style.style}
	@property
	def element(self):return self.__element
class View(Element):
	def __init__(B,*A,style=Style()):super().__init__('view',children=[A.element for A in A],style=style)
class Text(Element):
	def __init__(C,text='',children=[],style=Style()):
		A=children
		for B in A:
			if B.element[_A]in[_B]:raise ValueError(f"Text element can only have {B.element[_A]} children")
		super().__init__(_B,text=text,children=A,style=style)