<script lang="ts">
  import type { Device } from "$lib/types/device";
  import { fade, scale } from "svelte/transition";
  import { type BleDevice } from "@mnlphlp/plugin-blec";
  import SelectDeviceState from "./SelectDeviceState.svelte";
  import Modal from "../Modal.svelte";

  interface PairingModalProps {
    oncomplete: (device?: Device) => void;
    ignored: Set<string>; // mac addresses to ignore
  }

  let { oncomplete, ignored }: PairingModalProps = $props();


  // maybe include additional screens later..
  // @ts-ignore
  let state = $state('select') as 'select';
  // // @ts-ignore
  // let bleDevice = $state() as BleDevice;


  function onSelectComplete(d: BleDevice) {
    // bleDevice = d;
    oncomplete({
      displayName: d.name,
      macAddress: d.address,
    });
  }
</script>

<Modal class="w-5/6 h-2/3" oncomplete={() => oncomplete()}>
  {#if state === 'select'}
    <SelectDeviceState {ignored} oncomplete={onSelectComplete} />
  <!-- {:else}
    <DeviceInformationState oncomplete={(d: Device) => oncomplete(d)} {bleDevice}/> -->
  {/if}
</Modal>
