require 'ipp4r'

Ipp::MetaType.values.reverse.each do |t0|
	img = Ipp::Image.jaehne(1000, 1000, t0)
	Ipp::MetaType.values.reverse.each do |t1|
		img.convert(t1).save("j" + t0.inspect + "___" + t1.inspect + ".jpg" )
	end
end
