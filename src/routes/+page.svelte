<script lang="ts">
  import DeviceMap from "$lib/components/DeviceMap.svelte";
  import DevicePane from "$lib/components/DevicePane.svelte";
  import PairingModal from "$lib/components/pairing/PairingModal.svelte";
  import type { Device } from "$lib/types/device";

  let devices = $state([]) as Device[];

  let showdialog = $state(false);

  function onadd() {
    showdialog = true;
  }
  
  function oncomplete(device: Device | undefined) {
    showdialog = false;
    if (device) {
      devices.push(device)
    }
  }
</script>

<div class='h-screen w-screen'>
  <DeviceMap />
  <DevicePane {onadd} {devices} />
  {#if showdialog}
    <PairingModal ignored={new Set(devices.map(d => d.macAddress))} {oncomplete} />
  {/if}
</div>