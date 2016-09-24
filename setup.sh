mkdir ofxVLCVideoPlayerTmp
cd ofxVLCVideoPlayerTmp

mkdir archives
cd archives

echo "Downloading VLC 2.2.4 Source..."
wget http://download.videolan.org/pub/videolan/vlc/2.2.4/vlc-2.2.4.tar.xz

echo "Downloading VLC 2.2.4 Mac OS X..."
wget http://download.videolan.org/pub/videolan/vlc/2.2.4/macosx/vlc-2.2.4.dmg

cd ..

mkdir src
pwd
tar Jxvf archives/vlc-2.2.4.tar.xz -C src

mkdir macosx
hdiutil mount archives/vlc-2.2.4.dmg

mkdir -p ../libs/vlcSrcSrc/include
#Copy-Item src\vlc-2.2.4\src\* -recurse -destination ..\libs\vlcSrcSrc\include
cp -a src/vlc-2.2.4/src ../libs/vlcSrcSrc/include
#Remove-Item ..\libs\vlcSrcSrc\include\* -recurse -exclude *.h
find ../libs/vlcSrcSrc/include -type f \! -name "*.h" -exec rm {} \;

mkdir -p ../libs/vlcSrcLib/include
#Copy-Item src\vlc-2.2.4\lib\* -recurse -destination ..\libs\vlcSrcLib\include
cp -a src/vlc-2.2.4/lib ../libs/vlcSrcLib/include
#Remove-Item ..\libs\vlcSrcLib\include\* -recurse -exclude *.h
find ../libs/vlcSrcLib/include -type f \! -name "*.h" -exec rm {} \;

mkdir -p ../libs/vlcSrcInclude/include
#Copy-Item src\vlc-2.2.4\include\* -recurse -destination ..\libs\vlcSrcInclude\include
cp -a src/vlc-2.2.4/include ../libs/vlcSrcInclude/include
#Remove-Item ..\libs\vlcSrcInclude\include\* -recurse -exclude *.h
find ../libs/vlcSrcInclude/include -type f \! -name "*.h" -exec rm {} \;
pushd ../libs/vlcSrcInclude
patch -p1 < ../../macosx_atomic.patch
popd

mkdir -p ../libs/vlcSdk/include
#Copy-Item win32\vlc-2.2.4\sdk\include\vlc\* -recurse -destination ..\libs\vlcSdk\include
cp -a /Volumes/vlc-2.2.4/VLC.app/Contents/MacOS/include/vlc ../libs/vlcSdk/include

#Remove-Item ..\libs\vlcSdk\include\* -recurse -exclude *.h
find ../libs/vlcSdk/include -type f \! -name "*.h" -exec rm {} \;

mkdir -p ../libs/vlcSdk/lib/osx
#Copy-Item win32\vlc-2.2.4\sdk\lib\libvlc*.lib -destination ..\libs\vlcSdk\lib\vs
cp -a /Volumes/vlc-2.2.4/VLC.app/Contents/MacOS/lib/* ../libs/vlcSdk/lib/osx

mkdir -p ../copy_to_bin/vlc
#Copy-Item win32\vlc-2.2.4\libvlc*.dll -destination ..\copy_to_bin
cp -a /Volumes/vlc-2.2.4/VLC.app/Contents/MacOS/lib/* ../copy_to_bin
#Copy-Item win32\vlc-2.2.4\plugins -recurse -destination ..\copy_to_bin
cp -a /Volumes/vlc-2.2.4/VLC.app/Contents/MacOS/plugins ../copy_to_bin/vlc
cp -a /Volumes/vlc-2.2.4/VLC.app/Contents/MacOS/share ../copy_to_bin/vlc

hdiutil unmount /Volumes/vlc-2.2.4