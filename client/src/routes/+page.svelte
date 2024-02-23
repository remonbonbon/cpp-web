<script>
  // import shuffle from "lodash/shuffle";
  // import sample from "lodash/sample";
  import Job from "./Job.svelte";

  let jobs = [];
  let id = 1;
  let isDragOver = false;
  let dragOverCounter = 0;

  function dropHandler(ev) {
    isDragOver = false;
    dragOverCounter = 0;

    for (const file of ev.dataTransfer.files) {
      // console.log(file);

      jobs.unshift({
        id: id++,
        imageFile: file,
      });

      // Trigger reactive
      jobs = jobs;
    }
  }
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
    on:drop|preventDefault={dropHandler}
    on:dragover|preventDefault={() => {}}
  >
    <span>画像ファイルをここにドラッグ＆ドロップ</span>
  </div>
  <div class="jobs" class:dragOver={isDragOver}>
    {#each jobs as { id, imageFile } (id)}
      <Job bind:id bind:imageFile />
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
  .jobs.dragOver {
    display: none;
  }
</style>
