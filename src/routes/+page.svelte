<script lang="ts">
  import AlarmModal from "$lib/components/AlarmModal.svelte";
  import ConfirmationModal from "$lib/components/ConfirmationModal.svelte";
  import DeviceMap from "$lib/components/DeviceMap.svelte";
  import DevicePane from "$lib/components/DevicePane.svelte";
  import PairingModal from "$lib/components/pairing/PairingModal.svelte";
  import type { Device } from "$lib/types/device";
  import type { DeviceLocation } from "$lib/types/location";
  import { startScan, stopScan, type BleDevice } from "@mnlphlp/plugin-blec";
  import {
    checkPermissions,
    getCurrentPosition,
    requestPermissions,
  } from "@tauri-apps/plugin-geolocation";
  import { onMount } from "svelte";

  let devices = $state([]) as Device[];
  let locations = $state([]) as DeviceLocation[];

  // pairing modal
  let showpairingmodal = $state(false);

  async function onpair() {
    await stopService();
    showpairingmodal = true;
  }

  function pairingcomplete(device: Device | undefined) {
    showpairingmodal = false;
    if (device) {
      devices.push(device);
    }
    startService();
  }

  // device pane
  let selectedDevice = $state() as Device | undefined;
  let showdeletemodal = $state(false);
  let todelete: Device;

  function ondelete(device: Device) {
    todelete = device;
    showdeletemodal = true;
  }

  function onselect(device: Device) {
    if (selectedDevice === device) {
      selectedDevice = undefined;
    } else {
      selectedDevice = device;
    }
  }

  function deletecomplete(confirmed: boolean) {
    if (confirmed) {
      devices.splice(devices.indexOf(todelete), 1);
    }
    showdeletemodal = false;
  }

  // alarm modal
  let showalarmmodal = $state(false);

  function onalarm() {
    stopService();
    showalarmmodal = true;
  }

  function alarmcomplete() {
    showalarmmodal = false;
    startService();
  }

  // passive scanning
  let isScanning = false;
  let intervalId: number;

  onMount(() => {
    startService();
  });

  function startService() {
    if (isScanning) return;

    intervalId = setInterval(() => {
      let matching: Device[] = [];
      startScan((bles) => {
        matching = devices.filter((d) =>
          bles.map((d) => d.address).includes(d.macAddress)
        );
      }, 3 * 1000);

      setTimeout(async () => {
        let permissions = await checkPermissions();
        if (
          permissions.location === "prompt" ||
          permissions.location === "prompt-with-rationale"
        ) {
          permissions = await requestPermissions(["location"]);
        }

        if (permissions.location === "granted") {
          const pos = await getCurrentPosition();
          const now = Date.now();

          for (let device of devices) {
            locations.push({
              device,
              latitude: pos.coords.latitude,
              longitude: pos.coords.longitude,
              time: now,
            });
          }
        }
      }, 3 * 1000);
    }, 10 * 1000);
  }

  async function stopService() {
    if (!isScanning) return;
    clearInterval(intervalId);
    await stopScan();
  }
</script>

<div class="h-screen w-screen">
  <DeviceMap locations={locations.filter((l) => l.device === selectedDevice)} />
  <!-- <DeviceMap locations={[{longitude: 1, latitude: 1}, {longitude: 2, latitude: 2}]} /> -->
  <DevicePane {onpair} {ondelete} {onselect} {onalarm} {selectedDevice} {devices} />
  {#if showpairingmodal}
    <PairingModal
      ignored={new Set(devices.map((d) => d.macAddress))}
      oncomplete={pairingcomplete}
    />
  {/if}
  {#if showdeletemodal}
    <ConfirmationModal oncomplete={deletecomplete} />
  {/if}
  {#if showalarmmodal}
    <AlarmModal device={selectedDevice as Device} oncomplete={alarmcomplete}/>
  {/if}
</div>
