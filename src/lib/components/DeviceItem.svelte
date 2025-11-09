<script lang="ts">
  import type { Device } from "$lib/types/device";
  import BellIcon from "./icons/BellIcon.svelte";
  import TrashIcon from "./icons/TrashIcon.svelte";

  interface DeviceItemProps {
    device: Device;
    selected?: boolean;
    onselect?: () => void;
    ondelete?: () => void;
    onalarm?: () => void;
  }

  let { device, selected, onselect, ondelete, onalarm }: DeviceItemProps =
    $props();
</script>

<!-- svelte-ignore a11y_no_static_element_interactions -->
<!-- svelte-ignore a11y_click_events_have_key_events -->
<div
  class={`p-4 flex justify-between items-center bg-gray-100 rounded-3xl ${selected ? "bg-gray-300" : ""}`}
  onclick={() => {
    if (onselect) onselect();
  }}
>
  <div class="flex flex-col">
    <div class="text-xl">{device.displayName}</div>
    <div class="text-sm text-gray-500 -mt-1">
      {device.macAddress}
    </div>
  </div>
  <div class="flex justify-center items-center gap-2">
    {#if selected}
      <div class="flex justify-center items-center">
        <button
          class="bg-blue-600 text-white p-2 rounded-full"
          onclick={(e) => {
            e.stopPropagation();
            if (onalarm) onalarm();
          }}
        >
          <BellIcon />
        </button>
      </div>
      <div class="flex justify-center items-center">
        <button
          class="bg-red-600 text-white p-2 rounded-full"
          onclick={(e) => {
            e.stopPropagation();
            if (ondelete) ondelete();
          }}
        >
          <TrashIcon />
        </button>
      </div>
    {/if}
  </div>
</div>
