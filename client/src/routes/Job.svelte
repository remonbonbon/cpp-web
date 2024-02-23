<script>
  import { onMount } from "svelte";

  export let id = null;
  export let imageFile = null;

  let imgDom = null;

  onMount(() => {
    // console.log(`onMount ${id} ${imageFile.name}`);

    const reader = new FileReader();
    reader.onload = function (e) {
      imgDom.setAttribute("src", e.target.result);
      // console.log(`Loaded image ${id}`);
    };
    reader.readAsDataURL(imageFile);
  });
</script>

<!------------------------------------>

<div>
  <div>#{id} ({imageFile.type}) {imageFile.name}</div>
  <img src="" alt="upload" class="img" bind:this={imgDom} />
</div>

<!------------------------------------>
<style>
  .img {
    height: 150px;
  }
</style>
