import sys
sys.path.append('./build')

from ImageTools.Image import *
from ImageTools.Size import *
from flatbuffers.builder import Builder

from random import randint

"""
image = ImageT()
image.raw = []
for i in range(1000):
    image.raw.append(randint(0, 255))
image.size = SizeT()
image.size.width = 1920
image.size.height = 1080
image.size.depth = 3
image_msg = ImageT.InitFromObj(image)
print(image_msg)
"""

image = ImageT()
image.raw = []
for i in range(100):
    image.raw.append(randint(0, 255))
image.size = SizeT()
image.size.width = 1920
image.size.height = 1080
image.size.depth = 3
print("image.raw", image.raw)
print(image)

builder = Builder(0)
print("builder.Finish", builder.Finish(image.Pack(builder)))

buf_new = builder.Output()

print(len(buf_new), buf_new)
print("CORUPT")
for i in range(30, 40):
    buf_new[i] = randint(0, 255)
print(len(buf_new), buf_new)

image_new = Image.GetRootAsImage(buf_new, 0)
print(image_new)
raw = image_new.RawAsNumpy()
print("raw", raw)
size = image_new.Size()
print("size", size)
print(size.Width())
print(size.Height())
print(size.Depth())

print(len(buf_new), buf_new)
print("raw", raw)
print("CHANGE")
with open("buffer.bin", "wb") as f:
    f.write(buf_new + b"\0" * (4096 - len(buf_new)))
raw = image_new.RawAsNumpy()
for i in range(100):
    raw[i] = 13
print(len(buf_new), buf_new)
print("raw", raw)