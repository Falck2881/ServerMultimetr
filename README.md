# :thought_balloon: ServerMultimetr :thought_balloon:

- Данное приложение явлется частью реализации `application client-server`, в нашем случаи мы `server`.
- Где тогда `client` он тут - https://github.com/Falck2881/Multimetr

## Tools
  
  - C++17 и выше
  - Cmake v.16 и выше

## Build
```
$ mkdir buildServer
$ git clone https://github.com/Falck2881/ServerMultimetr.git
$ cmake -S ServerMultimetr/ -B buildServer/ -DCMAKE_BUILD_TYPE:STRING=Release
$ cmake --build buildServer/ --target all
$ cd buildServer
$ ./ServerMultimetr

```


