require 'ipp4r'

img = Ipp::Image.jaehne(1000, 1000)
red = Ipp::Color.new(1, 0, 0)
(0...[img.height, img.width].min).each do |i|
	img[i, i] = img[img.height - i - 1, i] = red
end
img.save("1_1.jpg")
img.subimage(100,100,-100,-100).fill!(red)
img.save("1_2.jpg")


