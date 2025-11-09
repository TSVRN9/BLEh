<script lang="ts">
  import { fade, scale } from "svelte/transition";

  interface ModalProps {
    oncomplete: () => void;
    children?: any;
    class?: string;
  }

  let { oncomplete, children, class: clazz }: ModalProps = $props();
</script>

<!-- svelte-ignore a11y_click_events_have_key_events -->
<!-- svelte-ignore a11y_no_static_element_interactions -->
<div
  class="z-20 fixed bg-black/40 top-0 left-0 w-screen h-screen flex items-center justify-center"
  onclick={() => oncomplete()}
  transition:fade={{ duration: 100 }}
>
  <div
    class={"rounded-2xl bg-white p-4 flex flex-col z-30 " + clazz}
    onclick={(e) => e.stopPropagation()}
    transition:scale
  >
  {@render children()}
</div>
</div>
