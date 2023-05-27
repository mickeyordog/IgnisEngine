import emb

print("Number of arguments", emb.numargs())
print("5 * 11 =", emb.multiply(5, 11))
v1 = (4.0, 3.0)
v2 = (2.0, 1.0)
print("Dot product between", v1, v2, emb.dot(v1, v2))


def test(num: float):
    print("Test function: " + str(num))
    return num * num

def blah(string: str):
    print("Blah function: " + string)
    return string

# TODO: get subclasses recursively
def get_component_classes():
    return Component.__subclasses__()

def get_class_fields(cls: type):
    return [attribute for attribute in dir(cls) if callable(getattr(cls, attribute)) is False]

# basically need to give C a list of of component class names -> list of component fields (name, val, and ref for changing val)
# then C can tell python to change a value and then request the entire updated list again
# C should only do this if a field updates
# perf doesn't really matter bc this should only be happening in editor
# upon cmd+S, save entire dict to file
# need to handle possibly changing component class names and fields
# for speed, does cpython auto compile to something good? OR should I use Cython? Also check how mem alloc works in python
def get_component_dict():
    return {cls.__name__: get_class_fields(cls) for cls in get_component_classes()}

class Component():
    def start(self):
        pass

    def update(self):
        pass

class Collider(Component):
    ...

class Transform(Component):
    x = 0.0
    y = 0.0
    z = 0.0

    def __init__(self):
        pass

    def move(self, pos: tuple[float, float, float]):
        self.x += pos[0]
        self.y += pos[1]
        self.z += pos[2]
        print("New pos is: " + str((self.x, self.y, self.z)))

    
print(Component.__subclasses__())
print([attribute for attribute in dir(Transform) if callable(getattr(Transform, attribute)) is False and attribute.startswith('__') is False])
t = Transform()
setattr(Transform, "x", 10.0)
getattr(Transform, "move")(t, (1.0, 2.0, 3.0))
print(type(Component.__subclasses__()[0]))

def cool_function(a:int):
    print("hi mickey")
    return a+2