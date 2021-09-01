# simon
Simon says.  
A simon game done to my kid. let's see if he uses it.

![Simon](https://github.com/RuiVarela/simon/raw/master/demo.gif)

## Building
This was coded on a mac, but it should be very portable.   
The only dependency needed is [raylib](https://www.raylib.com/). 
Build raylib and place the `raylib.h` and `libraylib.a` under `vendor/raylib/{env}` and update the makefile accordingly.   

```
make
./simon
```
And that is it..

## Building for web   

use a specific version of emsdk
```
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh
```

```
make WEB=1
python wasm-server.py
```
open chrome on http://localhost:8080/simon.html


