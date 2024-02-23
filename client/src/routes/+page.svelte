<script>
  // import shuffle from "lodash/shuffle";
  // import sample from "lodash/sample";
  import Job from "./Job.svelte";

  let jobs = [];
  let id = 1;

  function dropHandler(ev) {
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
<div class="container">
  <div
    class="dropZone"
    on:drop|preventDefault={dropHandler}
    on:dragover|preventDefault={() => {}}
  >
    <span>画像ファイルをここにドラッグ＆ドロップ</span>
  </div>
  <div class="jobs">
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
    padding: 0 1rem;
  }
  .dropZone {
    margin-top: 1rem;
    border: 4px dashed #888;
    width: 100%;
    height: 100px;
    display: flex;
    align-items: center;
    justify-content: center;
  }
  .jobs {
    margin-top: 1rem;
  }
</style>
