require 'mkmf'

# this file is just an example and needs tuning...

dir_config("ipp4r");

if !RUBY_PLATFORM.include?('win32')
  find_library("ippi-5.3",    "ippiCopy_8u_C3R",        ".")
  find_library("ippcore-5.3", "ippGetStatusString",     ".")
  find_library("ippcc-5.3",   "ippiRGBToGray_8u_C3C1R", ".")
end

find_header("ipp.h",     "C:/Program Files/Intel/IPP/5.3.2.073/ia32/include");
find_header("cxcore.h",  "C:/Program Files/OpenCV/cxcore/include");
find_header("highgui.h", "C:/Program Files/OpenCV/otherlibs/highgui");

create_makefile("ipp4r");

