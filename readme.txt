An openframeworks addon of video player using libvlc. 

Written by NAKANO, Jun'ichi/Buffer Renaiss(junitium@nakano.gr.jp)
How to use:

1. Get VLC source and binary. (I used VLC 2.0.5 for development, probably 2.0.x or 2.1.x is OK)
   http://www.videolan.org/vlc/#download

2. Set include paths as following:
  a. libvlc SDK include dir (\Program Files (x86)\VideoLAN\VLC\sdk\include)
  b. VLC source include dir (VLC_SOURCE_PATH\vlc\include)
  c. VLC source lib dir (VLC_SOURCE_PATH\vlc\lib)

3. Link libvlc.lib, libvlccore.lib.

4. Copy VLC plugins folder(\Program Files (x86)\VideoLAN\VLC\plugins) to bin dir.

5. If you are developing with Visual Studio, change project's linker optimization setting /OPT:REF to /OPT:NOREF(On default, it is set on Release build).

2013/4/23
