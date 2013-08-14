(function ($) {
  Drupal.behaviors.zoomTree = {
    attach: function(context, settings) {
    /*
     * <insert js here>
     */


      var flare ={
 "name": "flare",
 "children": [
  {
   "name": "analytics",
   "children": [
    {
     "name": "cluster",
     "children": [
      {"name": "AgglomerativeCluster", "size": 3938},
      {"name": "CommunityStructure", "size": 3812},
      {"name": "MergeEdge", "size": 743}
     ]
    },
    {
     "name": "graph",
     "children": [
      {"name": "BetweennessCentrality", "size": 3534},
      {"name": "LinkDistance", "size": 5731}
     ]
    }
   ]
  }
 ]
};

     //frame.
      var margin = {top: 10, right: 10, bottom: 100, left: 40},
        margin2 = {top: 430, right: 10, bottom: 20, left: 40},
        height = 500 - margin.top - margin.bottom,
        height2 = 500 - margin2.top - margin2.bottom,
        width = 960 - margin.left - margin.right,
        root,
        root2;

      var x = d3.scale.linear().range([0, width]),
        x2 = d3.scale.linear().range([0, width]),
        y = d3.scale.linear().range([height, 0]),
        y2 = d3.scale.linear().range([height2, 0]);
      var xAxis = d3.svg.axis().scale(x).orient("bottom"),
          xAxis2 = d3.svg.axis().scale(x2).orient("bottom"),
          yAxis = d3.svg.axis().scale(y).orient("left");

      var tree = d3.layout.tree()
          .size([width, height]);
      var tree2 = d3.layout.tree()
          .size([width, height2]);

      var root = flare;
        root.x0 = 0;
        root.y0 = height/2;
      var root2 = flare;
        root2.x0 = 0;
        root2.y0 = (height2)/2;


      //brush
      var brush = d3.svg.brush()
        .x(x2)
        .on("brush", brushed);

      //area
      var area = d3.svg.area()
        .interpolate("monotone")
        .x(function(d) { return x(d); })
        .y0(height)
        .y1(function(d) { return y(d); });
      var area2 = d3.svg.area()
        .interpolate("monotone")
        .x(function(d) { return x2(d); })
        .y0(height2)
        .y1(function(d) { return y2(d); });

      //visualization
      //tree layout components.
      var diagonal = d3.svg.diagonal();
      var diagonal2 = d3.svg.diagonal();

      var svg = d3.select('#zoomtree').append("svg")
        .attr("width", width + margin.left + margin.right)
        .attr("height", height + margin.top + margin.bottom);
      //detail
      var focus = svg.append("g")
        .attr("transform", "translate (" + margin.left + ", " + margin.top + ")");
      //gestalt
      var context = svg.append("g")
        .attr("transform", "translate (" + margin2.left + ", " + margin2.top + ")");

      var nodes = tree.nodes(flare);
      var links = tree.links(nodes);

      var node = focus.selectAll(".node")
        .data(nodes)
        .enter()
        .append("g")
          .attr("class", "node")
          .attr("transform", function(d) { return "translate (" + d.x + ", " + d.y + ")"; });
      node.append("circle")
        .attr("r", 5);

      var nodes2 = tree2.nodes(root2);
      var links2 = tree2.links(nodes2);
      var node2 =  context.selectAll(".node2")
        .data(nodes2)
        .enter()
        .append("g")
          .attr("class", "node2")
          .attr("transform", function(d) { return "translate (" + d.x + ", " + d.y + ")"; });
      node2.append("circle")
        .attr("r", 2);

      function brushed() {
        x.domain(brush.empty() ? x2.domain() : brush.extent());
        focus.select("path").attr('d', area);
        focus.select(".x.axis").call(xAxis);
      }
    /*
     * <custom script ends here>
     */
    }
  };
}) (jQuery);
