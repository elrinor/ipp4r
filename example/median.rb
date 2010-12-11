require 'ipp4r'

img = Ipp::Image.jaehne(1000, 1000)
img.add_rand_uniform!(0, 0.2).save("random.jpg")
img.filter_median(Ipp::Size.new(15,15)).save("median.jpg")



