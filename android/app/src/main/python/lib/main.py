from soft import Soft
import pages.hello as hello
def main():A=Soft(hello.hello);A.register('hello',hello.hello);return A