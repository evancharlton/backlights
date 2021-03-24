# Aurora firmware

This is a simple firmware for the MCU. This is built for specific hardware, but
can probably be generalized. The hardware in use right now is:

- ESP8266 MCU
- One strip of 18 LEDs (9 per monitor) in the BRG configuration.

## Building

This is built using [PlatformIO]. Import it into VS Code and build/upload from
there.

Note that you'll need to create `src/config.h` first. It needs to have the
following structure:

```cpp
#define WIFI_SSID "Your Wi-Fi network here"
#define WIFI_PASSWORD "Your Wi-Fi network password"

#define FIREBASE_HOST "Your Firebase RTDB hostname (fully-qualified)"
#define FIREBASE_AUTH "Your Firebase legacy auth token"
```
