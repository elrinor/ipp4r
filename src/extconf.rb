# WARNING! this file is just an example and needs tuning...
require 'optparse'

# Parse arguments first
options = {}
options[:include_dirs] = []
options[:lib_dirs] = []
options[:opencv] = false

begin
	OptionParser.new do |opts|
		opts.banner = "Usage: extconf.rb [options]"
		
		opts.separator ""
		
		opts.on("-v", "--ipp-version VERSION", "use VERSION suffix for IPP libraries") do |v|
			options[:ipp_version] = v
		end
		
		opts.on("-I", "--include-dir DIR", "look for include files in DIR") do |v|
			options[:include_dirs] << v
		end
		
		opts.on("-L", "--lib-dir DIR", "look for library files in DIR") do |v|
			options[:lib_dirs] << v
		end
		
		opts.on("--[no-]opencv", "Use OpenCV") do |v|
			options[:opencv] = v
		end
		
        opts.on_tail("-h", "--help", "Show this message") do
			puts opts
			exit 1
        end
	end.parse!
rescue SystemExit
	raise
rescue Exception => e
	puts "Error: #{e} (#{e.class})"
	exit 1
end


# Then mess with mkmf
require 'mkmf'

# Default libpathflag is buggy (at least for me)
def libpathflag(libpath=$DEFLIBPATH|$LIBPATH)
  libpath.map{|x|
    case x
    when "$(topdir)", /\A\./
      LIBPATHFLAG
    else
      LIBPATHFLAG+RPATHFLAG
    end % x
  }.join
end

dir_config("ipp4r");

$LIBPATH = $LIBPATH | options[:lib_dirs]
$include_dirs = options[:include_dirs]

unless find_header("ipp.h", *$include_dirs);
	puts "Error: IPP header file not found, please specify header search dirs using -I switch (use --help for list of available options)"
	exit 1
end

$ipp_lib_suffix = options.has_key?(:ipp_version) ? "-" + options[:ipp_version] : ""
unless have_library("ippi"    + $ipp_lib_suffix, "ippiCopy_8u_C3R",        "ipp.h") and
       have_library("ippcore" + $ipp_lib_suffix, "ippGetStatusString",     "ipp.h") and
       have_library("ippcc"   + $ipp_lib_suffix, "ippiRGBToGray_8u_C3C1R", "ipp.h")
	puts "Error: one of IPP libraries not found, please specify library search dirs using -L switch (use --help for list of available options)"
	exit 1
end

if options[:opencv]
	$CFLAGS << " -DUSE_OPENCV"
	unless find_header("cxcore.h", *$include_dirs) and
           find_header("highgui.h", *$include_dirs)
		puts "Error: one of OpenCV headers not found, please specify header search dirs using -I switch (use --help for list of available options)"
		exit 1
	end

	unless have_library("cxcore",  "cvCreateImage", "highgui.h") and 
	       have_library("highgui", "cvLoadImage",   "highgui.h")
		puts "Error: one of OpenCV libraries not found, please specify library search dirs using -L switch (use --help for list of available options)"
		exit 1
	end
end

create_makefile("ipp4r");
