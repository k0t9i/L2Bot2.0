# Yet another ingame bot for Lineage 2.
![image_2023-08-09_00-43-10](https://github.com/k0t9i/L2Bot2.0/assets/7733997/8b4356b0-f362-4ba8-8ca2-8c5cb949d873)

## Project components
#### L2BotCore (C++)
[The bot framework](L2BotCore). It realization can be an ingame bot, a radar etc.
#### L2BotDll (C++)
[Injectable Dll](L2BotDll) with framework realization for Lineage 2 Interlude.
#### InjectionLibrary (C++)
[Helper library](InjectionLibrary) for code injection.
#### Client (C#)
[Unfinished client](Client).

## Intercommunications
Communication between the client and the injected code occurs through a named pipe with json-messages.

## Pathfinding
Pathfinding is done using [L2jGeodataPathFinder](https://github.com/k0t9i/L2jGeodataPathFinder).

![image_2023-10-29_20-53-56](https://github.com/k0t9i/L2Bot2.0/assets/7733997/104e5ff2-7435-4def-be5c-3223f02e37c5)
