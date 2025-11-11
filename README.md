
# DIY BLE (Bluetooth Low Energy) AirTag using Tauri + SvelteKit + C++/Arduino 
Epic Fall '25 HackUMass Hackathon Project!

## Inspiration
I got too sendy over Halloweekend and I lost everything! My keys, my student ID....
I looked into buying an AirTag or a Tile, but I didn't like the idea of a vast network of devices eavesdropping and uploading my location to the cloud. Instead, our team is creating an alternative DIY tracker using Bluetooth Low Energy that you can have total control over.
## What it does
We created both a Bluetooth tracker and a cross-platform companion app for iOS and Android. The tracker periodically broadcasts an advertisement that the app listens for in the background, recording the time, location, and the signal strength of the broadcast. The app displays this information in a sleek and modern interface and allows users to connect to the device to play a noise on the tracker when within range. 
## How we built it
This project split into two parts: the mobile app and the tracker. The tracker itself is powered by an ESP32, advertising two BLE characteristics. One is periodically updated by the tracker to notify all listeners that it still exists, and the other is writable by any central device that signals to activate the buzzer.
The app is built using Tauri, a cross-platform app development framework powered by Rust that's based on the system webview. A web front-end communicates with a core thread running in Rust that communicates with a Kotlin/Swift back-end, depending on if it's compiled for Android or iOS.
## Challenges we ran into
We're a very ambitious duo team of freshmen tackling an intensive project, and we ran into issues and had to continually learn new concepts about hardware, Bluetooth, web development, and mobile app development.
## Accomplishments that we're proud of
We have a working prototype! The UI renders a map and a slideable panel to display nearby Bluetooth devices and track through location and send a ring! 
Using circuit analysis, we connected a piezo buzzer to the microcontroller and programmed it to play any song we'd like in an 8-bit retro styled theme!
## What we learned
Quite literally— EVERYTHING! Learning the BLE protocol, how to connect an ESP32 to a breadboard, how to turn accessories on and off using C++, wiring hardware on breadboards, how to create a mobile application using Svelte and Tailwind
## What's next for BLEh
How to develop a Tauri plugin to spin off background services using Rust, Swift, and Kotlin... 
