Write-Host 'test'

mkdir ofxVLCVideoPlayerTmp
cd ofxVLCVideoPlayerTmp

mkdir archives
cd archives
[System.IO.Directory]::SetCurrentDirectory((Get-Location -PSProvider FileSystem).Path)

$wc = (new-object System.Net.WebClient)

echo "Downloading VLC 2.2.4 Source..."
$wc.DownloadFile("http://download.videolan.org/pub/videolan/vlc/2.2.4/vlc-2.2.4.tar.xz", "vlc-2.2.4-src.tar.xz")

echo "Downloading VLC 2.2.4 Win32..."
$wc.DownloadFile("http://download.videolan.org/pub/videolan/vlc/2.2.4/win32/vlc-2.2.4-win32.7z", "vlc-2.2.4-win32.7z")

cd ..
[System.IO.Directory]::SetCurrentDirectory((Get-Location -PSProvider FileSystem).Path)

mkdir src
& 'C:\Program Files\7-Zip\7z.exe' e -y -oarchives archives\vlc-2.2.4-src.tar.xz 
& 'C:\Program Files\7-Zip\7z.exe' x -y -osrc archives\vlc-2.2.4-src.tar

mkdir win32
& 'C:\Program Files\7-Zip\7z.exe' x -y -owin32 archives\vlc-2.2.4-win32.7z

mkdir ..\libs\vlcSrcSrc\include
Copy-Item src\vlc-2.2.4\src\* -recurse -destination ..\libs\vlcSrcSrc\include
Remove-Item ..\libs\vlcSrcSrc\include\* -recurse -exclude *.h

mkdir ..\libs\vlcSrcLib\include
Copy-Item src\vlc-2.2.4\lib\* -recurse -destination ..\libs\vlcSrcLib\include
Remove-Item ..\libs\vlcSrcLib\include\* -recurse -exclude *.h

mkdir ..\libs\vlcSrcInclude\include
Copy-Item src\vlc-2.2.4\include\* -recurse -destination ..\libs\vlcSrcInclude\include
Remove-Item ..\libs\vlcSrcInclude\include\* -recurse -exclude *.h

mkdir ..\libs\vlcSdk\include
Copy-Item win32\vlc-2.2.4\sdk\include\vlc\* -recurse -destination ..\libs\vlcSdk\include
Remove-Item ..\libs\vlcSdk\include\* -recurse -exclude *.h

mkdir ..\libs\vlcSdk\lib\vs
Copy-Item win32\vlc-2.2.4\sdk\lib\libvlc*.lib -destination ..\libs\vlcSdk\lib\vs

mkdir ..\copy_to_bin
Copy-Item win32\vlc-2.2.4\libvlc*.dll -destination ..\copy_to_bin
Copy-Item win32\vlc-2.2.4\plugins -recurse -destination ..\copy_to_bin