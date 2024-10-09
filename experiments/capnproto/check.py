import capnp

capnp.remove_import_hook()
image_capnp = capnp.load('image.capnp')

print(dir(image_capnp.Image))
# print(help(image_capnp.Image.new_message))
# num_first_segment_words=0,
image = image_capnp.Image.new_message(num_first_segment_words=0, raw=[1, 2, 3, 4, 5, 6, 7, 8, 9, 10])
image.size.width = 1333
image.size.height = 1231
image.size.depth = 2
image.raw[1] = 255
image.raw[6] = 255
print(image)
print(image.size)
print(image.raw)
print(dir(image))
for i in range(len(image.to_bytes()) // 8):
    print(image.to_bytes()[i * 8:i * 8 + 8])

buf = bytearray(image.to_bytes())

print(len(buf), buf)
image_new = image_capnp.Image.from_bytes(buf)
print(dir(image_new))
print(image_new)
image.size.depth = 22
print(image_new)