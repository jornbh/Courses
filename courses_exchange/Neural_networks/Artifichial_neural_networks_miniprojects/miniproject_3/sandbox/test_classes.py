

class base:
    def __init__(self):
        pass
    def foo(self):
        print("start")
        print("Bar retruned", self.bar())
    def bar(self):
        return "base bar"

class child(base):
    def bar(self):
        return "Child"


B = base()
C = child()

B.foo()
C.foo()