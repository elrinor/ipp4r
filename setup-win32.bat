@echo off
echo 
echo ================================================================================
echo                             Running ruby extconf.rb...
echo ================================================================================
ruby src/extconf.rb %*
if errorlevel 1 goto end

echo 
echo ================================================================================
echo                                 Running nmake...
echo ================================================================================
nmake clean
nmake
if errorlevel 1 goto end

echo 
echo ================================================================================
echo                              Running manifest tool...
echo ================================================================================
mt -manifest ipp4r.so.manifest -outputresource:ipp4r.so;2

:end