# Vue in ESP8266 

## How to run it?

### Install env.

You need to install the following software.
- VSCode
- Node.js(20)
- Pnpm

And also need to install the following VSCode extensions.
- PlatformIO IDE
- C/C++ Extension Pack
- TypeScript And JavaScript Nightly
- TypeScript Vue Plugin (Volar)

### Copy exmaple file.
Copy `envSetup.cpp.example` file and rename to `envSetup.cpp` in src directory.
You need modify `wifi_ssid` and `wifi_password` value.

### Install dependencies and build Web files.
```
cd web
pnpm i
pnpm build

# dev mode
pnpm dev
```

### Upload to board.
1. In your VSCode sidebar, click PlatformIO icon. Click `Project Tasks` -> `nodemcuv2` -> `General` -> `Upload`.
2. Click `Project Tasks` -> `nodemcuv2` -> `Platform` -> `Build Filesystem Image`.
3. Click `Project Tasks` -> `nodemcuv2` -> `Platform` -> `Upload Filesystem Image`.

### Complete.
Now, check your serial monitor. Board ip address will print in serial monitor.
Open your browser and enter the IP.

### Vue development and debugging.
Modify the `.env.dev` file in the `web` path.
Change the IP in VITE_API_URL to your IP.
```
pnpm dev
```
Open your browser and enter `localhost:5173`.
