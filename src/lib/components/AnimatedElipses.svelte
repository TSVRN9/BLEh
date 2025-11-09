<script lang="ts">
  import { onDestroy, onMount } from "svelte";
  import { render } from "svelte/server";

  let animationFrame = $state(0);
  let intervalId: number;

  onMount(() => {
    const delay = 0.5*1000;
    intervalId = setInterval(() => {
        tickAnimation();
    }, delay);
  })

  onDestroy(() => {
    clearInterval(intervalId);
  })

  function tickAnimation() {
    animationFrame++;
    animationFrame %= 3;
  }

  function renderSpan(n: number) {
    switch (n) {
        case 0: return '.';
        case 1: return '..';
        case 2: return '...';
        default: throw new Error('Should never get here! @renderSpan');
    }
  }
</script>

<span>{renderSpan(animationFrame)}</span>