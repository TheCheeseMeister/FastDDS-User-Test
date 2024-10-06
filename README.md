Hi. :> Should be fine.

Need OpenSSL installed.

Create Build folder inside root folder and run "cmake .." (it'll give error :/)
Inside the Build folder go to "CMakeCache.txt". Scroll down to "OPENSSL_INCLUDE_DIR:PATH=(some error)". Get <path\to\OpenSSL>\include and add that after the =.

Now run "cmake ..", then "cmake --build ."

There should be a Debug folder in Build now that has the outputted exe "DDSHelloWorldUser".

I see the value in Docker now. :(
