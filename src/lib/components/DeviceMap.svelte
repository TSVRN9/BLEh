<script lang="ts">
    import Map from 'ol/Map.js';
    import View from 'ol/View.js';
    import OSM from 'ol/source/OSM.js';
    import TileLayer from 'ol/layer/Tile.js';
    import { onDestroy, onMount } from 'svelte';

    let mapDiv = $state() as HTMLDivElement;
    let map: Map;

    onMount(() => {
        map = new Map({
            layers: [
                new TileLayer({source: new OSM()}),
            ],
            view: new View({
                center: [0, 0],
                zoom: 2,
            }),
            target: mapDiv,
            controls: []
        }); 
    });

    onDestroy(() => {
        map.setTarget(undefined);
        map.dispose();
    });
</script>

<div bind:this={mapDiv} class="absolute w-full h-full bg-amber-50"></div>
