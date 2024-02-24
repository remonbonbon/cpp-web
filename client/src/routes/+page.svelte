<script>
  import { onMount } from "svelte";
  import { ulid } from "ulid";
  import { fromUint8Array as base64Encode } from "js-base64";
  import axios from "axios";

  import Job from "./Job.svelte";
  import config from "./config";

  let jobs = [];
  let isDragOver = false;
  let dragOverCounter = 0;

  function onDrop(e) {
    isDragOver = false;
    dragOverCounter = 0;

    _addImages(e.dataTransfer.files);
  }

  function onInputFiles(e) {
    _addImages(e.target.files);

    e.target.value = ""; // 同じファイルを連続で選ぶとchangeイベントが発生しない
  }

  function _addImages(files) {
    for (const file of files) {
      const id = ulid();
      jobs.unshift({
        id,
      });

      (async function () {
        try {
          const buf = await file.arrayBuffer();
          const base64 = base64Encode(new Uint8Array(buf));
          const res = await axios({
            method: "post",
            baseURL: config.baseURL,
            url: `/api/jobs/${id}`,
            data: {
              filename: file.name,
              type: file.type,
              data: base64,
            },
          });
          console.log(res.data);
        } catch (e) {
          console.error(e);
        }
      })();
    }

    // Trigger reactive
    jobs = jobs;
  }

  onMount(async () => {
    const res = await axios({
      method: "get",
      baseURL: config.baseURL,
      url: `/api/jobs`,
    });
    console.log(res);
    jobs = res?.data?.jobs.map((id) => ({ id })) ?? [];
  });
</script>

<!------------------------------------>

<!-- svelte-ignore a11y-no-static-element-interactions -->
<div
  class="container"
  on:dragenter|preventDefault={() => {
    dragOverCounter++;
    isDragOver = true;
    // console.log("dragenter", dragOverCounter, isDragOver);
  }}
  on:dragleave|preventDefault={() => {
    dragOverCounter--;
    if (dragOverCounter <= 0) isDragOver = false;
    // console.log("dragleave", dragOverCounter, isDragOver);
  }}
>
  <div
    class="dropZone"
    class:dragOver={isDragOver}
    on:drop|preventDefault={onDrop}
    on:dragover|preventDefault={() => {}}
  >
    <span>画像ファイルをここにドラッグ＆ドロップ</span>
    <span class:hiddenOnDrag={isDragOver}>
      or <input
        type="file"
        accept=".jpg,.jpeg,.png"
        multiple
        on:change={onInputFiles}
      />
    </span>
  </div>
  <div class="jobs" class:hiddenOnDrag={isDragOver}>
    {#each jobs as { id } (id)}
      <Job bind:id />
    {/each}
  </div>
</div>

<!------------------------------------>
<style>
  :global(html, body) {
    margin: 0;
    padding: 0;
  }
  :global(*) {
    box-sizing: border-box;
  }

  .container {
    padding: 1rem 1rem;
    width: 100vw;
    height: 100vh;
    display: flex;
    flex-flow: column;
  }

  .dropZone {
    flex-shrink: 0;
    border: 6px dashed #888;
    width: 100%;
    height: 8rem;

    display: flex;
    flex-flow: column;
    row-gap: 1ex;
    align-items: center;
    justify-content: center;
  }
  .dropZone.dragOver {
    width: 100%;
    height: 100%;
    background-color: #fee;
    border-color: #f00;
  }

  .jobs {
    margin-top: 1rem;
    overflow-y: auto;
    padding-right: 1rem; /* スクロールバー幅の分 */

    display: flex;
    flex-flow: column;
    row-gap: 0.5rem;
  }

  .hiddenOnDrag {
    display: none;
  }
</style>
