import type { Device } from "./device";

export interface DeviceLocation {
    longitude: number,
    latitude: number,
    time: number,
    device: Device
}