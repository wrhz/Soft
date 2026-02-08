from soft.style import Style
class Element:
	__element:dict={}
	def __init__(A,tag,text='',children={},style=Style(),**B):A.__element={'tag':tag,'text':text,'children':children,'style':style.style,**B}
	@property
	def element(self):return self.__element
class Text(Element):
	def __init__(A,text='',children={},style=Style()):super().__init__('text',text=text,children=children,style=style)