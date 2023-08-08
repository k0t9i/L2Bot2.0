# Yet another ingame bot for Lineage 2.
![image_2023-08-09_00-43-10](https://github.com/k0t9i/L2Bot2.0/assets/7733997/59c57585-720a-4588-aa94-ee3d6d78a39f)

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
