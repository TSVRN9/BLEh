<script lang="ts">
  import { connect, disconnect, send } from "@mnlphlp/plugin-blec";
  import AnimatedElipses from "./AnimatedElipses.svelte";
  import Modal from "./Modal.svelte";
  import type { Device } from "$lib/types/device";
  import { onDestroy, onMount } from "svelte";

  interface AlarmModalProps {
    device: Device;
    oncomplete: () => void;
  }

  let { device, oncomplete }: AlarmModalProps = $props();

  const service = "180f";
  const characteristic = "2a20";
  const fullService = `0000${service}-0000-1000-8000-00805F9B34FB`;
  const fullCharacteristic = `0000${characteristic}-0000-1000-8000-00805F9B34FB`;

  // @ts-ignore
  let state = $state("connecting") as
    | "failed"
    | "connecting"
    | "sending"
    | "playing";
  let isActive = true;

  onMount(() => {
    start();
  });

  async function start() {
    try {
      state = "connecting";
      await connect(device.macAddress, () => {
        oncomplete();
      });
      if (!isActive) return;
      state = "sending";
      await send(fullCharacteristic, [1], "withoutResponse", fullService);
      if (!isActive) return;
      state = "playing";
    } catch (e) {
      console.error(e);
      state = "failed";
    }
  }

  async function modalcomplete() {
    if (state === "sending" || state === "playing") {
      await send(fullCharacteristic, [0], "withoutResponse", fullService).catch(console.error);
      await disconnect();
    }
    isActive = false;
    oncomplete();
  }
</script>

<Modal
  class="gap-4 h-1/5 w-5/6 justify-center items-center"
  oncomplete={modalcomplete}
>
  {#if state === "connecting"}
    <h1 class="text-3xl font-bold">Connecting<AnimatedElipses /></h1>
  {:else if state === "sending"}
    <h1 class="text-3xl font-bold">Sending message<AnimatedElipses /></h1>
  {:else if state === "playing"}
    <h1 class="text-3xl font-bold">Playing alarm!</h1>
  {:else}
    <h1 class="text-2xl font-bold">Something went wrong!</h1>
    <button
      class="p-2 text-white bg-blue-600 rounded-2xl"
      onclick={() => start()}>Try again</button
    >
  {/if}
</Modal>
