<script lang="ts">
  import { CupertinoPane } from "cupertino-pane";
  import { onDestroy, onMount } from "svelte";
  import DeviceItem from "./DeviceItem.svelte";
  import type { Device } from "$lib/types/device";
  import PlusIcon from "./icons/PlusIcon.svelte";

  interface DevicePaneProps {
    devices: Device[];
    onpair?: () => void;
    ondelete?: (device: Device) => void;
    onselect?: (device: Device) => void;
    selectedDevice?: Device;
  }

  let { devices, onpair, ondelete, onselect, selectedDevice }: DevicePaneProps = $props();

  let paneDiv = $state() as HTMLDivElement;
  let pane: CupertinoPane;

  onMount(() => {
    pane = new CupertinoPane(paneDiv, {
      buttonDestroy: false,
      scrollZeroDragBottom: false,
      breaks: {
        top: { enabled: true, height: window.innerHeight * 0.6, bounce: true },
        middle: {
          enabled: true,
          height: window.innerHeight * 0.3,
          bounce: true,
        },
        bottom: { enabled: false, height: 40 },
      },
    });
    pane.present();
  });

  onDestroy(() => {
    pane.destroy();
  });
</script>

<div class="flex flex-col items-center px-8 py-4 gap-4" bind:this={paneDiv}>
  <div class="flex justify-between items-center">
    <h1 class="text-3xl font-bold">Devices</h1>
    <button
      class="h-12 w-12 rounded-full bg-gray-200 flex justify-center items-center"
      aria-label="add device"
      onclick={onpair}
    >
      <PlusIcon />
    </button>
  </div>
  <div class="flex flex-col pt-4 gap-2">
    {#if devices.length != 0}
      {#each devices as device}
        <DeviceItem
          {device}
          ondelete={() => {
            if (ondelete) ondelete(device);
          }}
          onselect={() => {
            if (onselect) onselect(device);
          }}
          selected={device === selectedDevice}
        />
      {/each}
    {:else}
      <div class="text-center text-gray-400">Add a new device to track!</div>
    {/if}
  </div>
</div>
