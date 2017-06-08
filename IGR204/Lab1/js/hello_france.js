var w = 800;
var h = 800;
var dataset = [];

var svg = d3.select("body").append("svg").attr("width", w).attr("height", h);
var info_text = svg.append("text").attr("id", 'info');

d3.tsv("data/france.tsv")
	.row(function(d, i) {

		return {
			codePostal: +d["Postal Code"],
			inseecode: +d.inseecode,
			place: d.place,
			x: +d.x,
			y: +d.y,
			population: +d.population,
			density: +d.density
		};

	})
	.get(function(error, rows) {

    	x = d3.scale.linear().domain(d3.extent(rows, function(row) { return row.x }))
					 .range([0, w]);
		y = d3.scale.linear().domain(d3.extent(rows, function(row) { return -row.y }))
					 .range([0, h]);
	 	r = d3.scale.linear().domain(d3.extent(rows, function(row) { return row.population }))
	 				 .range([0.1, 75])

    	dataset = rows;
    	draw()

	});

function draw() {

	svg.selectAll("rect")
		.data(dataset)
		.enter()
		.append("circle")
		.attr("cx", function(d) { return x(d.x) })
		.attr("cy", function(d) { return y(-d.y) })
		.attr("r", function(d) { return r(d.population) })
		.attr("fill", "#044B94")
		.attr("fill-opacity", 0.5)

	svg.selectAll("rect")
		.data(dataset)
		.enter()
		.append("rect")
		.attr("width", 1)
		.attr("height", 1)
		.attr("x", function(d) { return x(d.x) })
		.attr("y", function(d) { return y(-d.y) })
		.on("mouseover", mouse_over)
		.on("mouseout", mouse_out)

}

function mouse_over(d) {

	d3.select("#info")
		.attr("width", 10)
		.attr("height", 10)
        .attr("x", x(d.x) - 5)
        .attr("y", y(-d.y) - 5);
   	info = "Nom: " + d.place + "<br> Population: " + d.population +
        "<br> Densité: " + d.density;
    info_text.html(info)

}

function mouse_out(d) {

	d3.select("#info")
		.attr("width", 1)
		.attr("height", 1)
        .attr("x", x(d.x))
        .attr("y", y(-d.y));
    info_text.html("")

}