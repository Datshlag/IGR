var w = 600;
var h = 800;

var svg = d3.select("body").append("svg").attr("with", w).attr("height", h);

d3.tsv("data/france.tsv").get(function(error, rows) {
    console.log("Loaded " + rows.length + " rows");
    if(rows.length > 0) {
      console.log("First row: ", rows[0]);
      console.log("Last row: ", rows[rows.length - 1])
    }
});
