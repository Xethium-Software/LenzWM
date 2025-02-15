How to build and run?
```
gcc main.c -lX11 -o lynWM
Xephyr :1 -ac -br -noreset -screen 800x600 &
DISPLAY=:1 ./lynWM
```