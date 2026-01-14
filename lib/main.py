from soft import Soft
import pages.hello as hello

def main():
    soft = Soft(hello.hello, "Hello Soft")

    soft.register("hello", hello.hello)

    print(soft.title)

    return soft