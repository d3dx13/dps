import capnp

capnp.remove_import_hook()
image_capnp = capnp.load('image.capnp')

print(dir(image_capnp.Image))
image = image_capnp.Image.new_message(raw=[1, 2, 3, 4, 5, 6, 7, 8, 9, 10])
image.size.width = 1333
image.size.height = 1231
image.size.depth = 2
print(image)
print(image.size)
print(image.raw)
print(dir(image))
print(image.to_bytes())

buf = bytearray(image.to_bytes())

print(len(buf), buf)
image_new = image_capnp.Image.from_bytes(buf, builder=True)
print(dir(image_new))
print(image_new)
image.size.depth = 22
print(image_new)