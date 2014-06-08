/* Inspired by Lee Byron's test data generator. */
function stream_layers(n, m, o) {
  if (arguments.length < 3) o = 0;
  function bump(a) {
    var x = 1 / (.1 + Math.random()),
        y = 2 * Math.random() - .5,
        z = 10 / (.1 + Math.random());
    for (var i = 0; i < m; i++) {
      var w = (i / m - y) * z;
      a[i] += x * Math.exp(-w * w);
    }
  }
  return d3.range(n).map(function() {
    var a = [], i;
    for (i = 0; i < m; i++) a[i] = o + o * Math.random();
    for (i = 0; i < 5; i++) bump(a);
    return a.map(stream_index);
  });
}

/* Another layer generator using gamma distributions. */
function stream_waves(n, m) {
  return d3.range(n).map(function(i) {
    return d3.range(m).map(function(j) {
      var x = 20 * j / m - i / 3;
      return 2 * x * Math.exp(-.5 * x);
    }).map(stream_index);
  });
}

function stream_index(d, i) {
  return {x: i, y: Math.max(0, d)};
}



//var test_data = stream_layers(3,128,.1).map(function(data, i) {
var test_data1 = stream_layers(3,128,.1).map(function(data, i) {
  return {
    key: 'Stream' + i,
    values: data
  };
});

var test_data = [];

var chart;

nv.addGraph({
  generate: function() {
    var width = 800,
        height = 500;
        chart = nv.models.multiBar()
          .width(width)
          .height(height)
          .margin(10)
          .stacked(true);

      chart.xAxis
          .tickFormat(d3.format(',f'));

      chart.yAxis
          .tickFormat(d3.format(',.1f'));


    var svg = d3.select('#test1 svg')
          .attr('width', width)
          .attr('height', height)
          .datum(test_data);

    svg.transition().duration(500).call(chart);
      d3.select('p').text(JSON.stringify(test_data));

    return chart;
  }
});

var i = 0;

receiveTypeCount = function(typeCounts) {
    if (test_data.length > 20) {
      test_data = [];
      i = 0;
    } else {
        ++i;
    }

    for (var t in typeCounts) {
        test_data.push({key: t, color: 'red', values: [{ x: i, y: typeCounts[t] }]});
        //test_data[t] = [typeCounts[t]];
    }
};

function establishBridge() {
  if(rbkitClient) {
    clearInterval(interval);
    rbkitClient.sendDatatoJs.connect(receiveTypeCount);
  }
}
var interval = setInterval(establishBridge, 1000);

function renderGraph() {
    d3.select('p').text(JSON.stringify(test_data));
    // chart.update();
    var svg = d3.select('#test1 svg')
          .datum(test_data);

    svg.call(chart);
}

setInterval(renderGraph, 1000);
