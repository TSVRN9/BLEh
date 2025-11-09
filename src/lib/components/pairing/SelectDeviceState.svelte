<script lang="ts">
  import { startScan, stopScan, type BleDevice } from "@mnlphlp/plugin-blec";
  import { onMount, onDestroy } from "svelte";
  import AnimatedElipses from "../AnimatedElipses.svelte";
  import RefreshIcon from "../icons/RefreshIcon.svelte";

  interface SelectDeviceStateProps {
    ignored: Set<string>;
    oncomplete: (device: BleDevice) => void;
  }

  let { ignored, oncomplete }: SelectDeviceStateProps = $props();

  let isScanning = $state(false);
  let devices = $state([]) as BleDevice[];

  let timeoutId: number;

  onMount(() => {
    startSearchDevices();
  });

  onDestroy(() => {
    stopSearchDevices();
  });

  function startSearchDevices() {
    if (isScanning) return;

    const timeToScan = 5 * 1000;

    isScanning = true;

    startScan(
      (foundDevices) => {
        devices = foundDevices.filter(
          (d) => d.name != "" && !ignored.has(d.address)
        );
      },
      timeToScan,
      false
    );

    timeoutId = setTimeout(() => {
      stopSearchDevices();
    }, timeToScan);
  }

  function stopSearchDevices() {
    if (!isScanning) return;

    isScanning = false;
    stopScan();
    clearTimeout(timeoutId);
  }

  function toggleSearch() {
    if (isScanning) {
        stopSearchDevices();
    } else {
        startSearchDevices();
    }
  }
</script>

<h1 class="text-3xl font-bold">Add Device</h1>
<div class="flex justify-between items-center">
  <h2>
    {#if isScanning}
      Scanning<AnimatedElipses />
    {:else}
      {devices.length} devices found
    {/if}
  </h2>
  <button
    class={`p-2 rounded-full bg-gray-200 ${isScanning ? " animate-spin" : ""}`}
    onclick={() => toggleSearch()}
  >
    <RefreshIcon />
  </button>
</div>
<div class="flex flex-col overflow-y-scroll grow m-2 gap-2">
  {#each devices as d}
    <button class="flex flex-col items-start bg-gray-200 px-2 py-px rounded-md" onclick={() => oncomplete(d)}>
      <div>
        {d.name}
      </div>
      <div class="text-sm text-gray-500 -mt-1">
        {d.address}
      </div>
    </button>
  {/each}
</div>
