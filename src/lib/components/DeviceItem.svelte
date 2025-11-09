<script lang="ts">
  import type { Device } from "$lib/types/device";
  import TrashIcon from "./icons/TrashIcon.svelte";

  interface DeviceItemProps {
    device: Device;
    selected?: boolean;
    onclick?: () => void;
  }

  let { device, selected }: DeviceItemProps = $props();
</script>

<div class="p-4 flex justify-between items-center bg-gray-200 rounded-3xl">
  <div class="flex flex-col">
    <div class="text-xl">{device.displayName}</div>
    <div class="text-sm text-gray-500 -mt-1">
      {device.macAddress}
    </div>
  </div>
  <div class="flex justify-center items-center">
    <button class="bg-red-600 text-white p-2 rounded-full">
      <TrashIcon />
    </button>
  </div>
</div>
