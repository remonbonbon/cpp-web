<script>
  import { onMount } from "svelte";
  import axios from "axios";

  import config from "./config";

  export let id = null;
  export let imgUrl = null;
  export let originalFilename = null;
  export let createdAt = null;
  export let finishedAt = null;

  function formatDate(date) {
    if (!date) return "";
    return date.toLocaleString("ja-JP", {
      year: "numeric",
      month: "2-digit",
      day: "2-digit",
      hour12: false,
      hour: "2-digit",
      minute: "2-digit",
      second: "2-digit",
      fractionalSecondDigits: 3,
    });
  }

  onMount(() => {
    const intervalId = setInterval(async function () {
      try {
        const res = await axios({
          method: "get",
          baseURL: config.baseURL,
          url: `/api/jobs/${id}`,
        });
        if (!res.data) return;
        console.log(res.data);
        const d = res.data;

        if (d.filename) imgUrl = `${config.baseURL}/data/${d.filename}`;
        if (d.originalFilename) originalFilename = d.originalFilename;
        if (d.createdAt) createdAt = new Date(d.createdAt);
        if (d.finishedAt) finishedAt = new Date(d.finishedAt);

        if (d.result) {
          clearInterval(intervalId);
        }
      } catch (e) {
        console.error(e);
      }
    }, 1000);
  });
</script>

<!------------------------------------>

<div class="container">
  <div class="header">&lt;{id}&gt;</div>
  <div class="data">
    {#if imgUrl}
      <img src={imgUrl} alt="upload" class="img" />
    {/if}
    <div>
      <div><b>ファイル名</b>: {originalFilename ?? ""}</div>
      <div><b>開始日時</b>: {formatDate(createdAt)}</div>
      <div><b>完了日時</b>: {formatDate(finishedAt)}</div>
    </div>
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
  .data {
    display: flex;
    flex-flow: row;
    column-gap: 1ex;
  }
  .img {
    height: 150px;
  }
</style>
