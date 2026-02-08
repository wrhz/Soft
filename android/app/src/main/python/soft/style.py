class Style:
	__style:dict={}
	def __init__(A,**B):A.__style=B
	def set_style(A,**B):
		for(C,D)in B.items():A.__style[C]=D
	@property
	def style(self):return self.__style