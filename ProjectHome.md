Ipp4r is a Ruby extension that wraps some of the image processing routines from Intel Integrated Performance Primitives library. I wrote it while working at the Intel Research Lab at the faculty of Computational Mathematics and Cybernetics of the Moscow State University.

Ipp4r is written in C and extensively uses preprocessor metaprogramming to automate the generation of boilerplate code. To simplify some of the tasks I wrote a small library for preprocessor metaprogramming similar to Boost.Preprocessor. Note that Ipp4r was last built against Ruby 1.8.6 and it likely won't compile against the latest Ruby sources.

Main features of Ipp4r are:

  * Abstraction. Ipp4r offers a high-level Ruby interface to low-level IPP functions.
  * Extensibility. Ipp4r is easily extensible, so that new functions can be added without understanding all the trickery of Ruby implementation.
  * Performance. Ipp4r is backed by the Intel Integrated Performance Primitives library, so it significantly outperforms all of the available Ruby image processing libraries (at least that was true at the moment when these lines were written).

Example Ruby code:
```
require 'ipp4r'
 
img = Ipp::Image.jaehne(1000, 1000)
img.add_rand_uniform!(0, 0.2).save("random.jpg")
img.filter_median(Ipp::Size.new(15,15)).save("median.jpg")
```

In the third line, `jaehne` sample image of size 1000x1000 is created and stored in the `img` variable. In the 4th line a linearly distributed noise is added to `img` and result is saved to `"random.jpg"`. In the 5th line a median filter of size 15x15 is applied to the noisy image and the result is saved to `"median.jpg"`.