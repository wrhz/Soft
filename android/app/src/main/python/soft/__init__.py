from soft.elements import Element
class Soft:
	__size=800,600;__home:None|Element=None;__routes:dict={};__default_font_family:str='Arial'
	def __init__(A,home,size=(800,600),font_family='Arial'):A.__home=home;A.__size=size;A.__default_font_family=font_family
	def set_home(A,home):A.__home=home
	def register(A,route,page_function):A.__routes[route]=page_function
	@property
	def home(self):return self.__home
	@property
	def routes(self):return self.__routes
	@property
	def size(self):return self.__size
	@property
	def height(self):return self.__size[0]
	@property
	def width(self):return self.__size[1]
	@property
	def font_family(self):return self.__default_font_family