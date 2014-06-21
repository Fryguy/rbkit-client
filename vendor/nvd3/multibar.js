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


var test_data2 = [
            {"key":"Foo","color":"red","values":[{"x":1,"y":2453},
                                                {"x":2,"y":2453},
                                                {"x":3,"y":2453},
                                                {"x":4,"y":2453}
                ]},
            {"key":"String","color":"orange","values":[{"x":1,"y":17166},
                                                       {"x":2,"y":17168},
                                                       {"x":3,"y":17170},
                                                       {"x":4,"y":17172}]}
        ];

//var test_data = [{"key":"String","values":[{"x":0,"y":3},{"x":1,"y":0},{"x":2,"y":2},{"x":3,"y":2},{"x":4,"y":3},{"x":5,"y":4},{"x":6,"y":5},{"x":7,"y":6},{"x":8,"y":7},{"x":9,"y":8},{"x":10,"y":9}]},{"key":"Foo","values":[{"x":0,"y":4},{"x":1,"y":0},{"x":2,"y":0},{"x":4,"y":1},{"x":5,"y":1},{"x":6,"y":1},{"x":7,"y":1},{"x":8,"y":1},{"x":9,"y":1},{"x":10,"y":1}]}];

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

    svg.call(chart);
    d3.select('p').text(JSON.stringify(test_data));

    return chart;
  }
});

var xAxisIter = 0;

receiveTypeCount = function(typeCountsFromBackend) {
    var typeCounts = {};
    for (var attr in typeCountsFromBackend) {
        if (typeCountsFromBackend.hasOwnProperty(attr))
            typeCounts[attr] = typeCountsFromBackend[attr];
    }

    //d3.select('p').text(JSON.stringify(typeCounts));

    if (xAxisIter < 0) {
        rbkitClient.sendDatatoJs.disconnect(receiveTypeCount);
        return;
    }

    // first update the existing values
    for (var iter = 0; iter != test_data.length; ++iter) {
        var entry = { x: xAxisIter, y: 0 };
        var key = test_data[iter].key;

        if (key in typeCounts) {
            entry.y = typeCounts[key];
            delete typeCounts[key];
        }

        test_data[iter].values.push(entry);
    }

    // for new values, get base value, and initialize all defaults.
    if (xAxisIter == 0) {
        for (var key in typeCounts) {
            test_data.push({ key: key, values: [{ x: 0, y: typeCounts[key] }] });
        }
    } else {
        var baseEntryValues = test_data[0].values;
        for (var key in typeCounts) {
            var newData = { key: key, values: [] };
            for (iter = 0; iter != (baseEntryValues.length - 1); ++iter) {
                newData.values.push({ x: baseEntryValues[iter].x, y: 0 });
            }
            newData.values.push({ x: xAxisIter, y: typeCounts[key] });
            test_data.push(newData);
        }
    }

    if ( xAxisIter && (xAxisIter%100 == 0) ) {
        for (var iter = 0; iter != test_data.length; ++iter) {
            values = test_data[iter].values;
            test_data[iter].values = values.slice(-100, -1);
        }
    }

    d3.select('p').text(xAxisIter);
    ++xAxisIter;
};

function establishBridge() {
  if(rbkitClient) {
    clearInterval(interval);
    rbkitClient.sendDatatoJs.connect(receiveTypeCount);
  }
}
var interval = setInterval(establishBridge, 1000);

function renderGraph() {
    //d3.select('p').text(JSON.stringify(test_data));
    // chart.update();
    var svg = d3.select('#test1 svg')
          .datum(test_data);

    svg.call(chart);
}

var renderInterval = setInterval(renderGraph, 1000);
