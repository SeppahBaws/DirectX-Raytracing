const fs = require("fs");
const PNG = require("pngjs").PNG;
const pixelmatch = require("pixelmatch");

const folders = [
    "../bistro_ext-1", "../bistro_ext-2", "../bistro_ext-3",
    "../bistro_int-1", "../bistro_int-2", "../bistro_int-3",
    "../san_miguel-1", "../san_miguel-2", "../san_miguel-3", "../san_miguel-4",
    "../sponza-1", "../sponza-2", "../sponza-3",
];

folders.forEach((folder) => {
    console.log(`Comparing ${folder + "/output-late.png"} and ${folder + "/output-early.png"}`);
    const img1 = PNG.sync.read(fs.readFileSync(folder + "/output-late.png"));
    const img2 = PNG.sync.read(fs.readFileSync(folder + "/output-early.png"));
    const {width, height} = img1;
    const diff = new PNG({width, height});

    pixelmatch(img1.data, img2.data, diff.data, width, height, {threshold: 0.01, diffColor: [255,0,0]});

    console.log(`Writing result to ${folder + "/diff.png"}`);
    fs.writeFileSync(folder + "/diff.png", PNG.sync.write(diff));

    console.log("==================================\n");
});
