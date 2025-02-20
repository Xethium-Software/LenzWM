How to build and run?
```
gcc main.c -lX11 -o lenzWM
Xephyr :1 -ac -br -noreset -screen 1920x1080 &
DISPLAY=:1 ./lenzWM
```