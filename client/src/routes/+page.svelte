<script>
  // import shuffle from "lodash/shuffle";
  // import sample from "lodash/sample";

  function dropHandler(ev) {
    console.log("File(s) dropped");

    // Prevent default behavior (Prevent file from being opened)
    ev.preventDefault();

    // // if (ev.dataTransfer.items) {
    //   // Use DataTransferItemList interface to access the file(s)
    //   [...ev.dataTransfer.items].forEach((item, i) => {
    //     // If dropped items aren't files, reject them
    //     console.log(item);
    //     if (item.kind === "file") {
    //       const file = item.getAsFile();
    //       console.log(`… file[${i}].name = ${file.name}`);
    //     }
    //   });
    // // } else {
    // Use DataTransfer interface to access the file(s)
    [...ev.dataTransfer.files].forEach((file, i) => {
      console.log(file);
      console.log(`… file[${i}].name = ${file.name}`);
    });
    // }
  }

  function dragOverHandler(ev) {
    // console.log("File(s) in drop zone");

    // Prevent default behavior (Prevent file from being opened)
    ev.preventDefault();
  }
</script>

<!------------------------------------>

<!-- svelte-ignore a11y-no-static-element-interactions -->
<div class="drop_zone" on:drop={dropHandler} on:dragover={dragOverHandler}>
  <p>Drag one or more files to this <i>drop zone</i>.</p>
</div>

<!------------------------------------>
<style>
  .drop_zone {
    border: 5px solid blue;
    width: 200px;
    height: 100px;
  }
</style>
