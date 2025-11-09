<script lang="ts">
  import Map from "ol/Map.js";
  import View from "ol/View.js";
  import OSM from "ol/source/OSM.js";
  import TileLayer from "ol/layer/Tile.js";
  import { onDestroy, onMount } from "svelte";
  import type { DeviceLocation } from "$lib/types/location";
  import { fromLonLat } from "ol/proj";
  import { Feature } from "ol";
  import { LineString, Point } from "ol/geom";
  import CircleStyle from "ol/style/Circle.js";
  import Style from "ol/style/Style";
  import Stroke from "ol/style/Stroke";
  import Fill from "ol/style/Fill";
  import VectorLayer from "ol/layer/Vector";
  import VectorSource from "ol/source/Vector";

  interface DeviceMapProps {
    locations: DeviceLocation[];
  }

  let { locations }: DeviceMapProps = $props();

  const OSM_LAYER = new TileLayer({ source: new OSM() });
  let mapDiv = $state() as HTMLDivElement;
  let map: Map;

  onMount(() => {
    map = new Map({
      layers: [OSM_LAYER],
      view: new View({
        center: fromLonLat([-72.05977, 41.85843]),
        zoom: 8,
      }),
      target: mapDiv,
      controls: [],
    });
  });

  $effect(() => {
    const coords = locations.map((c) => fromLonLat([c.longitude, c.latitude]));

    const line = new Feature({
      geometry: new LineString(coords),
    });

    const pointFeatures = coords.map(
      (c) =>
        new Feature({
          geometry: new Point(fromLonLat(c)),
        })
    );

    const style = new Style({
      stroke: new Stroke({
        color: "blue",
        width: 5,
      }),
      image: new CircleStyle({
        radius: 8,
        fill: new Fill({
          color: "blue",
        }),
      }),
    });

    const vectorSource = new VectorSource({
      features: [line, ...pointFeatures],
    });

    const layer = new VectorLayer({
        source: vectorSource,
        style
    });

    map.setLayers([OSM_LAYER, layer]);
  });

  onDestroy(() => {
    map.setTarget(undefined);
    map.dispose();
  });
</script>

<div bind:this={mapDiv} class="absolute w-full h-full bg-amber-50"></div>
