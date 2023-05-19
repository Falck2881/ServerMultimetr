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
## Result

![image](https://github.com/Falck2881/ServerMultimetr/assets/100667839/9730fa40-5463-4cdc-80ff-df7720171e56)

## How this work 

Взаимодействие между  `client` и `server` выполняется на одном компьюторе, используя при этом сокет UNIX-домен (`AF_UNIX`). 
Клиент отправляет запрос, сервер считывает его и отправляет ответ клиенту. В терминале приложения `server` отоброжаються сообщения чтобы человек 
смог идентифицировать следующее:

- ошибки в случае неудачи отправки ответа клиенту 
- успешное принятие запроса от клиента 
- успешное подключение/отключение клиента
- завершение сеанса сервера и отключение всех сессий клиентов
 
## Example work a server 

![image](https://github.com/Falck2881/ServerMultimetr/assets/100667839/2590069c-6319-4b1f-b37f-db1dbf5a9ed6)

