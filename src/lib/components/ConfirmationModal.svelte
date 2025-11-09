<script lang="ts">
  import Modal from "./Modal.svelte";

    interface ConfirmationModalProps {
        oncomplete: (confirmed: boolean) => void;
    }

    let { oncomplete } = $props();
</script>

<Modal class="gap-4" oncomplete={() => oncomplete(false)}>
    <h1 class="text-3xl font-bold">Are you sure?</h1>
    <div class="flex items-center justify-center gap-4 w-full">
        <button class="p-2 rounded-3xl bg-red-300 w-16" onclick={() => oncomplete(true)}>Yes</button>
        <button class="p-2 rounded-3xl bg-gray-300 w-16" onclick={() => oncomplete(false)}>No</button>
    </div>
</Modal>