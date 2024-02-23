<script>
  import { onMount } from "svelte";
  import { fromUint8Array as base64Encode } from "js-base64";
  import axios from "axios";
  import { dev as isDev } from "$app/environment";

  export let id = null;
  export let imageFile = null;

  let imgDom = null;
  const baseURL = isDev ? "http://127.0.0.1:8080/" : "";

  onMount(async () => {
    // console.log(`onMount ${id} ${imageFile.name}`);

    const dataUrlReader = new FileReader();
    dataUrlReader.onload = function (e) {
      imgDom.setAttribute("src", e.target.result);
    };
    dataUrlReader.readAsDataURL(imageFile);

    const buf = await imageFile.arrayBuffer();
    // console.log("binary size", buf.byteLength);
    const base64 = base64Encode(new Uint8Array(buf));
    // console.log("base64 size", base64.length);

    try {
      const res = await axios({
        method: "post",
        baseURL,
        url: `/api/jobs/${id}`,
        data: {
          filename: imageFile.name,
          type: imageFile.type,
          data: base64,
        },
      });
      console.log(res.data);
    } catch (e) {
      console.error(e);
    }
  });
</script>

<!------------------------------------>

<div class="container">
  <div class="header">&lt;{id}&gt; {imageFile.name}</div>
  <div>
    <img src="" alt="upload" class="img" bind:this={imgDom} />
  </div>
</div>

<!------------------------------------>
<style>
  .container {
    border: 1px solid #ccc;
    padding: 0.5rem;

    display: flex;
    flex-flow: column;
    row-gap: 1ex;
  }
  .img {
    height: 150px;
  }
</style>
