require 'mkmf'

# this file is just an example and needs tuning...

dir_config("ipp4r");

find_header("cxcore.h",  "C:/Program Files/OpenCV/cxcore/include");
find_header("highgui.h", "C:/Program Files/OpenCV/otherlibs/highgui");
find_header("ipp.h",     "C:/Program Files/Intel/IPP/5.3.2.073/ia32/include");

create_makefile("ipp4r");
