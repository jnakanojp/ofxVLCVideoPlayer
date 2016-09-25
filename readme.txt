An openframeworks addon of video player using libvlc. 

Written by NAKANO, Jun'ichi/Buffer Renaiss(junitium@nakano.gr.jp)
How to use:

1. run setup.bat(windows) or setup.sh(Mac) (will download VLC2.2.4 Binary and Source)

2. If you are working on windows, Link libs/vlcSdk/lib/vs/libvlc.lib, libs/vlcSdk/lib/vs/libvlccore.lib.

3. Copy files of copy_to_bin dir to bin dir.

4. If you are developing with Visual Studio, add additional library directory “change project's linker optimization setting /OPT:REF to /OPT:NOREF(On default, it is set on Release build) and linker advanced setting /SAFESH:NO.

2016/9/24
