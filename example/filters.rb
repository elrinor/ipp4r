require 'ipp4r'
require 'matrix'

img = Ipp::Image.jaehne(1000, 1000)
img.filter(Matrix[[0, -1, 0], [-1, 4, -1], [0, -1, 0]]).save("filtered1.jpg")
img.filter(Matrix[[0, 0.125, 0], [0.125, 0.5, 0.125], [0, 0.125, 0]]).save("filtered2.jpg")
img.subimage(100, 100, -100, -100).dilate!(Matrix[[0, 0, 0, 0, 0, 0, 0],[1, 1, 1, 1, 1, 1, 1],[0, 0, 0, 0, 0, 0, 0]]);
img.subimage(200, 200, -200, -200).erode!(Matrix[[0, 0, 0, 0, 0, 0, 0],[1, 1, 1, 1, 1, 1, 1],[0, 0, 0, 0, 0, 0, 0]]);
img.save("filtered3.jpg")
