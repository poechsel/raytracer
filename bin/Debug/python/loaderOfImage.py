from PIL import Image
im = Image.open("../texture.jpg")
print (list(im.getdata()))
