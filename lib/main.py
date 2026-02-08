from soft import Soft
import pages.hello as hello

def main():
    soft = Soft(hello.hello)

    soft.register("hello", hello.hello)

    return soft